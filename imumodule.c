#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <getbno055.h>
#include <bno055.h>
#include <imumodule.h>
#include <errno.h>

int main()
{
    struct bnoeul *initdirptr;
    struct bnoeul *eulptr;
    struct bnolin *linptr;
    int retval = 0;

    /*
        get initial hrp values
    */

    calibrate_imu(initdirptr);

    /*
        main loop
    */
    while (true)
    {
        retval = get_eul(eulptr);
        // error getting euler orientation
        if (retval < 0)
        {
            return retval;
        }
        retval = get_lin(linptr);
        // error getting linear acceleration
        if (retval < 0)
        {
            return retval;
        }
    }
    return retval;
}

bool checkCircle(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr)
{
    clock_t time_start = clock();
    clock_t time_end = time_start;
    int retval = get_eul(bnoeulptr);
    struct bnoeul *tempeulptr;
    if (retval < 0)
    {
        
    }
    time_end = time_start + (POLYWAITTIME / CLOCKS_PER_SECOND);
    sleep(POLYWAITTIME);
    retval = get_eul(bnoeulptr);
}
bool checkLightning(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
{
    double heading = bnoeulptr->eul_head;
    double roll = bnoeulptr->eul_roll;
    double pitch = bnoeulptr->eul_pitch;
}

extern double getVelocity(struct bnolin *bnolinptr)
{
    double acc_x = bnolinptr->linacc_x;
    double acc_y = bnolinptr->linacc_y
    double acc_z = bnolinptr->linacc_z;
   

}

int calibrateImu(struct bnoeul *bnoeulptr)
{
    int retval = 0;
    bno_reset();
    retval = get_eul(bnoeulptr);

}



/*
    add spell to end of list: FIFO
*/
void enqueueSpell(short spell, spellQueueAlias *end)
{
    spellQueueAlias newNode = (spellQueueAlias) malloc(sizeof(spellQueueStruct));
    end->spellType = spell;
    end->next = NULL;
}

short dequeueSpell(spellQueueAlias *start)
{
    short spell;
    spell = start->spellType;
    start = start->next;
}

// initialize spellQueue & spellQueueStart
void initQueue()
{
    spellQueueStart->next = NULL;
    spellQueueStart->spellType = 0;
}