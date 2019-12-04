#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include "imumodule.h"

    /*
        we can use pointers for these as they start as NULL
    */
spellQueueAlias *spellQueueStart; // other files read spells from this guy
spellQueueAlias *spellQueueEnd; // end of queue
/*
    we can't use pointers here because we
*/
runeClassDataAlias runeClassData; // other files access this global struct
struct bnoeul *initEulPtr; // stores initial orientation



int imuMain(struct PlayerStaffData *P)
{
    printf("imumodule.c: starting imuMain\n");
    short currSpellType;
    clock_t start_t = 0;
    clock_t curr_t = 0;
    int numloop = 1;
    double time_passed = 0;
    int retval = 0;
    initEulPtr = (struct bnoeul *) malloc(sizeof(struct bnoeul));

    /*
        get initial hrp values
        passes to global pointer
    */
    printf("imumodule.c: initializeImu()\n");
    initializeImu();
    /*
        main loop
    */
    start_t = clock();
    printf("imumodule.c: about to start inner while loop before iscasting\n");
    while (P->isCasting)
    {
        curr_t = clock();
        time_passed = ((double)(curr_t - start_t)) / CLOCKS_PER_SEC;
        if (time_passed < FRAMEWAITTIME * numloop)
        {
            continue;
        }
        else
        {
            numloop++;
        }
        
        currSpellType = classifyShape();
        if ((0 <= currSpellType) && (currSpellType <= 4))
        {
            enqueueSpell(currSpellType);
            printf("imumodule.c: spellqueued!!!\n");
            printf("imumodule.c: spell type is %d\n", (int)currSpellType);
        }
    }
    printf("imumodule.c: exiting imuMain\n");
    free((void *)initEulPtr);
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
short checkCircle(struct bnoeul *starteulptr, struct bnoeul *curreulptr)
{
    short retval = NOTCIRCLE;
    double start_h;
    double start_r;
    double h;
    double r;
    
    start_h = starteulptr->eul_head;
    start_r = starteulptr->eul_roll;
    h = curreulptr->eul_head;
    r = curreulptr->eul_roll;

    if (angDiffWrap(r, start_r) > MAXPOLYDEV)
    {
        retval = WATER;
    }
    else if (angDiffWrap(h, start_h) > MAXPOLYDEV)
    {
        retval = WIND;
    }

    return retval;
}

/*
    checks if rune is for lightning
*/
bool checkLightning(struct bnoeul *starteulptr, struct bnoeul *curreulptr)
{
    bool result = true;
    double start_h;
    double h;
    double angle;

    start_h = starteulptr->eul_head;
    h = curreulptr->eul_head;
    angle = angDiffWrap(start_h, h);
    
    if (angle < ANGLETOLLIGHT)
    {
        result = false;
    }
    else if (angle > ANGLELIGHT + ANGLETOLLIGHT)
    {
        result = false;
    }
    return result;
}

bool checkFire(struct bnoeul *starteulptr, struct bnoeul *curreulptr)
{
    double start_p;
    double p;
    start_p = starteulptr->eul_pitc;
    p = curreulptr->eul_pitc;
    if (angDiffWrap(p, start_p) > ANGLETOLPITCH)
    {
        return true;
    }
    return false;
}

short classifyShape()
{
    short currSpellType;
    int errval;
    double time_passed;
    clock_t start_t;
    clock_t curr_t;
    struct bnoeul *starteulptr;
    struct bnoeul *curreulptr;
    bool isLightning;
    bool isFire;

    /*
        get starting angle
    */
    curreulptr = (struct bnoeul *) malloc(sizeof(struct bnoeul));
    starteulptr = (struct bnoeul *) malloc(sizeof(struct bnoeul));
    
    start_t = clock();

    

    while (true)
    {
        curr_t = clock();
        printf("imumodule: starting classifyShape\n");
        time_passed = ((double)(curr_t - start_t)) / CLOCKS_PER_SEC;
        if ((time_passed > POLYWAITTIME) && (time_passed < LIGHTNINGWAITTIME))
        {
            errval = get_eul(curreulptr);
            if (errval < 0)
            {
                currSpellType = ERRORVAL;
            }
            currSpellType = checkCircle(starteulptr, curreulptr);
            if (currSpellType == WATER)
            {
                currSpellType = WATER;
                break;
            }
            else if (currSpellType == WIND)
            {
                currSpellType = WIND;
                break;
            }
        }
        if (time_passed > LIGHTNINGWAITTIME)
        {
            errval = get_eul(curreulptr);
            isLightning = checkLightning(starteulptr, curreulptr);
            if (isLightning)
            {
                currSpellType = LIGHTNING;
                break;
            }
            isFire = checkFire(starteulptr, curreulptr);
            if (isFire)
            {
                currSpellType = FIRE;
                break;
            }
        }
    }
    free((void *) starteulptr);
    free((void *) curreulptr);
    return currSpellType;
}

double getDistance(clock_t start, clock_t end, struct bnoeul *bnoeulptr, struct bnolin *bnolinptr)
{
    double acc_x = bnolinptr->linacc_x;
    double acc_y = bnolinptr->linacc_y;
    double acc_z = bnolinptr->linacc_z;
    return acc_x + acc_y + acc_z;
}

bool hasValidLength(short spellType, double drawLen)
{
    if (spellType == WATER || spellType == WIND ||
        spellType == FIRE || spellType == EARTH)
    {
        return drawLen > MINDRAWLEN;
    }
    else if (spellType == LIGHTNING)
    {
        return drawLen > MINDRAWLENLIGHT;
    }
    return false;
}


void initializeImu()
{
    printf("imumodule.c: entered initializeImu()\n");
    int retval = 0;
    bno_reset();
    retval = get_eul(initEulPtr);
    if (retval < 0)
    {
        printf("Error in initializeIMU\n");
    }
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
    spellQueueAlias *oldstart;
    oldstart = spellQueueStart;
    if (spellQueueStart == NULL)
    {
        return -1;
    }
    spell = spellQueueStart->spellType;
    spellQueueStart = spellQueueStart->next;
    /*
        free memory
    */
    free((void *) oldstart);
    return spell;
}

// initialize spellQueue & spellQueueStart
void initQueue()
{
    spellQueueStart = NULL;
    spellQueueEnd = NULL;
}


/* 
    convert two angles from -180~180  to 0~360
*/
double angDiffWrap(double angle1, double angle2)
{
    double result = 0;
    double abig = angle1;
    double asmall = angle2;
    if (angle1 < angle2)
    {
        abig = angle2;
        asmall = angle1;
    }
    result = abig - asmall;
    if (result > 180)
    {
        result = 360 - result;
    }
    return result;
}