#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <getbno055.h>
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

    calibrateImu(initdirptr);

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
    time_end = time_start + (POLYWAITTIME / CLOCKS_PER_SEC);
    sleep(POLYWAITTIME);
    retval = get_eul(bnoeulptr);
}
bool checkLightning(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr)
{
    double heading = bnoeulptr->eul_head;
    double roll = bnoeulptr->eul_roll;
    double pitch = bnoeulptr->eul_pitc;
}

double getVelocity(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr)
{
    double acc_x = bnolinptr->linacc_x;
    double acc_y = bnolinptr->linacc_y;
    double acc_z = bnolinptr->linacc_z;
}

void calibrateImu(struct bnoeul *bnoeulptr)
{
    int retval = 0;
    bno_reset();
    retval = get_eul(bnoeulptr);
}



/*
    add spell to end of list: FIFO
*/
void enqueueSpell(short spell)
{
    /*
        allocate memory
    */
    struct spellQueueStruct *newNode = (struct spellQueueStruct*) malloc(sizeof(struct spellQueueStruct));
    /*
        fill in data of new struct
    */
    newNode->next = NULL;
    newNode->spellType = spell;
    /*
        if adding to empty list set values
    */
    if (spellQueueStart == NULL)
    {
        spellQueueStart = newNode;
        spellQueueEnd = newNode;
    }
    else
    {
        /*
            update next pointer of last element that existed before
        */
        spellQueueEnd->next = newNode;
        /*
            update last pointer
        */
        spellQueueEnd = newNode;
    }
    
}

/*
    remove spell from start of list: FIFO
    returns -1 on error
    0 = earth
    1 = fire
    2 = lightning
    3 = water
    4 = wind
*/
short dequeueSpell()
{
    short spell;
    struct spellQueue *start;
    if (spellQueueStart == NULL)
    {
        return -1;
    }
    spell = spellQueueStart->spellType;
    spellQueueStart = spellQueueStart->next;
    /*
        free memory
    */
    free((void *) start);
    return spell;
}

// initialize spellQueue & spellQueueStart
void initQueue()
{
    spellQueueStart = NULL;
    spellQueueEnd = NULL;
}