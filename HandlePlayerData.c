/*
 * HandlePlayerData
 * -> based on the PlayerStaffData struct, create input and output handlers
 *
 */

#include "HandlePlayerData.h"

/* each index of the array equates to it's spell ID */
const char* const bookOfSpells[] =
{
	"no_spell_chosen", // 0 (default case)
	"water_defense", // 1
	"fireball", // 2
	"air_rush", // 3
	"rock_grind", // 4
	"splash" // 5
};

char* const bookOfSounds[] =
{
	"Sounds/Slurp.mp3", // 0 (default case)
	"Sounds/Baby.mp3", // 1
	"Sounds/Burp.mp3" // 2
};

// -----------------------------------------------
// Initialization Steps
// -----------------------------------------------

struct PlayerStaffData* initPlayerStruct(bool* isTheGameInProgress)
{
	setupPinsOnRaspberryPi(); /* From GPIOHandler */

	struct PlayerStaffData* P = malloc(sizeof(struct PlayerStaffData));
	P->gameInProgress = isTheGameInProgress;
	P->currentSpell = 0;
	P->isCasting = false;
	P->castDamage = 0;

	P->isRumbling = false;
	P->rumbleLevel = 0;
	P->rumbleStartTime = 0;

	P->isLit = false;
	P->lightLevel = 0;
	P->lightStartTime = 0;

	P->isLoud = false;
	P->keyOfSoundToPlay = -1;

	P->isShielding = false;
	P->shieldPercent = 100;

	P->healthPercent = 100;

	return P;
}

// -----------------------------------------------
// De-initialization Steps
// -----------------------------------------------

void unloadPlayerData(struct PlayerStaffData* P)
{
	/* TODO - add code that saves the Player staff data to file on SD card */
	free(P);
}

// -----------------------------------------------
// Input Handlers
// -----------------------------------------------

bool isCasting(struct PlayerStaffData* P)
{
	bool isPressed = castButtonPressed();

	if (isPressed || P->isCasting)
	{
		P->isCasting = true;
		return true;
	}
	else
		return false;
}

bool isDoneCasting(struct PlayerStaffData* P)
{
	bool isPressed, isSuccessful;
	isPressed = endCastButtonPressed();

	if(isPressed)
	{
		P->isCasting = false;
		isSuccessful = true;
		endCasting(P, isSuccessful);
		return true;
	}

	return false;
}

int wasAttacked(struct PlayerStaffData* P)
{
	bool recievedAttack = false; // TODO - fill with bluetooth/wifi response
								 // 	 from other staff
	int damageType = -1;
	if (recievedAttack)
	{
		/* TODO - Have the comm of attack be continously sent from other
		 * 		staff until picked up by this staff
		 *  	- Have this staff send back a response that the "gotAttacked"
		 *    	signal was recieved
		 *		- Grab damageType variable from staff comm and return
		 */
		return damageType;
	}
	return damageType;
}

void imuInputHandler(struct PlayerStaffData* P)
{
	/* Need to fill up with data reciever from the IMU handler module
	 * - Update PlayerStaffData with recieved IMU data
	 *   - Updating the progress of the current spell
	 *	 - i.e. if spell counter is incr, which spell was drawn, variations
	 * TODO - add sections that represent a managed variable that tracks the
	 * 		current spell being drawn
	 */
	return;
}

void attackHandler(struct PlayerStaffData* P, int damageTaken)
{
	/* TODO - need to check if the spell being currently cast is a
	 * defense spell
	 */
	bool successfulCast;
	bool* gameStatus;

	successfulCast = false;
	endCasting(P, successfulCast);

	/* Health decrement and check */
	P->healthPercent -= damageTaken;
	if(P->healthPercent <= 0)
	{
		/* game over */
		gameStatus = P->gameInProgress;
		(*gameStatus) = false; // global var - defined in Listener.h

	}
}

void spellCaster(struct PlayerStaffData* P, int damageType)
{
	printf("In spellCaster\n");

	int level;

	if(damageType > -1)
	{
		/* need to STOP SPELL CASTING and call attackHandler */
		attackHandler(P, damageType);
	}
	/* Run spell starting steps */
	else /* damageType == -1 */
	{
		if(P->isCasting == false)
		{
			/* This is the FIRST time we've started the spell Casting seq
			 * Run initialization steps
			 */
			P->isCasting = true;

			level = MAX_ANALOG_RANGE / 2;
			if(!(P->isRumbling))
			{
				rumbleHandler(P, TURN_ON, level);
				// lightHandler(P, TURN_ON);
			}
		}

		/* Spell has been started and now process the IMU input */
		imuInputHandler(P);
	}
}

/* Pressure sensor was pressed now initate end cast sequence
 */
void endCasting(struct PlayerStaffData* P, bool successfulCast)
{
	P->isCasting = false;

	rumbleHandler(P, TURN_OFF, 0);
	lightHandler(P, TURN_OFF);

	if(successfulCast)
	{
		sendCast(P);
	}
}

void sendCast(struct PlayerStaffData* P)
{
	/* TODO - setup bluetooth communication
	 *	- cast damage should be in the player struct (P->castDamage)
	 */
}

// -----------------------------------------------
// Output Handlers
// -----------------------------------------------

/* Rumbling handler
 *  Handle Rumbling sequences based on Player data
 *	isCasting - rumble at a low frequency to provide haptic feedback
 */
void rumbleHandler(struct PlayerStaffData* P, int rumbleMode, int level)
{
	switch (rumbleMode)
	{
		case 0: /* Case for turning rumbler OFF */
			P->isRumbling = false;
			P->rumbleLevel = 0;
			P->rumbleStartTime = 0;
			changeRumbleMode(TURN_OFF);
			break;
		case 1: /* Case for turning rumbler ON */
			P->isRumbling = true;
			P->rumbleStartTime = clock();
			P->rumbleLevel = 1;
			changeRumbleMode(level);
			break;
		/* Utilize this code later for fine tuning rumbling */
		// case 2: /* Case for increasing rumbler by 1 level step */
		// 	assert(P->isRumbling == true);
		// 	assert(P->rumbleLevel < (MAX_ANALOG_RANGE * 9)/ 10);
		// 	P->rumbleLevel += ANALOG_STEP;
		// 	break;
		// case 3: /* Case for decreasing the rumbler by 1 level step */
		// 	assert(P->isRumbling == true);
		// 	assert(P->rumbleLevel > ANALOG_STEP);
		// 	P->rumbleLevel -= ANALOG_STEP;
		// 	break;
	}
}

/* Lights handler
 *  Handle light sequences based on Player data
 *	isCasting - be lit at a low level to provide haptic feedback
 *				< End goal -> make light pulse slowly >
 */
void lightHandler(struct PlayerStaffData* P, int lightMode)
{
	switch (lightMode)
	{
		case 0: /* Case for turning LEDs OFF */
			P->isLit = false;
			P->lightLevel = 0;
			P->lightStartTime = 0;
			break;
		case 1: /* Case for turning LEDs ON */
			P->isLit = true;
			P->lightStartTime = clock();
			P->lightLevel = 1;
			break;
		case 2: /* Case for increasing LEDs by 1 level */
			assert(P->isLit == true);
			assert(P->lightLevel < MAX_LIGHT);
			P->lightLevel++;
			break;
		case 3: /* Case for decreasing the LEDs by 1 level */
			assert(P->isLit == true);
			assert(P->lightLevel > 1);
			P->lightLevel--;
			break;
	}
}

/* Sounds handler
 *  Handle sound playback based on player data and given sound type
 *  Sound type (int) is the index of the desired sound in the bookOfSounds[]
 */
void soundHandler(struct PlayerStaffData* P, int soundType)
{
	pid_t pid;
	if((pid = fork()) == 0)
	{
		setpgid(0, 0);
		sigset_t prev_mask = P->prev_mask;
		sigprocmask(SIG_SETMASK, &prev_mask, NULL);
		char* omxplayer = "/usr/bin/omxplayer";

		if (execve(omxplayer, &bookOfSounds[soundType], NULL) < 0) {
			printf("%s: ERROR playing sound\n", omxplayer);
			exit(1);
		}
	}
}
