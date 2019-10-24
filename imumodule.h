/*
    global constant
*/
static unsigned POLYWAITTIME = 20; // time in ms must be unsigned to be passed to sleep()
static double MAXPOLYDEV = 30; // angle in degrees
static double ANGLETOLMULTIPLIER = 0.25; // y = a*(1+x)
static double MINDRAWLEN = 1.8; // in meters
static double MINDRAWLENLIGHT = 0.3; // in meters

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

    */
    short spellType;
} spellQueueAlias;

/*
    global variable
*/
extern runeClassDataAlias runeClassData; // other files access this global struct
extern spellQueueAlias *spellQueueStart; // other files read spells from this guy
extern spellQueueAlias *spellQueueEnd; // end of queue
/*
    function declarations
*/
extern int main();
extern bool checkCircle(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern bool checkLightning(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern double getVelocity(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern void calibrateImu(struct bnoeul *bnoeulptr);
extern bool isValidSpell(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern void enqueueSpell(short spell);
extern short dequeueSpell(); // returns -1 on error
extern void initQueue(); // initialize spellQueueStart