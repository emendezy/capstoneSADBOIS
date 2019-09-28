/*
 * Struct with player data,
 * Dictionaries with spell types and sounds to play
 */

#include <stdio.h>

/* spells allowed in staff inventory */
const size_t numSpellsAllowed = 3;
/* number of spells in our pre-defined book */
const size_t numSpellsInBook = 5;
/* number of sounds in out pre-defined book */
const size_t numOfSounds = 2;

struct playerStaffData
{
	int spells[numSpellsAllowed] = [0,2,3];
	bool isCasting = false;

	bool isRumbling = false;
	size_t rumbleLevel = 0; /* 0 -> 100 */

	bool isLit = false;

	bool isLoud = false;
	int keyOfSoundToPlay = -1; /* if none -> -1, else -> key for soundBook */

	bool isShielding = false;
	size_t shieldPercent = 100; /* 0(empty) -> 100(full) */

	size_t healthPercent = 100; /* 0(dead) -> 100(full) */
};

/* each index of the array equates to it's spell ID */
const char* bookOfSpells[numSpellsInBook] = [
	"water_defense", // 0
	"fireball", // 1
	"air_rush", // 2
	"rock_grind", // 3
	"splash" // 4
];

const char* bookOfSounds[numOfSounds] = [
	"sent_spell",
	"got_hit"
];
