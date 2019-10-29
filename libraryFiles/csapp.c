/*
 * csapp.c - Functions for the CS:APP3e book
 *
 * Updated 10/2018 khg, mkasper:
 *   - Remove sio_puts, sio_putl
 *   - Add %p and %o to sio_printf
 *
 * Updated 8/2018 khg:
 *   - Add sio_printf, sio_assert
 *   - Improve comments
 *
 * Updated 6/2018 khg, gdidier:
 *   - Document caveat to async-signal-safety of unix_error
 *   - Print trailing newline in sio_error
 *   - Fix output of sio_ltoa(LONG_MIN)
 *   - Const-correctness for strings
 *
 * Updated 4/2017 shrikang:
 *   - Improved comments.
 *
 * Updated 4/2017 szz:
 *   - Improved style
 *   - Fixed several warnings
 *
 * Updated 10/2016 reb:
 *   - Made unix_error signal safe.
 *
 * Updated 10/2016 reb:
 *   - Fixed bug in sio_ltoa that didn't cover negative numbers
 *
 * Updated 2/2016 droh:
 *   - Updated open_clientfd and open_listenfd to fail more gracefully
 *
 * Updated 8/2014 droh:
 *   - New versions of open_clientfd and open_listenfd are reentrant and
 *     protocol independent.
 *
 *   - Added protocol-independent inet_ntop and inet_pton functions. The
 *     inet_ntoa and inet_aton functions are obsolete.
 *
 * Updated 7/2014 droh:
 *   - Aded reentrant sio (signal-safe I/O) routines
 *
 * Updated 4/2013 droh:
 *   - rio_readlineb: fixed edge case bug
 *   - rio_readnb: removed redundant EINTR check
 */
/* $begin csapp.c */
#include "csapp.h"

/* Signal safe I/O to be used internally to csapp library */
static ssize_t sio_puts(const char s[]);

/**************************
 * Error-handling functions
 **************************/
/* $begin errorfuns */
/* $begin unixerror */
/* Unix-style error */
void unix_error(const char *msg) {
    sio_puts(msg);
    sio_puts(": ");
    sio_error(strerror(errno));
    // This call to sio_error makes unix_error async-signal-unsafe.
    // See header comment in csapp.h for more information.
}
/* $end unixerror */

/* Posix-style error */
void posix_error(int code, const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    exit(0);
}

/* Getaddrinfo-style error */
void gai_error(int code, const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, gai_strerror(code));
    exit(0);
}

/* Application error */
void app_error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(0);
}
/* $end errorfuns */

/* Obsolete gethostbyname error */
void dns_error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(0);
}


/*********************************************
 * Wrappers for Unix process control functions
 ********************************************/

/**
 * @brief   Wrapper for libc fork(). Exits on failure.
 * @return  PID of child process in parent, 0 in child.
 */
/* $begin forkwrapper */
pid_t Fork(void) {
    pid_t pid;

    if ((pid = fork()) < 0) {
        unix_error("Fork error");
    }

    return pid;
}
/* $end forkwrapper */

/**
 * @brief Wrapper for libc execve(). Exits on error.
 * @param filename  Executable file
 * @param argv[]    Argument vector
 * @param envp[]    Environment variables
 */
void Execve(const char *filename, char *const argv[], char *const envp[]) {
    if (execve(filename, argv, envp) < 0) {
        unix_error("Execve error");
    }
}

/**
 * @brief   Wrapper for libc wait(). Exits on failure.
 * @param status    Pointer to status code.
 *
 * @return  PID of reaped child.
 */
/* $begin wait */
pid_t Wait(int *status) {
    pid_t pid;

    if ((pid  = wait(status)) < 0) {
        unix_error("Wait error");
    }

    return pid;
}
/* $end wait */

/**
 * @brief Wrapper for libc waitpid(). Exits on error.
 * @param pid       PID to wait on.
 * @param iptr      Pointer to status code.
 * @param options   Modify wait behavior.
 *
 * @return  PID of reaped child (see man waitpid(3)).
 */
pid_t Waitpid(pid_t pid, int *iptr, int options) {
    pid_t retpid;

    if ((retpid  = waitpid(pid, iptr, options)) < 0) {
        unix_error("Waitpid error");
    }

    return retpid;
}

/**
 * @brief   Wrapper for libc kill(). Exits on failure.
 * @param pid       PID of the process.
 * @param signum    Signal to send.
 */
/* $begin kill */
void Kill(pid_t pid, int signum) {
    int rc;

    if ((rc = kill(pid, signum)) < 0) {
        unix_error("Kill error");
    }
}
/* $end kill */

/**
 * @brief   Identical to libc pause(), but does not return a value.
 */
void Pause(void) {
    (void) pause();
    return;
}

/**
 * @brief   Identical to libc sleep().
 * @param secs  Count of seconds to wait for.
 *
 * @return  Zero after time elapsed, seconds left if interrupted.
 */
unsigned int Sleep(unsigned int secs) {
    return sleep(secs);
}

unsigned int Alarm(unsigned int seconds) {
    return alarm(seconds);
}

/**
 * @brief   Wrapper for libc setpgid(). Exits on failure.
 * @param pid   PID of the process.
 * @param pgid  New PGID.
 */
void Setpgid(pid_t pid, pid_t pgid) {
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0) {
        unix_error("Setpgid error");
    }

    return;
}

/**
 * @brief   Identical to libc getpgrp().
 * @return   The current process group.
 */
pid_t Getpgrp(void) {
    return getpgrp();
}

/************************************
 * Wrappers for Unix signal functions
 ***********************************/

/**
 * @brief   Wrapper for the new sigaction interface. Exits on error.
 * @param signum    Signal to set handler for.
 * @param handler   Handler function.
 *
 * @return  Previous disposition of the signal.
 */
/* $begin sigaction */
handler_t *Signal(int signum, handler_t *handler) {
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0) {
        unix_error("Signal error");
    }

    return old_action.sa_handler;
}
/* $end sigaction */

/**
 * @brief   Wrapper for libc sigprocmask(). Exits on failure.
 * @param how       How to modify the current mask.
 * @param set       Set of signals to modify the mask with.
 * @param oldset    Signal set to save old signal mask.
 */
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
    if (sigprocmask(how, set, oldset) < 0) {
        unix_error("Sigprocmask error");
    }

    return;
}

/**
 * @brief   Wrapper for libc sigemptyset(). Exits on failure.
 * @param set   Set to initialize to empty.
 */
void Sigemptyset(sigset_t *set) {
    if (sigemptyset(set) < 0) {
        unix_error("Sigemptyset error");
    }

    return;
}

/**
 * @brief   Wrapper for libc sigfillset(). Exits on failure.
 * @param set   Set to initialize to full.
 */
void Sigfillset(sigset_t *set) {
    if (sigfillset(set) < 0) {
        unix_error("Sigfillset error");
    }

    return;
}

/**
 * @brief   Wrapper for libc sigaddset(). Exits on failure.
 * @param set       Set to add signal to.
 * @param signum    Signal to add.
 */
void Sigaddset(sigset_t *set, int signum) {
    if (sigaddset(set, signum) < 0) {
        unix_error("Sigaddset error");
    }

    return;
}

/**
 * @brief   Wrapper for libc sigdelset(). Exits on failure.
 * @param set       Set to delete signal from.
 * @param signum    Signal to delete.
 */
void Sigdelset(sigset_t *set, int signum) {
    if (sigdelset(set, signum) < 0) {
        unix_error("Sigdelset error");
    }

    return;
}

/**
 * @brief   Wrapper for libc sigismember(). Exits on failure.
 * @param set       Set to check.
 * @param signum    Signal to check for.
 *
 * @return  1 if signal present in set, 0 otherwise.
 */
int Sigismember(const sigset_t *set, int signum) {
    int rc;
    if ((rc = sigismember(set, signum)) < 0) {
        unix_error("Sigismember error");
    }

    return rc;
}

/**
 * @brief   Wrapper for libc sigsuspend(). Exits on failure.
 * @param set   Set to check.
 *
 * @return  -1
 */
int Sigsuspend(const sigset_t *set) {
    int rc = sigsuspend(set); /* always returns -1 */
    if (errno != EINTR) {
        unix_error("Sigsuspend error");
    }

    return rc;
}

/*************************************************************
 * The Sio (Signal-safe I/O) package - simple reentrant output
 * functions that are safe for signal handlers.
 *************************************************************/

/* Private sio functions */

/* $begin sioprivate */
/* sio_reverse - Reverse a string (from K&R) */
static void sio_reverse(char s[], size_t len) {
    size_t i, j;
    for (i = 0, j = len - 1; i < j; i++, j--) {
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* write_digits - write digit values of v in base b to string */
static size_t write_digits(uintmax_t v, char s[], unsigned char b) {
    size_t i = 0;
    do {
        unsigned char c = v % b;
        if (c < 10) {
            s[i++] = c + '0';
        }
        else {
            s[i++] = c - 10 + 'a';
        }
    } while ((v /= b) > 0);
    return i;
}

/* Based on K&R itoa() */
/* intmax_to_string - Convert an intmax_t to a base b string */
static size_t intmax_to_string(intmax_t v, char s[], unsigned char b) {
    bool neg = v < 0;
    size_t len;

    if (neg) {
        len = write_digits(-v, s, b);
        s[len++] = '-';
    }
    else {
        len = write_digits(v, s, b);
    }

    s[len] = '\0';
    sio_reverse(s, len);
    return len;
}

/* uintmax_to_string - Convert a uintmax_t to a base b string */
static size_t uintmax_to_string(uintmax_t v, char s[], unsigned char b) {
    size_t len = write_digits(v, s, b);
    s[len] = '\0';
    sio_reverse(s, len);
    return len;
}
/* $end sioprivate */

/* Public Sio functions */
/* $begin siopublic */

/* Put string */
/**
 * @brief   Prints a C string to stdout.
 * @param s   String to print to stdout.
 * @return    The number of bytes written, or -1 on error.
 *
 * @remark   This function is async-signal-safe.
 */
static ssize_t sio_puts(const char s[]) {
    return write(STDOUT_FILENO, s, strlen(s)); //line:csapp:siostrlen
}

/**
 * @brief   Prints an error message, and exits uncleanly with status 1.
 * @param s   Error message to print to stdout.
 * @return    Does not return.
 *
 * @remark   This function is async-signal-safe.
 */
void sio_error(const char s[]) {
    sio_puts(s);
    sio_puts("\n");
    _exit(1);    //line:csapp:sioexit
}

/**
 * @brief   Prints formatted output to stdout.
 * @param fmt   The format string used to determine the output.
 * @param ...   The arguments for the format string.
 * @return      The number of bytes written, or -1 on error.
 *
 * @remark   This function is async-signal-safe.
 * @see      sio_vfprintf
 */
ssize_t sio_printf(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    ssize_t ret = sio_vfprintf(STDOUT_FILENO, fmt, argp);
    va_end(argp);
    return ret;
}

/**
 * @brief   Prints formatted output to a file descriptor.
 * @param fileno   The file descriptor to print output to.
 * @param fmt      The format string used to determine the output.
 * @param ...      The arguments for the format string.
 * @return         The number of bytes written, or -1 on error.
 *
 * @remark   This function is async-signal-safe.
 * @see      sio_vfprintf
 */
ssize_t sio_fprintf(int fileno, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    ssize_t ret = sio_vfprintf(fileno, fmt, argp);
    va_end(argp);
    return ret;
}

/**
 * @brief   Prints formatted output to a file descriptor from a va_list.
 * @param fileno   The file descriptor to print output to.
 * @param fmt      The format string used to determine the output.
 * @param argp     The arguments for the format string.
 * @return         The number of bytes written, or -1 on error.
 *
 * @remark   This function is async-signal-safe.
 *
 * This is a reentrant and async-signal-safe implementation of vfprintf, used
 * to implement the associated formatted sio functions.
 *
 * This function writes directly to a file descriptor (using the `rio_writen`
 * function from csapp), as opposed to a `FILE *` from the standard library.
 * However, since these writes are unbuffered, this is not very efficient, and
 * should only be used when async-signal-safety is necessary.
 *
 * The only supported format specifiers are the following:
 *  -  Int types: %d, %i, %u, %x, %o (with size specifiers l, z)
 *  -  Others: %c, %s, %%, %p
 */
ssize_t sio_vfprintf(int fileno, const char *fmt, va_list argp) {
    size_t pos = 0;
    ssize_t num_written = 0;

    while (fmt[pos] != '\0') {
        // String output of this iteration
        const char *str = NULL;  // String to output
        size_t len = 0;  // Length of string to output

        // Mark whether we've matched a format
        bool handled = false;

        // Int to string conversion
        char buf[128];
        char convert_type = '\0';
        union {
            uintmax_t u;
            intmax_t s;
        } convert_value = {.u = 0};

        // Handle format characters
        if (fmt[pos] == '%') {
            switch (fmt[pos + 1]) {

            // Character format
            case 'c':
                buf[0] = (char) va_arg(argp, int);
                buf[1] = '\0';
                str = buf;
                len = 1;
                handled = true;
                pos += 2;
                break;

            // String format
            case 's':
                str = va_arg(argp, char *);
                len = strlen(str);
                handled = true;
                pos += 2;
                break;

            // Escaped %
            case '%':
                str = &fmt[pos + 1];
                len = 1;
                handled = true;
                pos += 2;
                break;

            // Pointer type
            case 'p': {
                void *ptr = va_arg(argp, void *);
                if (ptr == NULL) {
                    str = "(nil)";
                    len = strlen(str);
                    handled = true;
                }
                else {
                    convert_type = 'p';
                    convert_value.u = (uintmax_t) (uintptr_t) ptr;
                }
                pos += 2;
                break;
            }

            // Int types with no format specifier
            case 'd':
            case 'i':
                convert_type = 'd';
                convert_value.s = (intmax_t) va_arg(argp, int);
                pos += 2;
                break;
            case 'u':
                convert_type = 'u';
                convert_value.u = (uintmax_t) va_arg(argp, unsigned);
                pos += 2;
                break;
            case 'x':
                convert_type = 'x';
                convert_value.u = (uintmax_t) va_arg(argp, unsigned);
                pos += 2;
                break;
            case 'o':
                convert_type = 'o';
                convert_value.u = (uintmax_t) va_arg(argp, unsigned);
                pos += 2;
                break;

            // Int types with size format: long
            case 'l': {
                switch (fmt[pos + 2]) {
                case 'd':
                case 'i':
                    convert_type = 'd';
                    convert_value.s =
                      (intmax_t) va_arg(argp, long);
                    pos += 3;
                    break;
                case 'u':
                    convert_type = 'u';
                    convert_value.u =
                      (uintmax_t) va_arg(argp, unsigned long);
                    pos += 3;
                    break;
                case 'x':
                    convert_type = 'x';
                    convert_value.u =
                      (uintmax_t) va_arg(argp, unsigned long);
                    pos += 3;
                    break;
                case 'o':
                    convert_type = 'o';
                    convert_value.u =
                      (uintmax_t) va_arg(argp, unsigned long);
                    pos += 3;
                    break;
                }
            }

            // Int types with size format: size_t
            case 'z': {
                switch (fmt[pos + 2]) {
                case 'd':
                case 'i':
                    convert_type = 'd';
                    convert_value.s = (intmax_t) va_arg(argp, ssize_t);
                    pos += 3;
                    break;
                case 'u':
                    convert_type = 'u';
                    convert_value.u = (uintmax_t) va_arg(argp, size_t);
                    pos += 3;
                    break;
                case 'x':
                    convert_type = 'x';
                    convert_value.u = (uintmax_t) va_arg(argp, size_t);
                    pos += 3;
                    break;
                case 'o':
                    convert_type = 'o';
                    convert_value.u = (uintmax_t) va_arg(argp, size_t);
                    pos += 3;
                    break;
                }
            }

            }

            // Convert int type to string
            switch (convert_type) {
            case 'd':
                str = buf;
                len = intmax_to_string(convert_value.s, buf, 10);
                handled = true;
                break;
            case 'u':
                str = buf;
                len = uintmax_to_string(convert_value.u, buf, 10);
                handled = true;
                break;
            case 'x':
                str = buf;
                len = uintmax_to_string(convert_value.u, buf, 16);
                handled = true;
                break;
            case 'o':
                str = buf;
                len = uintmax_to_string(convert_value.u, buf, 8);
                handled = true;
                break;
            case 'p':
                str = buf;
                strcpy(buf, "0x");
                len = uintmax_to_string(convert_value.u, buf + 2, 16) + 2;
                handled = true;
                break;
            }
        }

        // Didn't match a format above
        // Handle block of non-format characters
        if (!handled) {
            str = &fmt[pos];
            len = 1 + strcspn(&fmt[pos + 1], "%");
            pos += len;
        }

        // Write output
        if (len > 0) {
            ssize_t ret = rio_writen(fileno, (void *) str, len);
            if (ret < 0 || (size_t) ret != len) {
                return -1;
            }
            num_written += len;
        }
    }

    return num_written;
}

/* Async-signal-safe assertion support*/
void __sio_assert_fail(const char *assertion, const char *file,
                       unsigned int line, const char *function) {
    Sio_fprintf(STDERR_FILENO,
                "%s: %s:%u: %s: Assertion `%s' failed.\n",
                __progname, file, line, function, assertion);
    abort();
}
/* $end siopublic */

/*******************************
 * Wrappers for the SIO routines
 ******************************/

/**
 * @brief   Identical to sio_error().
 * @param s   Error message to print to stdout.
 * @return    Does not return.
 *
 * @remark   This function is async-signal-safe.
 * @see      sio_error
 */
void Sio_error(const char s[]) {
    sio_error(s);
}

/**
 * @brief   Wrapper for sio_printf(). Exits on failure.
 * @param fmt   The format string used to determine the output.
 * @param ...   The arguments for the format string.
 * @return      The number of bytes written.
 *
 * @remark   This function is async-signal-safe.
 * @see      sio_printf
 */
ssize_t Sio_printf(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    ssize_t ret = Sio_vfprintf(STDOUT_FILENO, fmt, argp);
    va_end(argp);
    return ret;
}

/**
 * @brief   Wrapper for sio_fprintf(). Exits on failure.
 * @param fileno   The file descriptor to print output to.
 * @param fmt      The format string used to determine the output.
 * @param ...      The arguments for the format string.
 * @return         The number of bytes written, or -1 on error.
 *
 * @remark   This function is async-signal-safe.
 * @see      sio_fprintf
 */

/* Sio_fprintf - wrapper for sio_fprintf */
ssize_t Sio_fprintf(int fileno, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    ssize_t ret = Sio_vfprintf(fileno, fmt, argp);
    va_end(argp);
    return ret;
}

/**
 * @brief   Wrapper for sio_vprintf(). Exits on failure.
 * @param fileno   The file descriptor to print output to.
 * @param fmt      The format string used to determine the output.
 * @param argp     The arguments for the format string.
 * @return         The number of bytes written, or -1 on error.
 *
 * @remark   This function is async-signal-safe.
 * @see      sio_vfprintf
 */
ssize_t Sio_vfprintf(int fileno, const char *fmt, va_list argp) {
    ssize_t ret = sio_vfprintf(fileno, fmt, argp);
    if (ret < 0) {
        Sio_error("Sio_vfprintf error");
    }
    return ret;
}

/********************************
 * Wrappers for Unix I/O routines
 ********************************/

/**
 * @brief   Wrapper for libc open(). Exits on failure.
 * @param pathname  Path to file.
 * @param flags     Access mode.
 * @param mode      File mode bits.
 *
 * @return  New file descriptor if successful.
 */
int Open(const char *pathname, int flags, mode_t mode) {
    int rc;

    if ((rc = open(pathname, flags, mode)) < 0) {
        unix_error("Open error");
    }

    return rc;
}

/**
 * @brief   Wrapper for libc read(). Exits on failure.
 * @param fd        File descriptor.
 * @param buf       Buffer to read into.
 * @param count     Number of bytes to read.
 *
 * @return  Number of bytes read.
 */
ssize_t Read(int fd, void *buf, size_t count) {
    ssize_t rc;

    if ((rc = read(fd, buf, count)) < 0) {
        unix_error("Read error");
    }

    return rc;
}

/**
 * @brief   Wrapper for libc write(). Exits on failure.
 * @param fd    File descriptor to write to.
 * @param buf   Buffer pointing to source data.
 * @param count Number of bytes to write.
 *
 * @return  Number of bytes written.
 */
ssize_t Write(int fd, const void *buf, size_t count) {
    ssize_t rc;

    if ((rc = write(fd, buf, count)) < 0) {
        unix_error("Write error");
    }

    return rc;
}

off_t Lseek(int fildes, off_t offset, int whence) {
    off_t rc;

    if ((rc = lseek(fildes, offset, whence)) < 0) {
        unix_error("Lseek error");
    }

    return rc;
}

/**
 * @brief   Close open file descriptor. Exits on failure.
 * @param fd    File descriptor.
 */
void Close(int fd) {
    int rc;

    if ((rc = close(fd)) < 0) {
        unix_error("Close error");
    }
}

int Select(int  n, fd_set *readfds, fd_set *writefds,
        fd_set *exceptfds, struct timeval *timeout) {
    int rc;

    if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0) {
        unix_error("Select error");
    }

    return rc;
}

int Dup2(int fd1, int fd2) {
    int rc;

    if ((rc = dup2(fd1, fd2)) < 0) {
        unix_error("Dup2 error");
    }

    return rc;
}

void Stat(const char *filename, struct stat *buf) {
    if (stat(filename, buf) < 0) {
        unix_error("Stat error");
    }
}

void Fstat(int fd, struct stat *buf) {
    if (fstat(fd, buf) < 0) {
        unix_error("Fstat error");
    }
}

/*********************************
 * Wrappers for directory function
 *********************************/

DIR *Opendir(const char *name) {
    DIR *dirp = opendir(name);

    if (!dirp) {
        unix_error("opendir error");
    }

    return dirp;
}

struct dirent *Readdir(DIR *dirp) {
    struct dirent *dep;

    errno = 0;
    dep = readdir(dirp);
    if ((dep == NULL) && (errno != 0)) {
        unix_error("readdir error");
    }

    return dep;
}

int Closedir(DIR *dirp) {
    int rc;

    if ((rc = closedir(dirp)) < 0) {
        unix_error("closedir error");
    }

    return rc;
}

/***************************************
 * Wrappers for memory mapping functions
 ***************************************/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) {
    void *ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) -1)) {
        unix_error("mmap error");
    }

    return ptr;
}

void Munmap(void *start, size_t length) {
    if (munmap(start, length) < 0) {
        unix_error("munmap error");
    }
}

/***************************************************
 * Wrappers for dynamic storage allocation functions
 ***************************************************/

void *Malloc(size_t size) {
    void *p;

    if ((p  = malloc(size)) == NULL) {
        unix_error("Malloc error");
    }

    return p;
}

void *Realloc(void *ptr, size_t size) {
    void *p;

    if ((p  = realloc(ptr, size)) == NULL) {
        unix_error("Realloc error");
    }

    return p;
}

void *Calloc(size_t nmemb, size_t size) {
    void *p;

    if ((p = calloc(nmemb, size)) == NULL) {
        unix_error("Calloc error");
    }

    return p;
}

void Free(void *ptr) {
    free(ptr);
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/
void Fclose(FILE *fp) {
    if (fclose(fp) != 0) {
        unix_error("Fclose error");
    }
}

FILE *Fdopen(int fd, const char *type) {
    FILE *fp;

    if ((fp = fdopen(fd, type)) == NULL) {
        unix_error("Fdopen error");
    }

    return fp;
}

char *Fgets(char *ptr, int n, FILE *stream) {
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream)) {
        app_error("Fgets error");
    }

    return rptr;
}

FILE *Fopen(const char *filename, const char *mode) {
    FILE *fp;

    if ((fp = fopen(filename, mode)) == NULL) {
        unix_error("Fopen error");
    }

    return fp;
}

void Fputs(const char *ptr, FILE *stream) {
    if (fputs(ptr, stream) == EOF) {
        unix_error("Fputs error");
    }
}

size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t n;

    if (((n = fread(ptr, size, nmemb, stream)) < nmemb) && ferror(stream)) {
        unix_error("Fread error");
    }

    return n;
}

void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    if (fwrite(ptr, size, nmemb, stream) < nmemb) {
        unix_error("Fwrite error");
    }
}


/****************************
 * Sockets interface wrappers
 ****************************/

int Socket(int domain, int type, int protocol) {
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0) {
        unix_error("Socket error");
    }

    return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen) {
    int rc;

    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0) {
        unix_error("Setsockopt error");
    }
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen) {
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0) {
        unix_error("Bind error");
    }
}

void Listen(int s, int backlog) {
    int rc;

    if ((rc = listen(s,  backlog)) < 0) {
        unix_error("Listen error");
    }
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) {
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0) {
        unix_error("Accept error");
    }

    return rc;
}

void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen) {
    int rc;

    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0) {
        unix_error("Connect error");
    }
}

/*******************************
 * Protocol-independent wrappers
 *******************************/
/* $begin getaddrinfo */
void Getaddrinfo(const char *node, const char *service,
        const struct addrinfo *hints, struct addrinfo **res) {
    int rc;

    if ((rc = getaddrinfo(node, service, hints, res)) != 0) {
        gai_error(rc, "Getaddrinfo error");
    }
}
/* $end getaddrinfo */

void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
        size_t hostlen, char *serv, size_t servlen, int flags) {
    int rc;

    if ((rc = getnameinfo(sa, salen, host, hostlen, serv,
                    servlen, flags)) != 0) {
        gai_error(rc, "Getnameinfo error");
    }
}

void Freeaddrinfo(struct addrinfo *res) {
    freeaddrinfo(res);
}

void Inet_ntop(int af, const void *src, char *dst, socklen_t size) {
    if (!inet_ntop(af, src, dst, size)) {
        unix_error("Inet_ntop error");
    }
}

void Inet_pton(int af, const char *src, void *dst) {
    int rc;

    rc = inet_pton(af, src, dst);
    if (rc == 0) {
        app_error("inet_pton error: invalid dotted-decimal address");
    } else if (rc < 0) {
        unix_error("Inet_pton error");
    }
}

/*******************************************
 * DNS interface wrappers.
 *
 * NOTE: These are obsolete because they are not thread safe. Use
 * getaddrinfo and getnameinfo instead
 ***********************************/

/* $begin gethostbyname */
struct hostent *Gethostbyname(const char *name) {
    struct hostent *p;

    if ((p = gethostbyname(name)) == NULL) {
        dns_error("Gethostbyname error");
    }

    return p;
}
/* $end gethostbyname */

struct hostent *Gethostbyaddr(const char *addr, int len, int type) {
    struct hostent *p;

    if ((p = gethostbyaddr(addr, len, type)) == NULL) {
        dns_error("Gethostbyaddr error");
    }

    return p;
}

/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
        void * (*routine)(void *), void *argp) {
    int rc;

    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0) {
        posix_error(rc, "Pthread_create error");
    }
}

void Pthread_cancel(pthread_t tid) {
    int rc;

    if ((rc = pthread_cancel(tid)) != 0) {
        posix_error(rc, "Pthread_cancel error");
    }
}

void Pthread_join(pthread_t tid, void **thread_return) {
    int rc;

    if ((rc = pthread_join(tid, thread_return)) != 0) {
        posix_error(rc, "Pthread_join error");
    }
}

/* $begin detach */
void Pthread_detach(pthread_t tid) {
    int rc;

    if ((rc = pthread_detach(tid)) != 0) {
        posix_error(rc, "Pthread_detach error");
    }
}
/* $end detach */

void Pthread_exit(void *retval) {
    pthread_exit(retval);
}

pthread_t Pthread_self(void) {
    return pthread_self();
}

void Pthread_once(pthread_once_t *once_control, void (*init_function)()) {
    pthread_once(once_control, init_function);
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/

void Sem_init(sem_t *sem, int pshared, unsigned int value) {
    if (sem_init(sem, pshared, value) < 0) {
        unix_error("Sem_init error");
    }
}

void P(sem_t *sem) {
    if (sem_wait(sem) < 0) {
        unix_error("P error");
    }
}

void V(sem_t *sem) {
    if (sem_post(sem) < 0) {
        unix_error("V error");
    }
}

/****************************************
 * The Rio package - Robust I/O functions
 ****************************************/

/*
 * rio_readn - Robustly read n bytes (unbuffered)
 */
/* $begin rio_readn */
ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno != EINTR) {
                return -1;  /* errno set by read() */
            }

            /* Interrupted by sig handler return, call read() again */
            nread = 0;
        } else if (nread == 0) {
            break;                  /* EOF */
        }
        nleft -= nread;
        bufp += nread;
    }
    return n - nleft;             /* Return >= 0 */
}
/* $end rio_readn */

/*
 * rio_writen - Robustly write n bytes (unbuffered)
 */
/* $begin rio_writen */
ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno != EINTR) {
                return -1;       /* errno set by write() */
            }

            /* Interrupted by sig handler return, call write() again */
            nwritten = 0;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}
/* $end rio_writen */


/*
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
/* $begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
    int cnt;

    while (rp->rio_cnt <= 0) {      /* Refill if buf is empty */
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) {
                return -1;          /* errno set by read() */
            }

            /* Interrupted by sig handler return, nothing to do */
        } else if (rp->rio_cnt == 0) {
            return 0;               /* EOF */
        } else {
            rp->rio_bufptr = rp->rio_buf;   /* Reset buffer ptr */
        }
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;
    if ((size_t) rp->rio_cnt < n) {
        cnt = rp->rio_cnt;
    }
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}
/* $end rio_read */

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
/* $begin rio_readinitb */
void rio_readinitb(rio_t *rp, int fd) {
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}
/* $end rio_readinitb */

/*
 * rio_readnb - Robustly read n bytes (buffered)
 */
/* $begin rio_readnb */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0) {
            return -1;          /* errno set by read() */
        } else if (nread == 0) {
            break;              /* EOF */
        }
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}
/* $end rio_readnb */

/*
 * rio_readlineb - Robustly read a text line (buffered)
 */
/* $begin rio_readlineb */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
    size_t n;
    int rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {
            if (n == 1) {
                return 0; /* EOF, no data read */
            } else {
                break;    /* EOF, some data was read */
            }
        } else {
            return -1;    /* Error */
        }
    }
    *bufp = 0;
    return n-1;
}
/* $end rio_readlineb */

/**********************************
 * Wrappers for robust I/O routines
 **********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) {
    ssize_t n;

    if ((n = rio_readn(fd, ptr, nbytes)) < 0) {
        unix_error("Rio_readn error");
    }
    return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) {
    if ((size_t) rio_writen(fd, usrbuf, n) != n) {
        unix_error("Rio_writen error");
    }
}

void Rio_readinitb(rio_t *rp, int fd) {
    rio_readinitb(rp, fd);
}

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0) {
        unix_error("Rio_readnb error");
    }
    return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0) {
        unix_error("Rio_readlineb error");
    }
    return rc;
}

/********************************
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - Open connection to server at <hostname, port> and
 *     return a socket descriptor ready for reading and writing. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns:
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_clientfd */
int open_clientfd(char *hostname, char *port) {
    int clientfd, rc;
    struct addrinfo hints, *listp, *p;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV;  /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG;  /* Recommended for connections */
    if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo failed (%s:%s): %s\n",
                hostname, port, gai_strerror(rc));
        return -2;
    }

    /* Walk the list for one that we can successfully connect to */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (clientfd < 0) {
            continue; /* Socket failed, try the next */
        }

        /* Connect to the server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
            break; /* Success */
        }

        /* Connect failed, try another */
        if (close(clientfd) < 0) { //line:netp:openclientfd:closefd
            fprintf(stderr, "open_clientfd: close failed: %s\n",
                    strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) {   /* All connects failed */
        return -1;
    } else {      /* The last connect succeeded */
        return clientfd;
    }
}
/* $end open_clientfd */

/*
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns:
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_listenfd */
int open_listenfd(char *port) {
    struct addrinfo hints, *listp, *p;
    int listenfd, rc, optval=1;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
    hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */
    if ((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo failed (port %s): %s\n",
                port, gai_strerror(rc));
        return -2;
    }

    /* Walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenfd < 0) {
            continue;  /* Socket failed, try the next */
        }

        /* Eliminates "Address already in use" error from bind */
        setsockopt(listenfd, SOL_SOCKET,    //line:netp:csapp:setsockopt
                SO_REUSEADDR, (const void *) &optval , sizeof(int));

        /* Bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
            break; /* Success */
        }

        if (close(listenfd) < 0) { /* Bind failed, try the next */
            fprintf(stderr, "open_listenfd close failed: %s\n",
                    strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) { /* No address worked */
        return -1;
    }

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
        return -1;
    }
    return listenfd;
}
/* $end open_listenfd */

/****************************************************
 * Wrappers for reentrant protocol-independent helpers
 ****************************************************/
int Open_clientfd(char *hostname, char *port) {
    int rc;

    if ((rc = open_clientfd(hostname, port)) < 0) {
        unix_error("Open_clientfd error");
    }
    return rc;
}

int Open_listenfd(char *port) {
    int rc;

    if ((rc = open_listenfd(port)) < 0) {
        unix_error("Open_listenfd error");
    }
    return rc;
}

/* $end csapp.c */

