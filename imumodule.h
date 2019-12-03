#include <stdbool.h>

/*
    global constants
*/
static unsigned FRAMEWAITTIME = 10;
static unsigned POLYWAITTIME = 20; // time in ms must be unsigned to be passed to sleep()
static double MAXPOLYDEV = 30; // angle in degrees
    /*
        angle tolerances in degrees used when there should be no movement in that direction
    */
static double ANGLETOLHEAD = 15;
static double ANGLETOLROLL = 15;
static double ANGLETOLPITCH = 15;
    /*
        multiplier used when movement exists in that  direction
    */
static double ANGLETOLMULTIPLIER = 0.25; // y = a*(1+x)
static double MINDRAWLEN = 1.8; // in meters
static double MINDRAWLENLIGHT = 0.3; // in meters
static double POLYANGLETRI = 60;
static double POLYANGLELIGHT = 150;
static double POLYANGLESQUARE = 90;
    /*
        canonical axis

    */
    /*
        values of each type
        0 = earth
        1 = fire
        2 = lightning
        3 = water
        4 = wind
    */
static short EARTH = 0;
static short FIRE = 1;
static short LIGHTNING = 2;
static short WATER = 3;
static short WIND = 4;
static short NOTCIRCLE = 5;
static short ERRORVAL = -1;
/*
    struct
*/
typedef struct runeClassStruct
{
    double init_heading;
    double init_roll;
    double init_pitch;
    bool isCircle;
    bool isLightning;
} runeClassDataAlias;

typedef struct spellQueueStruct
{
    struct spellQueueStruct *next;
    /*
        spellType
        0 = earth
        1 = fire
        2 = lightning
        3 = water
        4 = wind
        other = various errors
    */
    short spellType;
} spellQueueAlias;

/*
    global variables
*/
    /*
        stores current spell type
        0 = earth
        1 = fire
        2 = lightning
        3 = water
        4 = wind
        other = various errors
    */
extern double currTotalLength;
extern short currSpellType;
extern bool isCasting;
    /*
        we can use pointers for these as they start as NULL
    */
extern spellQueueAlias *spellQueueStart; // other files read spells from this guy
extern spellQueueAlias *spellQueueEnd; // end of queue
/*
    we can't use pointers here because we
*/
extern runeClassDataAlias runeClassData; // other files access this global struct
extern struct bnoeul initEulStruct; // stores initial orientation
extern struct bnoeul currEulStruct; // stores current orientation
extern struct bnolin currLinStruct; // stores current velocity

/*
    function declarations
*/
extern int main();
extern short checkCircle();
extern bool checkLightning(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern short classifyShape(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);

extern void initializeImu();

extern bool isValidSpell(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern bool hasValidLength(short spellType, double drawLen);

    /*
        Functions modifying global spell queue
    */
extern void enqueueSpell(short spell);
extern short dequeueSpell(); // returns -1 on error
extern void initQueue(); // initialize spellQueueStart
