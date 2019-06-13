/*******************************************************************************
  Author: Paul Ewers
  Game:	  Toasted
  File:	  LevelOne.h
  Date:	  February 17th, 2007
  
  Description:
          This header file has everything we need for LevelOne.c
*******************************************************************************/

#ifndef LEVEL_ONE_H
#define LEVEL_ONE_H

#include "LevelManager.h"
#include "EnemyManager.h"
#include "timer.h"

/* this is the boss struct that will hold all the info for the level one boss */
struct levelOneBoss
{
  LEVEL_POSITION *Position;
  ENEMY *stats;
  GUN* closeGun;
  GUN* rangedGun;
  int Xpos;
  int Ypos;
  int oldPlayerX;
  int oldPlayerY;
  int alive;
  int hpholder;
  double startflashing;
  int flashing;
} levelOneBoss;

/* this is the miniboss struct that will hold all the info for the level one miniboss */
struct levelOneMiniBoss
{
  LEVEL_POSITION *Position;
  ENEMY *stats;
  GUN* rangedGun;
  int Xpos;
  int Ypos;
  int alive;
  double lastTime;
  double interval;
  int hpholder;
  double startflashing;
  int flashing;
} levelOneMiniBoss;

/* used for the tutorial */
int spawnChecker;
int levelOneBossChecker;

/* *** prototypes *** */

void updateLevelOneBoss();
void updateLevelOneMiniBoss();

void loadLevelOneBoss(int xPos, int yPos);
void loadLevelOneMiniBoss(int xPos, int yPos);

void deleteLevelOneBoss();
void deleteLevelOneMiniBoss();

void updateLevelOne();

#endif
