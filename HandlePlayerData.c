/*
 * HandlePlayerData
 * -> based on the PlayerStaffData struct, create input and output handlers
 *
 */

#include <stdio.h>
#include <PlayerData.h>

// -----------------------------------------------
// Initialization Steps
// -----------------------------------------------

PlayerData* initPlayerStruct(int[] spellsChosen) {
	PlayerData* P = {
		.spells = &spellsChosen,
		.currentSpell = 0,
		.isCasting = false,
		.isRumbling = false,
		.rumbleLevel = false,
		.isLit = false,
		.isLoud = false,
		.keyOfSoundToPlay = -1,
		.isShielding = false,
		.shieldPercent = 100,
		.healthPercent = 100
	};

	return P;
}

// -----------------------------------------------
// Functions that react to changed Player Data
// -----------------------------------------------

/* Rumbling handler
 *  Handle Rumbling sequences based on Player data
 *	isCasting - rumble at a low frequency to provide haptic feedback
 */
void rumbler(PlayerData* P) {
	/* Case for spell start */
	if(P->isRumbling && P->isCasting) {
		P->rumbleLevel = SPELL_CASTING_RUMBLE_LEVEL;
	}
}

/* Lights handler
 *  Handle light sequences based on Player data
 *	isCasting - be lit at a low level to provide haptic feedback
 *				< End goal -> make light pulse slowly >
 */
void lights(PlayerData* P) {
	/* Case for spell start */
	if(P->isLit && P->isCasting) {
		P->litLevel = SPELL_CASTING_LIGHT_LEVEL; /* placeholder for now */
	}
}


// -----------------------------------------------
// Functions that change Player Data
// -----------------------------------------------

/* Rotary Encoder for spell choice
 * 	direction == 1 : right
 *  direction == -1 : down
 */
void changeSpell(PlayerData* P, int direction) {
	if(direction == 1) {
		if(P->currentSpell == NUM_SPELLS_ALLOWED - 1) P->currentSpell = 0;

		else P->currentSpell++;
	}
	else if(direction == -1) {
		if(P->currentSpell == 0) P->currentSpell = NUM_SPELLS_ALLOWED - 1;

		else P->currentSpell--;
	}
	else {
		printf("ERROR: should not be set to a different value than -1 or 1\n");
	}
}

/* Button pressed to initialize cast sequence
 */
void startCasting(PlayerData* P) {
	P->isCasting = true;
	P->isRumbling = true;
	/* not sure how to implement the rumble sequence just yet - for just
	   using a set value */
	rumbler(P);

	P->isLit = true;
	lights(P);
}
