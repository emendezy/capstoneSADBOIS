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
	"healing_ward", // 1
	"burning_brand", // 2
	"crackling_bolt", // 3
	"bastion", // 4
	"second_wind" // 5
	"infernal_pact" // 6
	"molten_bombardment" // 7
	"healing_surge" // 8
	"thundering_devastation" // 9
};

const char* const bookOfSounds[] =
{
	"sent_spell",
	"got_hit"
};

// -----------------------------------------------
// Initialization Steps
// -----------------------------------------------

struct PlayerStaffData* initPlayerStruct(bool* gameIsActive)
{
	setupPinsOnRaspberryPi(); /* From GPIOHandler */

	struct PlayerStaffData* P = malloc(sizeof(struct PlayerStaffData));
	P->gameInProgress = gameIsActive;
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
	bool castButtonPressed = false;
	// ^ TODO - fill with cast button input reading

	if (castButtonPressed)
	{
		return true;
	}
	else
		return false;
}

int wasAttacked(struct PlayerStaffData* P)
{
	bool recievedAttack = false; // TODO - fill with bluetooth/wifi response
								 // 	 from other staff
	int damageTaken = 0;
	if (recievedAttack)
	{
		/* TODO - Have the comm of attack be continously sent from other
		 * 		staff until picked up by this staff
		 *  	- Have this staff send back a response that the "gotAttacked"
		 *    	signal was recieved
		 *		- Grab damageTaken variable from staff comm and return
		 */
		// damage = readBluetoothInput
		return damageTaken;
	}
	else
		return -1;
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
	P->healthPercent -= damageTaken;
	if(P->healthPercent <= 0)
	{
		/* game over */
		bool* gameStatus = P->gameInProgress;
		(*gameStatus) = false; // global var - defined in Listener.h

	}
}

void spellCaster(struct PlayerStaffData* P, int damageTaken)
{
	if(damageTaken != -1)
	{
		/* need to stop spell casting and call attackHandler */

		attackHandler(P, damageTaken);
	}
	/* Run spell starting steps */
	else if((P->rumbleStartTime == 0 && P->lightStartTime == 0)
			&& damageTaken == -1)
	{
		P->isCasting = true;

		rumbleHandler(P, TURN_ON);
		lightHandler(P, TURN_ON);
	}
	/* Spell has been started and keep processing the IMU input */
	else
	{
		imuInputHandler(P);
	}

}

/* Pressure sensor was pressed end cast sequence
 */
void endCasting(struct PlayerStaffData* P, bool successfulCast)
{
	P->isCasting = false;

	rumbleHandler(P, TURN_OFF);
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
void rumbleHandler(struct PlayerStaffData* P, int rumbleMode)
{
	switch (rumbleMode)
	{
		case 0: /* Case for turning rumbler OFF */
			assert(P->isRumbling == true);
			P->isRumbling = false;
			P->rumbleLevel = 0;
			P->rumbleStartTime = 0;
			break;
		case 1: /* Case for turning rumbler ON */
			assert(P->isRumbling == false);
			P->isRumbling = true;
			P->rumbleStartTime = clock();
			P->rumbleLevel = 1;
			break;
		case 2: /* Case for increasing rumbler by 1 level */
			assert(P->isRumbling == true);
			assert(P->rumbleLevel < MAX_RUMBLE);
			P->rumbleLevel++;
			break;
		case 3: /* Case for decreasing the rumbler by 1 level */
			assert(P->isRumbling == true);
			assert(P->rumbleLevel > 1);
			P->rumbleLevel--;
			break;
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
			assert(P->isLit == true);
			P->isLit = false;
			P->lightLevel = 0;
			P->lightStartTime = 0;
			break;
		case 1: /* Case for turning LEDs ON */
			assert(P->isLit == false);
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
