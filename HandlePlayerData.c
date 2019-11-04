/*
 * HandlePlayerData
 * -> based on the PlayerStaffData struct, create input and output handlers
 *
 */

#include "HandlePlayerData.h"

/* each index of the array equates to it's spell ID */

const char* const bookOfSpells[] =
{
	"healing_ward", // 0
	"burning_brand", // 1
	"crackling_bolt", // 2
	"bastion", // 3
	"second_wind" // 4
	"infernal_pact" // 5
	"molten_bombardment" // 6
	"healing_surge" // 7
	"thundering_devastation" // 8

};

const char* bookOfSounds[] =
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
	P->activeSpells = calloc(TOTAL_SPELLS_IN_SPELLBOOK, sizeof(int));
	P->isCasting = false;
	P->startOfSpell = false;
	P->castDamage = 0;

	P->spellQueue = malloc(sizeof(struct spellQueueStruct));
	// initQueue();
	P->hasBastion = 0;
	P->hasImmunity = false;
	P->immunityStart = (clock_t) 0;
	P->immunityTime = 0;

	P->isBurning = false;
	P->burnStart = (clock_t) 0;
	P->burnPerSecond = 0;
	P->burnTotalTime = 0;

	P->isWeakened = false;
	P->weaknessPercent = 0;
	P->weaknessTime = 0;
	P->weaknessStart = (clock_t) 0;

	P->coolDownMask = calloc(TOTAL_SPELLS_IN_SPELLBOOK, sizeof(int));

	P->damageValues = calloc(NUM_DAMAGE_VALUES, sizeof(int));

	P->isRumbling = false;
	P->rumbleLevel = 0;
	P->rumbleStartTime = 0;

	P->isLit = false;
	P->lightLevel = 0;
	P->lightStartTime = 0;

	P->isLoud = false;
	P->keyOfSoundToPlay = -1;

	P->isShielding = false;
	P->shieldPercent = 0;
	P->shieldTime = 0;
	P->shieldStart = (clock_t) 0;

	P->healthPercent = MAX_HEALTH;

	P->frontOfOneSecond = clock();
	P->backOfOneSecond = clock();

	return P;
}

// -----------------------------------------------
// De-initialization Steps
// -----------------------------------------------

void unloadPlayerData(struct PlayerStaffData* P)
{
	/* TODO - add code that saves the Player staff data to file on SD card */
	free(P->activeSpells);
	free(P->damageValues);
	free(P->coolDownMask);
	free(P);
}

// -----------------------------------------------
// Input Handlers
// -----------------------------------------------

bool isCurrCasting(struct PlayerStaffData* P)
{
	bool isPressed = castButtonPressed();
	if(!(P->isCasting) && isPressed)
	{
		P->startOfSpell = true;
	}

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
		for (int i = 0; i < TOTAL_SPELLS_IN_SPELLBOOK; i++) {
			P->activeSpells[i] = 0;
		}
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
	// short spellType = dequeueSpell();
	// if(spellType != -1) {
	// 	// Spell in queue that was successfully dequeued
	// 	printf("Spell was properly dequeued - %d\n", spellType);
	// 	P->activeSpells[spellType]++;
	// }
	P->activeSpells[0] = 0;
	P->activeSpells[1] = 0;
	P->activeSpells[2] = 5;
	P->activeSpells[3] = 0;
	P->activeSpells[4] = 0;
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
	int level;
	P->backOfOneSecond = clock();

	bool stopCasting = isDoneCasting(P); /* handle send spell logic */

	if(damageType > -1)
	{
		/* need to STOP SPELL CASTING and call attackHandler */
		attackHandler(P, damageType);
	}
	/* Run spell starting steps */
	else if(damageType == -1 && !stopCasting) /* damageType == -1 */
	{
		if(P->startOfSpell == true)
		{
			printf("first time in spellCaster\n");
			/* This is the FIRST time we've started the spell Casting seq
			 * Run initialization steps
			 */
			level = MAX_ANALOG_RANGE / 2;
			if(!(P->isRumbling))
			{
				rumbleHandler(P, TURN_ON, level);
				// lightHandler(P, TURN_ON);
			}
			P->startOfSpell = false;
		}

		/* Spell has been started and now process the IMU input */
		imuInputHandler(P);
	}

	/* Clean up code - dealing with timing gaurds */
	double oneSec = 1;
	if(oneSec <= ((double)(P->backOfOneSecond - P->frontOfOneSecond)) / CLOCKS_PER_SEC)
	{
		printf("1 second mark\n");
		editCoolDownValues(P, 1);
		checkWeakness(P);
		checkShield(P);
		checkImmunity(P);

		if(P->isBurning)
		{
			handleBurning(P);
		}
		P->frontOfOneSecond = clock();
		P->backOfOneSecond = clock();
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

void handleBurning(struct PlayerStaffData* P)
{
	if(P->burnTotalTime == 0)
	{
		P->isBurning = false;
		P->burnPerSecond = 0;
	}
	else
	{
		P->healthPercent -= P->burnPerSecond;
		P->burnTotalTime--;
	}
}

void editCoolDownValues(struct PlayerStaffData* P, int amount)
{
	for (int i = 0; i < TOTAL_SPELLS_IN_SPELLBOOK; i++)
	{
		if(P->coolDownMask[i] > 0)
		{
			P->coolDownMask[i] -= amount;
		}
	}
}

void checkWeakness(struct PlayerStaffData* P)
{
	if(P->isWeakened)
	{
		clock_t currentTime = clock();
		clock_t weaknessTime = (clock_t)(P->weaknessTime);
		if(P->weaknessStart + weaknessTime < currentTime)
		{
			P->isWeakened = false;
			P->weaknessPercent = 0;
		}
	}
}

void checkShield(struct PlayerStaffData* P)
{
	if(P->isShielding)
	{
		clock_t currentTime = clock();
		clock_t shieldTime = (clock_t)(P->shieldTime);
		if(P->shieldStart + shieldTime < currentTime)
		{
			P->isShielding = false;
			P->shieldPercent = 0;
		}
	}
}

void checkImmunity(struct PlayerStaffData* P)
{
	if(P->hasImmunity)
	{
		clock_t currentTime = clock();
		clock_t immuneTime = (clock_t)(P->immunityTime);
		if(P->immunityStart + immuneTime < currentTime)
		{
			P->hasImmunity = false;
		}
	}
}

/*
	"healing_ward", // 0
	"burning_brand", // 1
	"crackling_bolt", // 2
	"bastion", // 3
	"second_wind" // 4
*/

void sendCast(struct PlayerStaffData* P)
{
	// -> processDamageRecieved() will do the damage application after being hit with a damage payload
	printf("Sending a cast! ----\n");

	int maxSpell, timesCast;
	maxSpell = 0;
	for(int i = 0; i < TOTAL_SPELLS_IN_SPELLBOOK; i++)
	{
		printf("%d\n", P->activeSpells[i]);
		if(P->activeSpells[i] > maxSpell) maxSpell = i;
	}

	timesCast = P->activeSpells[maxSpell];

	if(P->coolDownMask[maxSpell] == 0)
	{
		// classify the spell type
		switch(maxSpell){
			case 0:
				printf("healing_ward!\n");
				// reduce damge (10 + timesCast)
				P->isShielding = true;
				P->shieldPercent = 50;
				P->shieldTime = 10 + timesCast;
				P->shieldStart = clock();

				// restore 5 + (5*timesCast)
				P->healthPercent += 5 + (5 * timesCast);

				// NOTE - no need to send a P->damageValues package
				break;
			case 1:
				printf("burning_brand!\n");
				P->damageValues[0] = calcSendingSpellDamage(P, 10); // damage
				P->damageValues[1] = -1; // no cooldown affect
				P->damageValues[2] = 1; // burned affect is true
				P->damageValues[3] = 5 + (5 * timesCast); // damage over time
				P->damageValues[4] = 10; // damage time duration in seconds
				P->damageValues[5] = 50; // weakness percent
				P->damageValues[6] = 5 + timesCast; // weakness time duration in seconds

				sendDamagePackage(P->damageValues);
				break;
			case 2:
				printf("crackling bolt!\n");
				// deal damage instantly
				P->damageValues[0] = calcSendingSpellDamage(P, 20 + (3 * timesCast));
				P->damageValues[1] = 1 + timesCast; // add a cooldown time of 1 + 1*lightning seconds
				P->damageValues[2] = 0; // no burn affect
				P->damageValues[3] = 0; // ""
				P->damageValues[4] = 0; // ""
				P->damageValues[5] = 0; // no weakening affect
				P->damageValues[6] = 0; // ""

				sendDamagePackage(P->damageValues);
				break;
			case 3:
				printf("bastion!\n");
				P->hasBastion += 1;
				P->hasImmunity = true;
				P->immunityStart = clock();
				P->immunityTime = 4 + timesCast;

				// NOTE - no need to send a P->damageValues package
				break;
			case 4:
				printf("second_wind!\n");
				editCoolDownValues(P, 5 + timesCast);

				// NOTE - no need to send a P->damageValues package
				break;
		}
	}

	/* Fake test code
		run this spell on the user's self for now
	*/
	processDamageRecieved(P, P->damageValues);
}

int calcSendingSpellDamage(struct PlayerStaffData* P, int damage)
{
	if (P->isWeakened)
	{
		return (MAX_WEAKNESS - P->weaknessPercent) * damage / MAX_WEAKNESS;
	}
	return damage;
}

void sendDamagePackage(int* damageValues)
{
	/* TODO - setup NRF24L01 chip communication */
}

int calcTotalDamage(int shieldPercent, int dmg)
{
	return dmg - (shieldPercent * dmg / MAX_SHIELD);
}

/* damageValues array key
 * 0 - overall damage delt
 * 1 - reciever's current spells being cast are put on cooldown for this amount of time (is -1 if not applicapable)
 * 2 - burned affect (1 - active, 0 - inactive)
 * 3 - if burned is 1 -> burn damage
 * 4 - if burned is 1 -> burn time
 * 5 - weakness level (0 - 100) % future damage reduction for player getting hit
 * 6 - weakness time (5 + 1triangle sec) how long the weakness level lasts
 */
void processDamageRecieved(struct PlayerStaffData* P, int* damageValues)
{
	printf("Under Attack! :o\n");

	// check for shield ability
	if(!(P->hasBastion) && !(P->hasImmunity))
	{
		printf("-> damage processing logic\n");

		// 0 - overall damage delt
		P->healthPercent -= calcTotalDamage(P->shieldPercent,damageValues[0]);

		// 1 - spell put on cooldown (index of spell in activeSpells)
		for(int s = 0; s < TOTAL_SPELLS_IN_SPELLBOOK; s++)
		{
			if(P->activeSpells[s] > 0)
			{
				// set cooldown time
				P->coolDownMask[s] = damageValues[1];
			}
		}

		// 2 (active/inactive) -> 3,4 burned affect : set burn damage and time
		if(damageValues[2])
		{
			// burned affect == 1 (active)
			P->isBurning = true;
			P->burnStart = clock();
			P->burnPerSecond = damageValues[3] / damageValues[4];
			P->burnTotalTime = damageValues[4];
		}
		if(damageValues[5] > 0)
		{
			P->isWeakened = true;
			P->weaknessPercent = damageValues[5];
			P->weaknessTime = damageValues[6];
			P->weaknessStart = clock();
		}
	}
	else
	{
		// can be a value of 0 - use up one bastion shield
		P->hasBastion--;
	}
	printf("Player health = %d\n", P->healthPercent);
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
	char* omxplayerLocation = "/usr/bin/omxplayer";

	char* omxplayer = malloc(sizeof("omxplayer -o local ") + sizeof(bookOfSounds[soundType]));
	strcpy(omxplayer, "omxplayer -o local ");

	char* file = malloc(sizeof(bookOfSounds[soundType]));
	strcpy(file, bookOfSounds[soundType]);
	omxplayer = strcat(omxplayer, file);
	if((pid = fork()) == 0)
	{
		setpgid(0, 0);
		sigset_t prev_mask = P->prev_mask;
		sigprocmask(SIG_SETMASK, &prev_mask, NULL);

		if (execve(omxplayerLocation, &omxplayer, NULL) < 0) {
			printf("%s: ERROR playing sound\n", omxplayerLocation);
			exit(1);
		}
	}
	free(omxplayer);
}
