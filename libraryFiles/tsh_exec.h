#ifndef __TSH_EXEC_H__
#define __TSH_EXEC_H__

/*
 * tsh_exec.h: definitions and interfaces for tshlab exec functionality
 *
 * This file defines enumerations and structs used in tshlab exec functionality,
 * as well as the interfaces for various support functions routines.
 * You should use these routines to help write your tiny shell exec support.
 *
 * Please notice that these functions are NOT async-signal-safe. Be careful
 * when using these functions.
 *
 */

#include <unistd.h>
#include <stdbool.h>

/* Error Codes for map, unmap and launch */
#define MAP_ERROR_EXPECT_EXACTLY_1 1
#define MAP_ERROR_NO_EXTENSION 2
#define MAP_ERROR_EXTENSION_NOT_FOUND 3
#define MAP_ERROR_NEEDS_0_OR_2 4
#define MAP_ERROR_ALREADY_MAPPED 5
#define MAP_ERROR_UNMAP_NEEDS_EXT  6
#define MAP_ERROR_UNMAP_NO_EXT 7
#define MAP_ERROR_NEED_EXT_TO_SET 8
#define MAP_ERROR_EOM 9
#define MAP_ERROR_NEED_ENTRY 10
#define MAP_ERROR_NO_ENTRY_TO_DEL 11

/* Entry of the filename to arguments mapping */
struct exec_entry
{
  char* extension;	    // Extension of the extry
  int argc;             // Number of arguments without placeholder.
  char **argv;		      // The arguments list
};

typedef struct exec_entry ExecEntry;

/*
 * Looks up a file extension in the table and returns a pointer
 * to a matching entry if any, otherwise return NULL
 *
 * extension: The file extension to be searched
 */
struct exec_entry *get_entry(const char *extension);

/*
 * Create an entry for an extension (which cannot be empty string),
 * prints out error message (using maperror) if there is an error
 *
 * extension: the file extension to be set
 * argc: Number of arguments mapping to
 * argv: The list of arguments mapping to
 */
int
set_exec_entry(const char *extension, int argc, char** argv);

/*
 * Frees up and removes an entry (and unmaps its extension),
 * returns -1 on error, 0 on success.
 *
 * entry: Pointer to the entry that is going to be destroyed
 */
int
destroy_entry(struct exec_entry* entry);

/*
 * Prints out all current mappings
 *
 * fh: File descriptor that the output is directed to
 */
void show_maps(int fh);

/* Prints out error messages for map commands
 *
 * fh: File descriptor that the output is directed to
 * code: Error code defined above
 * possibleExtension: The possible file extension that lead
 *                    to the error
 */
void maperror(int fh, int code, char* possibleExtension);

#endif // __TSH_EXEC_H__
