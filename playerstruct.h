#include <time.h>
#include <stdbool.h>
#include "tsh_exec.h"
#include "tsh_helper.h"


struct PlayerStaffData
{
	bool* gameInProgress;
	int* activeSpells; // from book of spells
	bool isCasting;
	bool startOfSpell;
	int castDamage; // damage of spell created by this user

	struct spellQueueStruct *spellQueue;

	int hasBastion;
	bool hasImmunity;
	int immunityTime;

	bool isBurning;
	int burnPerSecond;
	int burnTotalTime;

	bool isWeakened;
	int weaknessPercent;
	int weaknessTime;
	clock_t weaknessStart;

	int* coolDownMask;

	int* damageValues;

	bool isRumbling;
	int rumbleType; /* Can be END_CAST (0) or SPELL_START (1) */
	int rumbleCount;

	bool isLit;
	int lightLevel; /* 0 -> 10 */
	clock_t lightStartTime;

	bool isLoud;
	int keyOfSoundToPlay; /* if none -> -1, else -> key for soundBook */

	bool isShielding;
	int shieldPercent; /* 0(empty) -> 100(full) */
	int shieldTime;

	int healthPercent; /* 0(dead) -> 100(full) */
	bool isHealing;
	int healthRestorePerSecond;
	int healthRestoreTime;

	clock_t frontOfOneQuarterSecond;
	clock_t frontOfOneSecond;
	clock_t mostRecentTime;

	sigset_t prev_mask;// for forking (masks for block/unblock)
};