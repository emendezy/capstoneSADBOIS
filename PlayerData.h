/*
 * Struct with player data,
 * Dictionaries with spell types and sounds to play
 */

#include <studio.h>
#include <time.h>

#ifndef HEADER_GAURD_FOR_PLAYER_DATA
#define HEADER_GAURD_FOR_PLAYER_DATA

#define NUM_SPELLS_ALLOWED = 3;

#define SPELL_CASTING_RUMBLE_LEVEL = 20; /* out of 100 */
#define SPELL_END_RUMBLE_LEVEL = 70; /* out of 100 */

#define SPELL_CASTING_LIGHT_LEVEL = 20; /* out of 100 */
#define SPELL_END_LIGHT_LEVEL = 70; /* out of 100 */

/* 4,000,000,000 clocks = 1 sec ??-> TODO: need to confirm number with tests */
#define END_SPELL_RUMBLE_TIME = 4000000000 * 1.5;
#define END_SPELL_LIGHT_TIME = 4000000000 * 1.5;

/* spells allowed in staff inventory */
const size_t numSpellsAllowed = NUM_SPELLS_ALLOWED;
/* number of spells in our pre-defined book */
const size_t numSpellsInBook = 5;
/* number of sounds in out pre-defined book */
const size_t numOfSounds = 2;

struct PlayerStaffData;

typedef struct PlayerStaffData
{
	int (*spells)[numSpellsAllowed]; /* player enters keys of spells desired */
	int currentSpell;
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
} PlayerData;

/* each index of the array equates to it's spell ID */
const char *bookOfSpells[numSpellsInBook] = [
	"water_defense", // 0
	"fireball", // 1
	"air_rush", // 2
	"rock_grind", // 3
	"splash" // 4
];

const char *bookOfSounds[numOfSounds] = [
	"sent_spell",
	"got_hit"
]; /* probably have these correspond with audio files */

#endif
