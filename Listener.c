/*
 * Listener.c - Listen for input and react accordingly
 */

#include "Listener.h"

gameIsActive = false;

void playTheGame()
{
	//-------------------------------------
	// Setup
	//-------------------------------------

	/* initialize player struct */
	struct PlayerStaffData* P = initPlayerStruct(&gameIsActive);

	/* Instantiate variables needed for the game loop */
	bool isCurrentlyCasting = false;
	bool isBeingAttacked = false;
	int damage = 0;

	//-------------------------------------
	// Loop while game is active
	//-------------------------------------

	while(gameIsActive)
	{
		/* Be listening for user input or enemy attack */
		isCurrentlyCasting = isCasting(P);
		damage = wasAttacked(P);
		if(damage != -1)
			isBeingAttacked = true;
		else
			isBeingAttacked = false

		if (isCurrentlyCasting)
		{
			spellCaster(P, damage); /* handle attack interupt in spellCaster */
		}
		else
		{
			if(isBeingAttacked)
			{
				/* decrement health
				 * provide haptic feeback to the user
				 */
				attackHandler(P, damage);
			}

		}
	}

	//-------------------------------------
	// Game Over - run cleanup code
	//-------------------------------------

	unloadPlayerData(P);
}

void waitForGameToStart()
{
	//-------------------------------------
	// Loop while game is inactive
	//-------------------------------------
	if (/* button to start game is pressed */)
	{
		gameIsActive = true;
	}

}

/*
 * Main loop for I/O control on Raspberry Pi
 * Currently flow is designed in psudo code
 */
int main()
{
	initializeRaspberryPi();
	while(1)
	{
		if (gameIsActive)
			playTheGame();
		else
			waitForGameToStart();
	}
}
