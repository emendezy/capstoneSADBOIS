/* Handle Player Data - function header */

#include "GPIOHandler.h"

struct PlayerStaffData* initPlayerStruct(bool*);

void unloadPlayerData(struct PlayerStaffData*);



bool isCurrCasting(struct PlayerStaffData*);

bool isDoneCasting(struct PlayerStaffData*);

int wasAttacked(struct PlayerStaffData*);

void imuInputHandler(struct PlayerStaffData*);

void updatePlayerFields(struct PlayerStaffData*);

void attackHandler(struct PlayerStaffData*, int);

void spellCaster(struct PlayerStaffData*, int);

void healPlayer(struct PlayerStaffData*);

void handleBurning(struct PlayerStaffData*);

void editCoolDownValues(struct PlayerStaffData*, int);

void updateCooldownLightsOnStaff(int, int);

void checkWeakness(struct PlayerStaffData*);

void checkShield(struct PlayerStaffData*);

void checkImmunity(struct PlayerStaffData*);

void sendCast(struct PlayerStaffData*);

void sendDamagePackage(int*);

int calcSendingSpellDamage(struct PlayerStaffData*, int);

int calcShieldAffect(int, int);

void processDamageRecieved(struct PlayerStaffData*, int*);

void endCasting(struct PlayerStaffData*, bool);

void rumbleHandler(struct PlayerStaffData*, int);

void alternateRumble(int);

void lightHandler(struct PlayerStaffData*, int);

void soundHandler(struct PlayerStaffData*, int);
