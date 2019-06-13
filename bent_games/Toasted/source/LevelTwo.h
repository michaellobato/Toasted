/*******************************************************************************
  Author: Paul Ewers, Cullen McElroy
  Game:	  Toasted
  File:	  LevelTwo.h
  Date:	  February 17th, 2007
  
  Description:
          This header file has everything we need for LevelTwo.c
*******************************************************************************/

#ifndef LEVEL_TWO_H
#define LEVEL_TWO_H

#include "LevelManager.h"
#include "EnemyManager.h"
#include "timer.h"

/* this is the boss struct that will hold all the info for the level two boss */
struct levelTwoBoss
{
	LEVEL_POSITION *Position;
	ENEMY *stats;
	GUN* currentGun;
	int Xpos;
	int Ypos;
	int alive;
	int circleMode;
	double lastTime;
	double interval;
	int hpholder;
    double startflashing;
    int flashing;
} levelTwoBoss;
	
/* this is the miniboss struct that will hold all the info for the level two miniboss */
struct levelTwoMiniBoss
{
  LEVEL_POSITION *Position;
  ENEMY *stats;
  GUN* currentGun;
  int Xpos;
  int Ypos;
  int alive;
  int circleMode;
  double lastTime;
  double interval;
  int hpholder;
  double startflashing;
  int flashing;
} levelTwoMiniBoss;

/* *** prototypes *** */

void updateLevelTwoMiniBoss();
void updateLevelTwoBoss();

void loadLevelTwoMiniBoss(int xPos, int yPos);
void loadLevelTwoBoss(int xPos, int yPos);

void deleteLevelTwoMiniBoss();
void deleteLevelTwoBoss();

#endif
