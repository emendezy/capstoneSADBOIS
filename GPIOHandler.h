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

#define GAME_START_BUTTON 22
#define SPELL_START_BUTTON 23
#define SPELL_END_BUTTON 4
#define RUMBLER_PIN 17


#define MAX_ANALOG_RANGE 100
#define ANALOG_STEP 5
#define LED_PIN 15

void initializeRaspberryPi();

void setupPinsOnRaspberryPi();

bool gameStartButtonPressed();

bool castButtonPressed();

bool endCastButtonPressed();

void changeRumbleMode(int);

void changeLEDMode(int);

#endif
