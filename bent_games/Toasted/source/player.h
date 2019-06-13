/*! ************************************************************************
  \file LevelManager.h
  \author Chris Howard, Sheridan Beebe
  \par email: choward@digipen.edu, sbeebe@digipen.edu
  \par Game: Toasted
  \date Febuary 6th, 2007
  \brief

	This is the header file for player.c. It defines the structs and
	functions that will be used for the player.
    
***************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>
#include "LevelManager.h"
#include "WeaponManager.h"
#include "MenuSystem.h"

struct S_PLAYER
{
	int X;
	int Y;
	int hp;
  int hpholder;
  int timer;
  CHAR_INFO color;

  /*Mike: file this player is saved to */
	char saveFile[MAX_SAVE_LENGTH + 10];
	char name[51];

	/* store which levels have been beaten */
	int levelOneComplete;
	int levelTwoComplete;
	int levelThreeComplete;
	int levelFourComplete;
	int levelFiveComplete;

	/* store which weapons are picked up */
	int singlePistolEnabled;
	int doublePistolEnabled;
	int spreadGunEnabled;
	int scatterGunEnabled;
	int subMachineGunEnabled;
	int heavyMachineGunEnabled;
	int rifleEnabled;
	int sniperEnabled;
	int rocketLauncherEnabled;
	int omegaLaserEnabled;

	/* store which weapons are picked up in an array */
	int weaponsEnabled[WEAPON_COUNT];

	/* store the current weapon ID */
	WEAPON_ID currentWeapon;

	int lastMoveTime;
	int moveSpeed;
    int stopped;
	int lastWeaponSwitchTime;
	int weaponSwitchTime;
	float lastDirectionX;
	float lastDirectionY;

	/* store boss check points */
	int atLevelOneBoss;
	int atLevelTwoBoss;
	int atLevelThreeBoss;
	int atLevelFourBoss;
	int atLevelFiveBoss;

};

struct S_PLAYER player;

void plyrMvLeft();
void plyrMvRight();
void plyrMvUp();
void plyrMvDown();
int getPlayerX();
int getPlayerY();
void setPlayerX(int x);
void setPlayerY(int y);
void* getPlayer();
char getPlyrCollidedWith();

void loadPlayerData(char *fileName, struct S_PLAYER *sPlayer);
void savePlayerData(char *fileName, struct S_PLAYER *sPlayer);
void loadPlayerVar(char *varName, char *varValue, struct S_PLAYER *sPlayer);
void savePlayerVarInt(char *varName, int varValue, FILE *fileOut);
void savePlayerVarString(char *varName, char *varValue, FILE *fileOut);

void updatePlayer();

void nextWeapon(int);

void myItoa(int value, char *valueString);

#endif
