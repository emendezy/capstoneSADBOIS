/*
 * Code for dealing with outputs on raspberry pi
 */

#include "GPIOHandler.h"

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

void playSound(int soundIndex)
{
	/* soundIndex - represents the index in the array of available sounds */
	pid_t pid;
	int status;

	pid = fork();
	if(pid == 0)
	{
		printf("I am the child\n");
		execlp("/usr/bin/omxplayer", " ", bookOfSounds[soundIndex], " --no-keys", NULL);
		printf("done playing");
		_exit(0);
	} else
	{
		printf("I am the parent\n");
		while(wait(&status) != pid)
		{
			printf("...\n");
		}
		printf("it exited\n");
		system("killall omxplayer.bin");
	}
}
