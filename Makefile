OBJS	= getbno055.o i2c_bno055.o imumodule.o GPIOHandler.o HandlePlayerData.o Listener.o
SOURCE	= getbno055.c i2c_bno055.c imumodule.c GPIOHandler.c HandlePlayerData.c Listener.c
HEADER	= getbno055.h spellQueueStruct.h playerstruct.h HandlePlayerDataFunctions.h imumodule.h GPIOHandler.h HandlePlayerData.h Listener.h
OUT	= gametest
CC	 = gcc
FLAGS	 = -g -c
LDLIBS = -lwiringPi

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LDLIBS)

getbno055.o: getbno055.c
	$(CC) $(FLAGS) getbno055.c

i2c_bno055.o: i2c_bno055.c
	$(CC) $(FLAGS) i2c_bno055.c

imumodule.o: imumodule.c
	$(CC) $(FLAGS) imumodule.c

GPIOHandler.o: GPIOHandler.c
	$(CC) $(FLAGS) GPIOHandler.c

HandlePlayerData.o: HandlePlayerData.c
	$(CC) $(FLAGS) HandlePlayerData.c

Listener.o: Listener.c
	$(CC) $(FLAGS) Listener.c


clean:
	rm -f $(OBJS) $(OUT)

#CC=gcc
#CFLAGS= -O3 -Wall -g
#LIBS=
#AR=ar

#ALLBIN=getbno055

#all: ${ALLBIN}

#clean:
	#rm -f *.o ${ALLBIN}

#listener: tsh_exec.h tsh_exec.c csapp.h csapp.c tsh_helper.h tsh_helper.c getbno055.h getbno055.c i2c_bno055.c spellQueueStruct.h playerstruct.h imumodule.h imumodule.c GPIOHandler.h GPIOHandler.c HandlePlayerData.h HandlePlayerData.c Listener.h Listener.c
	#gcc tsh_exec.h tsh_exec.c csapp.h csapp.c tsh_helper.h tsh_helper.c getbno055.h getbno055.c i2c_bno055.c spellQueueStruct.h playerstruct.h imumodule.h imumodule.c GPIOHandler.h GPIOHandler.c HandlePlayerData.h HandlePlayerData.c Listener.h Listener.c -lwiringPi -pthread -ggdb -Wall
	#gcc ar cr listener.a tsh_exec.o csapp.o tsh_helper.o GPIOHandler.o HandlePlayerData.o Listener.o
	#gcc listener.a -o listener


#getbno055: i2c_bno055.o getbno055.o
#	$(CC) i2c_bno055.o getbno055.o -o getbno055 ${LIBS}

