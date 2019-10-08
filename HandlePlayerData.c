/*
 * HandlePlayerData
 * -> based on the PlayerStaffData struct, create input and output handlers
 *
 */

#include "PlayerData.h"

/* each index of the array equates to it's spell ID */
const char* const bookOfSpells[] = {
	"no_spell_chosen", // 0 (default case)
	"water_defense", // 1
	"fireball", // 2
	"air_rush", // 3
	"rock_grind", // 4
	"splash" // 5
};

const char* const bookOfSounds[] = {
	"sent_spell",
	"got_hit"
};

// -----------------------------------------------
// Initialization Steps
// -----------------------------------------------

struct PlayerStaffData* initPlayerStruct() {
	struct PlayerStaffData* P = malloc(sizeof(struct PlayerStaffData));
	P->currentSpell = 0;
	P->isCasting = false;
	P->isRumbling = false;
	P->rumbleLevel = false;
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
// Functions that react to changed Player Data
// -----------------------------------------------

/* Rumbling handler
 *  Handle Rumbling sequences based on Player data
 *	isCasting - rumble at a low frequency to provide haptic feedback
 */
void rumbler(struct PlayerStaffData* P) {
	/* Case for spell start */
	if(P->isRumbling && P->isCasting) {
		P->rumbleLevel = SPELL_CASTING_RUMBLE_LEVEL;
		P->rumbleStartTime = clock();
	}
	/* Case for spell end */
	else if(P->isRumbling && !(P->isCasting)) {
		P->rumbleLevel = SPELL_END_RUMBLE_LEVEL;
		P->rumbleStartTime = clock();
	}
}

/* Lights handler
 *  Handle light sequences based on Player data
 *	isCasting - be lit at a low level to provide haptic feedback
 *				< End goal -> make light pulse slowly >
 */
void lights(struct PlayerStaffData* P) {
	/* Case for spell start - TODO: Add light sequence */
	if(P->isLit && P->isCasting) {
		P->lightLevel = SPELL_CASTING_LIGHT_LEVEL; /* placeholder for now */
		P->lightStartTime = clock();
	}
	/* Case for spell end */
	else if(P->isLit && !(P->isCasting)) {
		/* stop light for now to signal done with spell
		 * TODO: Add light sequence */
		P->lightLevel = SPELL_END_LIGHT_LEVEL;
		P->lightStartTime = clock();
	}
}


// -----------------------------------------------
// Functions that change Player Data
// -----------------------------------------------

/* NOTE - NOT USING ROTARY ENCODER ANYMORE */
// /* Rotary Encoder for spell choice
//  * 	direction == 1 : right
//  *  direction == -1 : down
//  */
// void changeSpell(struct PlayerStaffData* P, int direction) {
// 	if(direction == 1) {
// 		if(P->currentSpell == (NUM_SPELLS_ALLOWED - 1)) P->currentSpell = 0;

// 		else P->currentSpell++;
// 	}
// 	else if(direction == -1) {
// 		if(P->currentSpell == 0) P->currentSpell = NUM_SPELLS_ALLOWED - 1;

// 		else P->currentSpell--;
// 	}
// 	else {
// 		printf("ERROR: should not be set to a different value than -1 or 1\n");
// 	}
// }

/* Button was pressed to initialize cast sequence
 */
void startCasting(struct PlayerStaffData* P) {
	P->isCasting = true;
	P->isRumbling = true;
	/* not sure how to implement the rumble sequence just yet - for just
	   using a set value */
	rumbler(P);

	P->isLit = true;
	lights(P);
}

/* Pressure sensor was pressed end cast sequence
 */
void endCasting(struct PlayerStaffData* P) {
	P->isCasting = false;
	/* Do one last rumble and LED flash to show the spell is completed and sent
	 * over to the other player */
	rumbler(P);
	lights(P);
	P->isRumbling = false; /* 1 more rumble after as the spell gets sent out */
	P->isLit = false; /* 1 more light flash after as the spell get sent out */
}

// -----------------------------------------------
// Update functions that rely on timing
// -----------------------------------------------

/*
 * Stop rumbling after set time - which is in player data
 * need to keep calling in main function to keep track of the clock cycles
 * and to to stop the rumbling at appropriate times
 *
 * Rumble stopping events:
 *  - Stop rumbling 1.5 sec after a spell has been cast and completed
 */
void stopRumbler(struct PlayerStaffData* P) {
	/* Stop the rumble after last rumble from sending the spell out */
	if(!(P->isCasting) && !(P->isRumbling) &&
		(clock() - P->rumbleStartTime >= END_SPELL_RUMBLE_TIME)) {
		P->rumbleLevel = 0;
		P->rumbleStartTime = 0; /* technically don't need to change this, but
								   just to be verbose I'm doing it anyways */
	}
}

void stopLights(struct PlayerStaffData* P) {
	/* Stop the lights gradually */
	if(!(P->isCasting) && !(P->isLit)) {
		/* TODO: Create a function that increases then decreases the light
		level so that it looks like a pulse */
	}
}
