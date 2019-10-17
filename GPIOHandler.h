/*
 * GPIO Ouput functions
 * 		for communicating directly with the raspberry pi
 */

#ifndef HEADER_GAURD_FOR_GPIO_HANDLER
#define HEADER_GAURD_FOR_GPIO_HANDLER

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define LED_PIN 0
#define RUMBLER_PIN 1

void setupPinsOnRaspberryPi();


#endif
