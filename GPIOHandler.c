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
	/* Digital Pins */
	pinMode(LED_PIN, OUTPUT);
	pinMode(SPELL_START_BUTTON, INPUT);

	/* Analog Pins with PWM setup (range 0 - 100) */
	softPwmCreate(RUMBLER_PIN, 0, MAX_ANALOG_RANGE);

	/* TODO - add more setup code */
}

bool gameStartButtonPressed()
{
	if(digitalRead(GAME_START_BUTTON) == LOW)
		return true;

	return false;
}

bool castButtonPressed()
{
	if(digitalRead(SPELL_START_BUTTON) == HIGH)
	{
		printf("The game start button has been pressed");
		delay(2000);
		return true;
	}

	return false;
}

bool endCastButtonPressed()
{
	if(digitalRead(SPELL_END_BUTTON) == HIGH)
		return true;

	return false;
}

void changeRumbleMode(int level)
{
	printf("changing rumbleMode");

	softPwmWrite(RUMBLER_PIN, level);
}

void changeLEDMode(int mode)
{
	if(mode == 1)
		digitalWrite(LED_PIN, HIGH);
	else /* mode == 0 */
		digitalWrite(LED_PIN, LOW);
}
