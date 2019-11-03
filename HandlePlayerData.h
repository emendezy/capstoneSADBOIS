/*
 * Struct with player data,
 * Dictionaries with spell types and sounds to play
 */

#ifndef HEADER_GAURD_FOR_PLAYER_DATA
#define HEADER_GAURD_FOR_PLAYER_DATA

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>

#include <pthread.h>

#include <assert.h>
#include <time.h>

#include "GPIOHandler.h"
#include "imumodule.h"

#include "tsh_exec.h"
#include "csapp.h"
#include "tsh_helper.h"

// #define NDEBUG /* Comment for no debug mode */
#define TURN_OFF 0
#define TURN_ON 1
#define INCR 2
#define DECR 3

// EARTH = 0;
// FIRE = 1;
// LIGHTNING = 2;
// WATER = 3;
// WIND = 4;

#define MAX_RUMBLE 10
#define MAX_LIGHT 10

/* number of spells in our pre-defined book */
#define TOTAL_SPELLS_IN_SPELLBOOK 5
/* number of sounds in out pre-defined book */
#define TOTAL_NUM_OF_SOUNDS 2

#define NUM_DAMAGE_VALUES 5
/* ------------------------------------------------------------------------- */

struct PlayerStaffData
{
	bool* gameInProgress;
	int* activeSpells; // from book of spells
	bool isCasting;
	int castDamage; // damage of spell created by this user

	struct spellQueueStruct *spellQueue;
	int hasBastion;

	bool isRumbling;
	int rumbleLevel; /* 0 -> 10 */
	clock_t rumbleStartTime;

	bool isLit;
	int lightLevel; /* 0 -> 10 */
	clock_t lightStartTime;

	bool isLoud;
	int keyOfSoundToPlay; /* if none -> -1, else -> key for soundBook */

	bool isShielding;
	int shieldPercent; /* 0(empty) -> 100(full) */

	int healthPercent; /* 0(dead) -> 100(full) */

	sigset_t prev_mask;// for forking (masks for block/unblock)
};

/*
 * when the player casts a self-buff, or when the player is hit by an enemy spell, use this:
 */
struct playerSpellEffects
{
	//ward status
	bool ward = false;
	float wardReduction = 0.5;
	int wardTimeLeft = 0; //X milliseconds

	//heal over time status
	bool healOverTime = false;
	int healthPerTick; //hp restored after each second
	int healTimeLeft = 0; //number of seconds left to heal

	//instantaneous damage
	int damageTaken = 0;

	//forced cooldown
	bool forcedCooldown = false;
	int forcedCooldownDuration = 0; //duration of status

	//damage over time status
	bool damageOverTime = false;
	int damagePerTick;
	int damageTimeLeft = 0;
	
	//immunity status; ignore next incoming spell packet
	bool immune = false;
	int immuneTimeLeft = 0;
	
	//cooldown reduction (instantaneous)
	int cooldownReduction = 0;

	//damage boost status
	bool empowered = false;
	float empoweredAmplification; //will be passed some float like wardReduction to multiply (1.X)
	int empoweredDuration = 0;

	//instantaneous heal
	int heal = 0;
}

struct PlayerStaffData* initPlayerStruct(bool*);

void unloadPlayerData(struct PlayerStaffData*);

void rumbleHandler(struct PlayerStaffData*, int, int);

void lightHandler(struct PlayerStaffData*, int);

void soundHandler(struct PlayerStaffData*, int);

bool isCurrCasting(struct PlayerStaffData*);

bool isDoneCasting(struct PlayerStaffData*);

int wasAttacked(struct PlayerStaffData*);

void imuInputHandler(struct PlayerStaffData*);

void attackHandler(struct PlayerStaffData*, int);

void spellCaster(struct PlayerStaffData*, int);

void sendCast(struct PlayerStaffData*);

void processDamageRecieved(struct PlayerStaffData*, int);

void endCasting(struct PlayerStaffData*, bool);


#endif
