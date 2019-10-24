/*
    global constant
*/
static unsigned FRAMEWAITTIME = 10;
static unsigned POLYWAITTIME = 20; // time in ms must be unsigned to be passed to sleep()
static double MAXPOLYDEV = 30; // angle in degrees
    /*
        angle tolerances in degrees used when there should be no movement in that direction
    */
static double ANGLETOLHEAD = 15;;
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
extern bool checkCircle(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern bool checkLightning(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern short classifyShape(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern double getVelocity(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern void calibrateImu();
extern bool isValidSpell(struct bnoeul *bnoeulptr, struct bnolin *bnolinptr);
extern void enqueueSpell(short spell);
extern short dequeueSpell(); // returns -1 on error
extern void initQueue(); // initialize spellQueueStart