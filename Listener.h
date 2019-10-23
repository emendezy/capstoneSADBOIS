/*
 * Listener.h - Header file for Listener.c
 */

#ifndef HEADER_GAURD_FOR_LISTENER
#define HEADER_GAURD_FOR_LISTENER

#include <stdbool.h>
#include "HandlePlayerData.h"
#include "GPIOHandler.h"

extern bool gameIsActive;

void playTheGame();

void waitForGameToStart();

#endif
