#include <stdbool.h>
#include <math.h>

/*
    global constants
*/
static double FRAMEWAITTIME = 0.0001; // time in seconds
static double POLYWAITTIME = 0.1; // time in seconds;
static double LIGHTNINGWAITTIME = 0.5;
static double MAXPOLYDEV = 30; // angle in degrees
static double ANGLELIGHT = 20;
    /*
        angle tolerances in degrees used when there should be no movement in that direction
    */
//static double ANGLETOLHEAD = 30;
//static double ANGLETOLROLL = 30;
static double ANGLETOLPITCH = 30;
static double ANGLETOLLIGHT = 15;
    /*
        multiplier used when movement exists in that  direction
    */
//static double ANGLETOLMULTIPLIER = 0.25; // y = a*(1+x)
static double MINDRAWLEN = 1.8; // in meters
static double MINDRAWLENLIGHT = 0.3; // in meters
//static double POLYANGLETRI = 60;
//static double POLYANGLELIGHT = 150;
//static double POLYANGLESQUARE = 90;
    
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

// struct bnolin *bnolinptr
/*
    function declarations
*/
extern int main();
extern short checkCircle(struct bnoeul *starteulptr, struct bnoeul *curreulptr);
extern bool checkLightning(struct bnoeul *starteulptr, struct bnoeul *curreulptr);
extern bool checkFire(struct bnoeul *starteulptr, struct bnoeul *curreulptr);
extern short classifyShape();

extern void initializeImu();

extern bool isValidSpell(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern bool hasValidLength(short spellType, double drawLen);

    /*
        Functions modifying global spell queue
    */
extern void enqueueSpell(short spell);
extern short dequeueSpell(); // returns -1 on error
extern void initQueue(); // initialize spellQueueStart

    /*
        function do convert angle differences
    */
double angDiffWrap(double angle1, double angle2);

int imuMain();