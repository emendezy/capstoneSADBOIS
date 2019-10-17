/*
 * Code for dealing with outputs on raspberry pi
 */

#include "GPIOHandler.h"


void initializeRaspberryPi()
{
	wiringPiSetup();
	wiringPiSetupGpio();
}

void setupPinsOnRaspberryPi()
{
	pinMode(LED_PIN, OUTPUT);
	pinMode(RUMBLER_PIN, OUTPUT);
	/* TODO - add more setup code */
}

void changeRumbleMode(int mode)
{
	if(mode == 1)
		digitalWrite(RUMBLER_PIN, HIGH);
	else /* mode == 0 */
		digitalWrite(RUMBLER_PIN, LOW);
}

void changeLEDMode(int mode)
{
	if(mode == 1)
		digitalWrite(LED_PIN, HIGH);
	else /* mode == 0 */
		digitalWrite(LED_PIN, LOW);
}
