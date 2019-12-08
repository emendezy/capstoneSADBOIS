#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include "imumodule.h"

/*
    Macros for debugging
*/
#ifndef __DEBUG_IMU__
#define __DEBUG_IMU__ 1
#endif

/*
    debugging printing code from
    https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
*/
#define debug_print(fmt, ...) \
        do { if (__DEBUG_IMU__) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

/*
        we can use pointers for these as they start as NULL
    */
spellQueueAlias *spellQueueStart; // other files read spells from this guy
spellQueueAlias *spellQueueEnd; // end of queue

/*
    we can't use pointers here because we
*/
runeClassDataAlias runeClassData; // other files access this global struct
struct bnoeul *initEulPtr;




int imuMain(struct PlayerStaffData *P)
{
    debug_print("imumodule.c: starting imuMain Line %s %d\n", __FILE__, __LINE__);
    short currSpellType;
    clock_t start_t = 0;
    clock_t curr_t = 0;
    int numloop = 1;
    double time_passed = 0;
    int retval = 0;
    initEulPtr = (struct bnoeul *) malloc(sizeof(struct bnoeul));
    // senaddr = (char *) malloc(sizeof(char)*256);


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
            if (currSpellType == 0)
            {
                printf("spell type is EARTH %s line %d\n", __FILE__, __LINE__);
            }
            else if (currSpellType == 1)
            {
                printf("spell type is FIRE %s line %d\n", __FILE__, __LINE__);
            }
            else if (currSpellType == 2)
            {
                printf("spell type is LIGHTNING %s line %d\n", __FILE__, __LINE__);
            }
            else if (currSpellType == 3)
            {
                printf("spell type is WATER %s line %d\n", __FILE__, __LINE__);
            }
            else if (currSpellType == 4)
            {
                printf("spell type is WIND %s line %d\n", __FILE__, __LINE__);
            }
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
short checkCircle(struct bnoeul *starteulptr, struct bnoeul *curreulptr, 
    struct bnogra* startgraptr)
{
    short retval = NOTCIRCLE;
    short gravdir;
    double start_h;
    double start_r;
    double start_p;
    double h;
    double r;
    double p;
    double dhead;
    double droll;
    double dpitch;
    gravdir = checkGravDir(startgraptr);
    start_h = starteulptr->eul_head;
    start_r = starteulptr->eul_roll;
    start_p = starteulptr->eul_pitc;
    h = curreulptr->eul_head;
    r = curreulptr->eul_roll;
    p = curreulptr->eul_pitc;

    dhead = angDiffWrap(h, start_h);
    droll = angDiffWrap(r, start_r);
    dpitch = angDiffWrap(p, start_p);
    
    if (dhead > MAXPOLYDEV && gravdir == GRAVY)
    {
        retval = WATER;
    }
    else if (dhead > MAXPOLYDEV && droll < ANGLETOLROLL 
        && dpitch < ANGLETOLPITCH && gravdir == GRAVZ)
    {
        retval = WIND;
    }

    return retval;
}

/*
    checks if rune is for lightning
*/
bool checkLightning(struct bnoeul *starteulptr, struct bnoeul *curreulptr,
    struct bnogra *startgraptr)
{
    bool result = true;
    double start_r;
    double r;
    double angle;
    short gravdir;
    gravdir = checkGravDir(startgraptr);

    start_r = starteulptr->eul_roll;
    r = curreulptr->eul_roll;
    angle = angDiffWrap(start_r, r);
    
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

bool checkFire(struct bnoeul *starteulptr, struct bnoeul *curreulptr,
    struct bnogra *startgraptr)
{
    short gravdir;
    double start_h;
    double h;
    double angle = angDiffWrap(h, start_h);
    gravdir = checkGravDir(startgraptr);
    start_h = starteulptr->eul_head;
    h = curreulptr->eul_head;
    if (angle > ANGLEFIRE && gravdir == GRAVX)
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
    struct bnogra *startgraptr;
    bool isLightning;
    bool isFire;

    /*
        get starting angle
    */
    curreulptr = (struct bnoeul *) malloc(sizeof(struct bnoeul));
    starteulptr = (struct bnoeul *) malloc(sizeof(struct bnoeul));
    startgraptr = (struct bnogra *) malloc(sizeof(struct bnogra));
    
    start_t = clock();
    errval = get_eul(starteulptr);
    errval = get_gra(startgraptr);

    while (true)
    {
        curr_t = clock();
        time_passed = ((double)(curr_t - start_t)) / CLOCKS_PER_SEC;
        if ((time_passed > POLYWAITTIME) && (time_passed < LIGHTNINGWAITTIME))
        {
            errval = get_eul(curreulptr);
            if (errval < 0)
            {
                currSpellType = ERRORVAL;
            }
            /*
                check for wind or water
            */
            currSpellType = checkCircle(starteulptr, curreulptr, startgraptr);
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
            isLightning = checkLightning(starteulptr, curreulptr, startgraptr);
            if (isLightning)
            {
                currSpellType = LIGHTNING;
                break;
            }
            isFire = checkFire(starteulptr, curreulptr, startgraptr);
            if (isFire)
            {
                currSpellType = FIRE;
                break;
            }
        }
    }
    free((void *) starteulptr);
    free((void *) curreulptr);
    free((void *) startgraptr);
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
    int retval = 0;
    time_t tsnow = time(NULL);
    printf("about to call get_i2cbus %s line %d\n", __FILE__, __LINE__);
    printf("senaddr is %s %s line %d\n", senaddr, __FILE__, __LINE__);
    get_i2cbus(senaddr);
    printf("entered initializeImu() %s line %d\n", __FILE__, __LINE__);
    // bno_reset();
    printf("initEulPtr == %d,%s line %d\n", ((int) initEulPtr), __FILE__, __LINE__);
    retval = get_eul(initEulPtr);
    if (retval < 0)
    {
        printf("Error in initializeIMU\n");
    }
    printf("imumodule.c: exiting initializeImu()\n");
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
    convert the difference between two angles
    such that it is easier to compare differences
    result will be no more than 180 degrees;
    direction will not matter
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

/*
    outputs the direction in which gravity is greatest:
    GRAVX == 0
    GRAVY == 1
    GRAVZ == 2
*/
short checkGravDir(struct bnogra *startgraptr)
{
    short gravdir;
    double gx; double gy; double gz;
    gravdir = GRAVX;
    gx = fabs(startgraptr->gravityx);
    gy = fabs(startgraptr->gravityy);    
    gz = fabs(startgraptr->gravityz);
    if (gx > gy && gx > gz)
    {
        gravdir = GRAVX;
    }
    else if (gy > gx && gy > gz)
    {
        gravdir = GRAVY;
    }
    else if (gz > gx && gz > gy)
    {
        gravdir = GRAVZ;
    }
    return gravdir;
}