#include <stdbool.h>
#include <math.h>
#include "spellQueueStruct.h"
#include "playerstruct.h"
#include "HandlePlayerDataFunctions.h"
#include "getbno055.h"

/*
    global constants
*/
static double FRAMEWAITTIME = 0.01; // time in seconds
static double POLYWAITTIME = 0.1; // time in seconds;
static double LIGHTNINGWAITTIME = 0.1;
static double MAXCLASSTIME = 0.2;
static double MAXPOLYDEV = 20; // angle in degrees
static double ANGLELIGHT = 20;
static double MAXCIRCLE = 90;
static double ANGLEFIRE = 20; // degrees
    /*
        angle tolerances in degrees used when there should be no movement in that direction
    */
static double ANGLETOLHEAD = 30;
static double ANGLETOLROLL = 30;
static double ANGLETOLPITCH = 30;
static double ANGLETOLLIGHT = 15;
    /*
        gravity direction
    */
static short GRAVX = 0;
static short GRAVY = 1;
static short GRAVZ = 2;
static short GRAVUNKNOWN = 3;
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
static short WATER = 0;
static short FIRE = 1;
static short LIGHTNING = 2;
static short EARTH = 3;
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
// double currTotalLength;
// short currSpellType;
// bool isCasting;
extern spellQueueAlias *spellQueueStart; // other files read spells from this guy
extern spellQueueAlias *spellQueueEnd; // end of queue
/*
*/
extern struct bnoeul *initEulPtr; // stores initial orientation


// struct bnolin *bnolinptr
/*
    function declarations
*/
extern int main();
extern short checkCircle(struct bnoeul*, struct bnoeul*, struct bnogra*);
extern bool checkLightning(struct bnoeul*, struct bnoeul*, struct bnogra*);
extern bool checkFire(struct bnoeul*, struct bnoeul*, struct bnogra*);
extern short classifyShape();

extern void initializeImu();

extern bool isValidSpell(struct bnoeul*, struct bnolin*);
extern bool hasValidLength(short, double);

extern short checkGravDir(struct bnogra*);

    /*
        Functions modifying global spell queue
    */
extern void enqueueSpell(short);
extern short dequeueSpell(); // returns -1 on error
extern void initQueue(); // initialize spellQueueStart

    /*
        function do convert angle differences
    */
double angDiffWrap(double, double);


int imuMain(struct PlayerStaffData*);
