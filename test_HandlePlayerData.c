/*
 * Test file for HandlePlayerData.c
 */

#include <assert.h>
#include "PlayerData.h"

/* create fake initially created PlayerData struct */
struct PlayerStaffData* P_expected = &(struct PlayerStaffData){
	.currentSpell = 0,
	.isCasting = false,
	.isRumbling = false,
	.rumbleLevel = false,
	.rumbleStartTime = 0,
	.isLit = false,
	.lightLevel = 0,
	.lightStartTime = 0,
	.isLoud = false,
	.keyOfSoundToPlay = -1,
	.isShielding = false,
	.shieldPercent = 100,
	.healthPercent = 100
};

bool compareStructs(struct PlayerStaffData* expected,
					struct PlayerStaffData* actual) {
	/* not null */
	assert(expected);
	assert(actual);

	/* check internal values */
	assert(expected->currentSpell == actual->currentSpell);
	assert(expected->isCasting == actual->isCasting);
	assert(expected->isRumbling == actual->isRumbling);
	assert(expected->rumbleLevel == actual->rumbleLevel);
	assert(expected->rumbleStartTime == actual->rumbleStartTime);
	assert(expected->isLit == actual->isLit);
	assert(expected->lightLevel == actual->lightLevel);
	assert(expected->lightStartTime == actual->lightStartTime);
	assert(expected->isLoud == actual->isLoud);
	assert(expected->keyOfSoundToPlay == actual->keyOfSoundToPlay);
	assert(expected->isShielding == actual->isShielding);
	assert(expected->shieldPercent == actual->shieldPercent);
	assert(expected->healthPercent == actual->healthPercent);

	return true;
}

int main() {
	/*
	 * All functions are called here for testing
	 */

	/* Test initialize struct */
	struct PlayerStaffData *actual = initPlayerStruct();
	assert(compareStructs(P_expected, actual));

	printf("Everything ran to completion!\n");
}
