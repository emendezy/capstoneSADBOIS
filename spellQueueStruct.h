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