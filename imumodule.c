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
    bool isCircle = false;
    bool isLightning = false;
    int retval = 0;

    /*
        get initial hrp values
        passes to global pointer
    */
    calibrateImu();
    /*
        main loop
    */
    while (true)
    {
        sleep(FRAMEWAITTIME);
        retval = get_eul(&currEulStruct);
        // error getting euler orientation
        if (retval < 0)
        {
            return retval;
        }
        retval = get_lin(&currLinStruct);
        // error getting linear acceleration
        if (retval < 0)
        {
            return retval;
        }
        currSpellType = classifyShape(&currEulStruct, &currLinStruct);        
    }
    return retval;
}

/*
    checks if rune is a Horizontal or Vertical circle
    must pass pointers bnoeulptr and bnolinptr that are already filled
    waits POLYWAITTIME; if angle change is > MAXPOLYWAITTIME
    returns WIND if wind (horizontal circle)
    returns WATER if water (vertical circle)
    returns NOTCIRCLE otherwise
*/
short checkCircle(struct bnoeul *starteulptr, struct bnolin *bnolinptr)
{
    int retval;
    double final_h;
    double final_r;
    double final_p;
    struct bnoeul *finaleulptr;
    double init_h = starteulptr->eul_head;
    double init_r = starteulptr->eul_roll;
    double init_p = starteulptr->eul_pitc;
    clock_t time_start = clock();
    // allocate memory for struct
    finaleulptr = (struct bnoeul *) malloc(sizeof(struct bnoeul));

    // wait POLYWAITTIME
    sleep(POLYWAITTIME);
    retval = get_eul(finaleulptr);
    if (retval < 0)
    {
        return false;
    }
    
    // free memory after casting to void*
    free((void *)finaleulptr);

}

/*
    checks if rune is for lightning
*/
bool checkLightning(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr)
{
    double init_h = initEulStruct.eul_head;
    double init_r = initEulStruct.eul_roll;
    double init_p = initEulStruct.eul_pitc;
    double h = bnoeulptr->eul_head;
    double r = bnoeulptr->eul_roll;
    double p = bnoeulptr->eul_pitc;
    if (init_r - r < ANGLETOLPITCH)
    {
        return false;
    }
    else if (init_h - h < ANGLETOLHEAD)
    {
        return false;
    }
}

short classifyShape(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr)
{
    short circleType = 0;
    bool isLightning;

    circleType = checkCircle(bnoeulptr, bnolinptr);
    isLightning = checkLightning(bnoeulptr, bnolinptr);
    if (circleType == WATER)
    {
        currSpellType = WATER;
        return currSpellType;
    }
    else if (circleType == WIND)
    {
        currSpellType = WIND;
        return currSpellType;
    }
    else if (isLightning)
    {
        currSpellType = LIGHTNING;
        return currSpellType;
    }
    return currSpellType;
}

double getDistance(clock_t start, clock_t end, struct bnoeul *bnoeulptr, struct bnolin *bnolinptr)
{
    double acc_x = bnolinptr->linacc_x;
    double acc_y = bnolinptr->linacc_y;
    double acc_z = bnolinptr->linacc_z;
}

void calibrateImu()
{
    int retval = 0;
    bno_reset();
    retval = get_eul(&initEulStruct);
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