/*
 * Code for dealing with outputs on raspberry pi
 */

#include "GPIOHandler.h"

extern int soundRet;

const char* const bookOfSounds[] =
{
	"Sounds/Slurp.mp3", // 0 (default case)
	"Sounds/Baby.mp3", // 1
	"Sounds/Burp.mp3" // 2
};

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
	printf("changing rumbleMode");
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

void* playSound(void* ptrToSoundIndex)
{
	/* soundIndex - represents the index in the array of available sounds */
	int* correctCast = (int*)ptrToSoundIndex;
	int soundIndex = *correctCast;
	char* sound = strcat("/usr/bin/omxplayer ", bookOfSounds[soundIndex]);

	system(sound);
	printf("done playing");
	soundRet = 100;
	pthread_exit(&soundRet);
}
