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
	pinMode(GAME_START_BUTTON, INPUT);
	// pullUpDnControl (GAME_START_BUTTON, PUD_UP);
	pinMode(SPELL_START_BUTTON, INPUT);
	// pullUpDnControl (SPELL_START_BUTTON, PUD_UP);
	pinMode(SPELL_END_BUTTON, INPUT);
	// pullUpDnControl (SPELL_END_BUTTON, PUD_UP);

	pinMode(RUMBLER_PIN, OUTPUT);
	// Output - deliver pulses to user as haptic feedback;

	/* Analog Pins with PWM setup (range 0 - 100) */
	// softPwmCreate(RUMBLER_PIN, 0, MAX_ANALOG_RANGE);
}

bool gameStartButtonPressed()
{
	if(digitalRead(GAME_START_BUTTON) == LOW)
		return true;

	return false;
}

bool castButtonPressed()
{
	if(digitalRead(SPELL_START_BUTTON) == LOW)
	{
		printf("The cast start button has been pressed");
		delay(1000);
		return true;
	}

	return false;
}

bool endCastButtonPressed()
{
	if(digitalRead(SPELL_END_BUTTON) == LOW)
	{
		printf("Spell end button pressed\n");
		delay(1000);
		return true;
	}

	return false;
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
