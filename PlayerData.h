/*
 * Struct with player data,
 * Dictionaries with spell types and sounds to play
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#ifndef HEADER_GAURD_FOR_PLAYER_DATA
#define HEADER_GAURD_FOR_PLAYER_DATA

// #define NDEBUG /* Comment for no debug mode */
#define TURN_OFF 0
#define TURN_ON 1
#define INCR 2
#define DECR 3

#define MAX_RUMBLE 10
#define MAX_LIGHT 10

/* number of spells in our pre-defined book */
#define TOTAL_SPELLS_IN_SPELLBOOK 5
/* number of sounds in out pre-defined book */
#define TOTAL_NUM_OF_SOUNDS 2

/* ------------------------------------------------------------------------- */

struct PlayerStaffData
{
	int currentSpell; // from book of spells
	bool isCasting;

	bool isRumbling;
	size_t rumbleLevel; /* 0 -> 10 */
	clock_t rumbleStartTime;

	bool isLit;
	size_t lightLevel; /* 0 -> 10 */
	clock_t lightStartTime;

	bool isLoud;
	int keyOfSoundToPlay; /* if none -> -1, else -> key for soundBook */

	bool isShielding;
	size_t shieldPercent; /* 0(empty) -> 100(full) */

	size_t healthPercent; /* 0(dead) -> 100(full) */
};

/* each index of the array equates to it's spell ID */
// const char* const bookOfSpells[] = {
// 	"no_spell_chosen", // 0 (default case)
// 	"water_defense", // 1
// 	"fireball", // 2
// 	"air_rush", // 3
// 	"rock_grind", // 4
// 	"splash" // 5
// };

// const char* const bookOfSounds[] = {
// 	"sent_spell",
// 	"got_hit"
// };

struct PlayerStaffData* initPlayerStruct();

void rumbleHandler();

void lightHandler();

void changeSpell();

void startCasting();

void endCasting();

void stopRumbler();

void stopLights();


#endif
