/*
 * Listener.c - Listen for input and react accordingly
 */

#include "Listener.h"

bool gameIsActive = false;

void playTheGame()
{
	//-------------------------------------
	// Setup
	//-------------------------------------

	/* initialize player struct */
	struct PlayerStaffData* P = initPlayerStruct();

	/* Instantiate variables needed for the game loop */
	bool isCurrentlyCasting = false;
	bool isBeingAttacked = false;

	//-------------------------------------
	// Loop while game is active
	//-------------------------------------

	while(gameIsActive)
	{
		/* Be listening for user input or enemy attack */
		isCurrentlyCasting = isCasting(P);
		isBeingAttacked = wasAttacked(P);

		if (isCurrentlyCasting && !isBeingAttacked)
		{
			spellCaster(P);
		}
		else if (!isCurrentlyCasting && isBeingAttacked)
		{
			/* decrement health
			 * provide haptic feeback to the user
			 */
		}
		else if (isCurrentlyCasting && isBeingAttacked)
		{
			/* need to interupt casting process
			 * - this won't neccissarily be called here
			 * - it will most likely be some interupt throughout the player
			 *   handler
			 */
		}
		else
		{
			/* nothing is happening here - no cast and no attack detected */
		}

		/* End game when Health reaches 0
		 *  - Change gameInProgress to false
		 *  - Send game over signal to other player
		 */
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
	while(1)
	{
		if (gameIsActive)
			playTheGame();
		else
			waitForGameToStart();
	}
}
