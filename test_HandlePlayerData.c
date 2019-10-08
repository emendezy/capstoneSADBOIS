/*
 * Test file for HandlePlayerData.c
 */

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

void test_rumbleHandler(struct PlayerStaffData* actual) {
	rumbleHandler(actual, TURN_ON);
	rumbleHandler(actual, INCR);
	rumbleHandler(actual, INCR);
	assert(actual->isRumbling == true);
	assert(actual->rumbleLevel == 3);

	rumbleHandler(actual, DECR);
	assert(actual->rumbleLevel == 2);
	rumbleHandler(actual, TURN_OFF);
	assert(actual->isRumbling == false);
	assert(actual->rumbleLevel == 0);
}

void test_lightHandler(struct PlayerStaffData* actual) {
	lightHandler(actual, TURN_ON);
	lightHandler(actual, INCR);
	lightHandler(actual, INCR);
	assert(actual->isLit == true);
	assert(actual->lightLevel == 3);

	lightHandler(actual, DECR);
	assert(actual->lightLevel == 2);
	lightHandler(actual, TURN_OFF);
	assert(actual->isLit == false);
	assert(actual->lightLevel == 0);
}

int main() {
	/*
	 * All functions are called here for testing
	 */

	/* Test initialize struct */
	struct PlayerStaffData *actual = initPlayerStruct();
	assert(compareStructs(P_expected, actual));

	/* test rumbleHandler */
	test_rumbleHandler(actual);
	assert(compareStructs(P_expected, actual)); /* left actual unchanged */

	/* test lightHandler */
	test_lightHandler(actual);
	assert(compareStructs(P_expected, actual)); /* left actual unchanged */

	printf("Everything ran to completion!\n");
}
