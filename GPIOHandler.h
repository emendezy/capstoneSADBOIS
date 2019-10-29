/*
 * GPIO Ouput functions
 * 		for communicating directly with the raspberry pi
 */

#ifndef HEADER_GAURD_FOR_GPIO_HANDLER
#define HEADER_GAURD_FOR_GPIO_HANDLER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <softPwm.h>

#define GAME_START_BUTTON 0

#define LED_PIN 1
#define SPELL_START_BUTTON 8
#define SPELL_END_BUTTON 3

#define RUMBLER_PIN 4
#define MAX_ANALOG_RANGE 100
#define ANALOG_STEP 5

void initializeRaspberryPi();

void setupPinsOnRaspberryPi();

bool gameStartButtonPressed();

bool castButtonPressed();

bool endCastButtonPressed();

void changeRumbleMode(int);

void changeLEDMode(int);

#endif
