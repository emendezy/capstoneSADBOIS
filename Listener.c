/*
 * Listener.c - Listen for input and react accordingly
 */

#include "Listener.h"

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
	bool stopCasting = false;
	int damageType;

	int testOnce = 1;

	/* Install Signal Handler(s) */
	signal(SIGCHLD, sigchld_handler); // Handles terminated or stopped child
	sigset_t prev_mask, mask;

	//-------------------------------------
	// Loop while game is active
	//-------------------------------------
	printf("game is active\n");
	while(gameIsActive)
	{
		sigfillset(&mask);
		sigprocmask(SIG_BLOCK, &mask, &prev_mask);
		P->prev_mask = prev_mask;

		// fake code
		if(testOnce == 1)
		{
			soundHandler(P, 0);
			testOnce = 0;
		}

		/* Be listening for user input or enemy attack */
		isCurrentlyCasting = isCasting(P);
		// ^ returns true if button pressed or button was already pressed and
		//     P->isCasting was set to true
		stopCasting = isDoneCasting(P); /* handle send spell logic */

		damageType = wasAttacked(P);
		// TODO - look up damage Type's value to apply the damage done
		// ^ damageType represents index for damages[] that has damage values
		if(damageType > -1)
			isBeingAttacked = true;
		else
			isBeingAttacked = false;

		if (isCurrentlyCasting && !isBeingAttacked)
		{
			if(!stopCasting)
			{
				spellCaster(P, damageType);
				/* ^ handle attack interupt in spellCaster */
			}
			/* successful cast end is handled in isDoneCasting() */
		}
		else
		{
			attackHandler(P, damageType);

		}

		// unblock signals
		sigprocmask(SIG_SETMASK, &prev_mask, NULL);
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
	if (gameStartButtonPressed())/*TODO - button to start game is pressed */
	{
		printf("game button set to true\n");
		delay(1000);
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
	printf("in main loop\n");
	while(1)
	{
		if (gameIsActive)
			playTheGame();
		else
			waitForGameToStart();
	}
}

/* 		(* From Shell lab - 15213 *)
 * int sig: signal, should be SIGCHLD (which is why this is sigchld_handler)
 * waits for pid of child and notifies user of stop/term signals in child
 * child process, deletes terminated and exited jobs
 */
void sigchld_handler(int sig) {
	int status;
	pid_t pid;
	jid_t jid;
	int err = errno; //maintain errno

	sigset_t prev_mask, mask;
	sigfillset(&mask);

	//wait for the child that has this pid
	while ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0) {
		sigprocmask(SIG_BLOCK, &mask, &prev_mask);
		jid = job_from_pid(pid);

		delete_job(jid); // only occurs if status was terminated or exited
		sigprocmask(SIG_SETMASK, &prev_mask, NULL);
	}
	errno = err;
	return;
}
