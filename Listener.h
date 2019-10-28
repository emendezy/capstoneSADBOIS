/*
 * Listener.h - Header file for Listener.c
 */

#ifndef HEADER_GAURD_FOR_LISTENER
#define HEADER_GAURD_FOR_LISTENER

#include "HandlePlayerData.h"
#include "GPIOHandler.h"

bool gameIsActive = false;

void playTheGame();

void waitForGameToStart();

void sigchld_handler(int);

#endif
