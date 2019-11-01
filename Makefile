CC=gcc
CFLAGS= -O3 -Wall -g
LIBS=
AR=ar

ALLBIN=getbno055

all: ${ALLBIN}

clean:
	rm -f *.o ${ALLBIN}

listener: tsh_exec.c csapp.c tsh_helper.c GPIOHandler.h GPIOHandler.c HandlePlayerData.h HandlePlayerData.c Listener.h Listener.c
	gcc tsh_exec.c csapp.c tsh_helper.c GPIOHandler.h GPIOHandler.c HandlePlayerData.h HandlePlayerData.c Listener.h Listener.c -lwiringPi -pthread
	gcc ar cr listener.a tsh_exec.o csapp.o tsh_helper.o GPIOHandler.o HandlePlayerData.o Listener.o
	gcc listener.a -o listener


getbno055: i2c_bno055.o getbno055.o
	$(CC) i2c_bno055.o getbno055.o -o getbno055 ${LIBS}

