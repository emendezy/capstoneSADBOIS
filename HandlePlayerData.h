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

// #include "tsh_exec.h"
// #include "csapp.h"
// #include "tsh_helper.h"


// #define NDEBUG /* Comment for no debug mode */
#define TURN_OFF 0
#define TURN_ON 1
#define INCR 2
#define DECR 3

#define NO_RUMBLE -1
#define END_CAST 0
#define SPELL_CASTING 1

#define ENDING_RUMBLE_ALT 7 /* Even numbers mean on | Odd numbers mean off */

// EARTH = 0;
// FIRE = 1;
// LIGHTNING = 2;
// WATER = 3;
// WIND = 4;

#define MAX_RUMBLE 10
#define MAX_LIGHT 10

#define MAX_HEALTH 100
#define MAX_SHIELD 100
#define MAX_WEAKNESS 100

/* number of spells in our pre-defined book */
#define TOTAL_SPELLS_IN_SPELLBOOK 5
/* number of sounds in out pre-defined book */
#define TOTAL_NUM_OF_SOUNDS 2

#define NUM_DAMAGE_VALUES 6
/* ------------------------------------------------------------------------- */



/*
 * when the player casts a self-buff, or when the player is hit by an enemy spell, use this:
 */
// struct playerSpellEffects
// {
// 	//ward status
// 	bool ward = false;
// 	float wardReduction = 0.5;
// 	int wardTimeLeft = 0; //X milliseconds

// 	//heal over time status
// 	bool healOverTime = false;
// 	int healthPerTick; //hp restored after each second
// 	int healTimeLeft = 0; //number of seconds left to heal

// 	//instantaneous damage
// 	int damageTaken = 0;

// 	//forced cooldown
// 	bool forcedCooldown = false;
// 	int forcedCooldownDuration = 0; //duration of status

// 	//damage over time status
// 	bool damageOverTime = false;
// 	int damagePerTick;
// 	int damageTimeLeft = 0;

// 	//immunity status; ignore next incoming spell packet
// 	bool immune = false;
// 	int immuneTimeLeft = 0;

// 	//cooldown reduction (instantaneous)
// 	int cooldownReduction = 0;

// 	//damage boost status
// 	bool empowered = false;
// 	float empoweredAmplification; //will be passed some float like wardReduction to multiply (1.X)
// 	int empoweredDuration = 0;

// 	//instantaneous heal
// 	int heal = 0;
// }

struct PlayerStaffData* initPlayerStruct(bool*);

void unloadPlayerData(struct PlayerStaffData*);



bool isCurrCasting(struct PlayerStaffData*);

bool isDoneCasting(struct PlayerStaffData*);

int wasAttacked(struct PlayerStaffData*);

void imuInputHandler(struct PlayerStaffData*);

void updatePlayerFields(struct PlayerStaffData*);

void attackHandler(struct PlayerStaffData*, int);

void spellCaster(struct PlayerStaffData*, int);

void healPlayer(struct PlayerStaffData*);

void handleBurning(struct PlayerStaffData*);

void editCoolDownValues(struct PlayerStaffData*, int);

void updateCooldownLightsOnStaff(int, int);

void checkWeakness(struct PlayerStaffData*);

void checkShield(struct PlayerStaffData*);

void checkImmunity(struct PlayerStaffData*);

void sendCast(struct PlayerStaffData*);

void sendDamagePackage(int*);

int calcSendingSpellDamage(struct PlayerStaffData*, int);

int calcShieldAffect(int, int);

void processDamageRecieved(struct PlayerStaffData*, int*);

void endCasting(struct PlayerStaffData*, bool);

void rumbleHandler(struct PlayerStaffData*, int);

void alternateRumble(int);

void lightHandler(struct PlayerStaffData*, int);

void soundHandler(struct PlayerStaffData*, int);

#endif
