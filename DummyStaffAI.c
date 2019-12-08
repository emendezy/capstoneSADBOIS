/* AI code for dummy staff */

#include "DummyStaffAI.h"

double frontSec = clock();
double backSec = clock();

void runAICode()
{
	frontSec = clock();
	backSec = clock();

	while(true)
	{
		backSec = clock();
		// send spells at time intervals
	}
}

void updatePlayerFields(struct PlayerStaffData* P)
{
	P->mostRecentTime = clock();
	/* Clean up code - dealing with timing gaurds */
	double fullQuarterSecond = 1;
	double timePassedQuarter = ((double)(P->mostRecentTime - P->frontOfOneQuarterSecond)) / (CLOCKS_PER_SEC/4);

	double fullSecond = 1;
	double timePassed = ((double)(P->mostRecentTime - P->frontOfOneSecond)) / (CLOCKS_PER_SEC);

	// ##################### 1/4 SEC #####################
	if(fullQuarterSecond <= timePassedQuarter && P->rumbleType == END_CAST)
	{
		if(P->rumbleCount <= ENDING_RUMBLE_ALT)
		{
			alternateRumble(P->rumbleCount); /* Even - on | Odd - off */
			P->rumbleCount++;
		}
		P->frontOfOneQuarterSecond = clock();
	}

	// ##################### 1 SEC #####################
	if(fullSecond <= timePassed)
	{
		printf("-----------------------------\n1 second mark\n");

		editCoolDownValues(P, 1);

		if(P->isHealing)
			healPlayer(P);
		if(P->isWeakened)
			checkWeakness(P);
		if(P->isShielding)
			checkShield(P);
		if(P->hasImmunity)
			checkImmunity(P);
		if(P->isBurning)
			handleBurning(P);
		if(P->rumbleType == SPELL_CASTING)
		{
			alternateRumble(P->rumbleCount);
			P->rumbleCount++;
		}

		P->frontOfOneSecond = clock();
	}
}

int main()
{
	double initSec = clock();
	bool gameStarted = false;
	bool fiveSecondsPast;

	printf("Starting AI code in 5 Seconds\n");

	while(true)
	{
		backSec = clock();
		fiveSecondsPast = ((double)(initSec - backSec) / CLOCKS_PER_SEC) >= 5;
		if (!gameStarted && fiveSecondsPast)
		{
			gameStarted = true;
			printf("Game Starting! ---------------------------------\n");
			runAICode();
			break;
		}
	}
}
