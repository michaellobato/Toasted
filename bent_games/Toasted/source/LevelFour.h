/*! ************************************************************************
  \file LevelFour.h
  \author Sheridan Beebe
  \par email: sbeebe@digipen.edu
  \par Game: Toasted
  \date Febuary 12th, 2007
  \brief

	This is the header file for LevelFour.c. It holds functions used
	specificaly for level four, for such things as bosses and special wall
	switching stuff.
    
***************************************************************************/

#ifndef LEVEL_FOUR_H
#define LEVEL_FOUR_H

#include "LevelManager.h"
#include "EnemyManager.h"
#include "WeaponManager.h"
#include "windows.h"

#define TANK_HEIGHT	15
#define TANK_WIDTH 20
#define NUMBER_OF_FRAMES 3
#define FRAME_DELAY 500
#define MOVE_DELAY 250
#define ST_ACCEL 50
#define ST_DAMAGE 4
#define FLASH_TIME 1000

#define FSB_HEIGHT 4
#define FSB_WIDTH 7
#define FSB_FRAME_COUNT 4
#define FSB_FRAME_DELAY 100
#define FSB_MOVE_DELAY 300

enum E_SUPERTANK_STATE {
	SUPERTANK_NOTHING,
	SUPERTANK_SIT,
	SUPERTANK_SPAWN,
	SUPERTANK_WAITING,
	SUPERTANK_RAM_FORWARD, 
	SUPERTANK_RAM_RIGHT,
	SUPERTANK_RAM_BACK,
	SUPERTANK_RAM_LEFT
	};
typedef enum E_SUPERTANK_STATE SUPERTANK_STATE;

enum E_FSB_STATE {
	FSB_NOTHING,
	FSB_ATTACKING,
	FSB_SITTING
	};
typedef enum E_FSB_STATE FSB_STATE;

struct SuperTank
{
	int xPosition;
	int yPosition;
	int startingX;
	int startingY;
	LEVEL_POSITION *superTankPosition;
	ENEMY *enemyStats;
	GUN *currentGun;
	CHAR_INFO frame[NUMBER_OF_FRAMES][TANK_HEIGHT][TANK_WIDTH];
	int alive;
	int lastFrameTime;
	int lastMoveTime;
	int currentFrame;
	SUPERTANK_STATE currentState;
	int animationPaused;
	int stateSwitchTimer;
	int lastTime;
	int lastSpawnTime;
	int spawnTimeLimit;
	int spawnCount;
	int spawnTotal;
	int ramWaitStartTime;
	int ramTotalWaitTime;
	int ramWaiting;
	int moveDelay;
	int lastHP;
	int flash;
	int flashTime;
};


struct FLYING_SLASH_BOT
{
	int xPosition;
	int yPosition;
	LEVEL_POSITION *fsbPosition;
	ENEMY *enemyStats;
	CHAR_INFO frame[FSB_FRAME_COUNT][FSB_HEIGHT][FSB_WIDTH];
	int alive;
	int lastFrameTime;
	int lastMoveTime;
	int currentFrame;
	SUPERTANK_STATE currentState;
	int animationPaused;
	int moveDelay;
	int lastHP;
	int flash;
	int flashTime;
	int lastTime;
};

struct SuperTank superTank;
typedef struct FLYING_SLASH_BOT FlyingSlashBot;
FlyingSlashBot *fsbOne;
FlyingSlashBot *fsbTwo;
FlyingSlashBot *fsbThree;
FlyingSlashBot *fsbFour;
int fsbCount;
int bossOpened;

void updateLevelFour();

void loadSuperTank(int xPosition, int yPosition);
FlyingSlashBot *createFSB(char fsbChar, int xPosition, int yPosition);
void updateSuperTank();
void updateFSB(FlyingSlashBot *fsb);
int checkFSBCollision(int x, int y);
int checkFSBCollisionPlayer(FlyingSlashBot *fsb);
void drawSuperTank();
void clearSuperTank();
void clearFSB(FlyingSlashBot *fsb);
void loadBossChars(char *fileName, char *colorName,  CHAR_INFO cha[TANK_HEIGHT][TANK_WIDTH]);
void loadFSBChars(char *fileName, char *colorName,  CHAR_INFO cha[FSB_HEIGHT][FSB_WIDTH]);
void drawFSB(FlyingSlashBot *fsb);
CHAR_INFO *getSuperTankChar(int x, int y);
CHAR_INFO *getFSBChar(FlyingSlashBot *fsb, int x, int y);
void spawnSubEnemy();
void superTankShoot();
void superTankRamRight();
void superTankRamLeft();
void superTankRamUp();
void superTankRamBack();
void superTankMoveToCenter();
int checkCollisionRight(int newX, int newY);
int checkCollisionLeft(int newX, int newY);
int checkCollisionUp(int newX, int newY);
int checkCollisionDown(int newX, int newY);
int superTankLostHealth();
void superTankFlash();

#endif
