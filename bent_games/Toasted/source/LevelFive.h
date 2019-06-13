/*! ************************************************************************
  \file LevelFive.h
  \author Chris Howard
  \par email: choward@digipen.edu
  \par Game: Toasted
  \date Febuary 14th, 2007
  \brief

	Header for level Five
    
***************************************************************************/

#ifndef LEVEL_FIVE_H
#define LEVEL_FIVE_H

#include "LevelManager.h"
#include "EnemyManager.h"
#include "windows.h"

#define BOSS_HEIGHT	28
#define BOSS_WIDTH 70
#define MOVE_DELAY_MAINBOSS 30


void loadMainBoss(int xPosition, int yPosition);
void updateMainBoss();
void drawMainBoss(ENEMY *coords, CHAR_INFO inf[BOSS_HEIGHT][BOSS_WIDTH], LEVEL_POSITION * draw, LEVEL_POSITION * drawE);
void clearMainBoss(ENEMY *coords, CHAR_INFO inf[BOSS_HEIGHT][BOSS_WIDTH]);
void loadMainBossChars(char *fileName, char *colorName,  CHAR_INFO cha[BOSS_HEIGHT][BOSS_WIDTH]);
CHAR_INFO *getMainBossChar(int x, int y);
int mainbossX();
int mainbossY();
int LarmX();
int LarmY();
int RarmX();
int RarmY();
int LhandX();
int LhandY();
int RhandX();
int RhandY();
int LVL_5(int state);

CHAR_INFO *getRarmChar(int x, int y);
CHAR_INFO *getLarmChar(int x, int y);
CHAR_INFO *getLhandChar(int x, int y);
CHAR_INFO *getRhandChar(int x, int y);
void LarmFindPlyr();
void RarmFindPlyr();
void LhandGetPos();
void RhandGetPos();

#endif
