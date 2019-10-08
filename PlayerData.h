/*
 * Struct with player data,
 * Dictionaries with spell types and sounds to play
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#ifndef HEADER_GAURD_FOR_PLAYER_DATA
#define HEADER_GAURD_FOR_PLAYER_DATA

/* spells allowed in staff inventory */
#define NUM_SPELLS_ALLOWED 3
/* number of spells in our pre-defined book */
#define TOTAL_SPELLS_IN_SPELLBOOK 5
/* max length of spell name */
#define MAX_SPELL_NAME_LENGTH 10
/* number of sounds in out pre-defined book */
#define TOTAL_NUM_OF_SOUNDS 2

#define SPELL_CASTING_RUMBLE_LEVEL 20 /* out of 100 */
#define SPELL_END_RUMBLE_LEVEL 70 /* out of 100 */

#define SPELL_CASTING_LIGHT_LEVEL 20 /* out of 100 */
#define SPELL_END_LIGHT_LEVEL 70 /* out of 100 */

/* 4,000,000,000 clocks = 1 sec ??-> TODO: need to confirm number with tests */
#define END_SPELL_RUMBLE_TIME (4000000000 * 1.5)
#define END_SPELL_LIGHT_TIME (4000000000 * 1.5)

/* ------------------------------------------------------------------------- */

struct PlayerStaffData
{
	int currentSpell; // from book of spells
	bool isCasting;

	bool isRumbling;
	size_t rumbleLevel; /* 0 -> 100 */
	clock_t rumbleStartTime;

	bool isLit;
	size_t lightLevel; /* 0 -> 100 */
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

void rumbler();

void lights();

void changeSpell();

void startCasting();

void endCasting();

void stopRumbler();

void stopLights();


#endif
