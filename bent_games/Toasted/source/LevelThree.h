#ifndef LEVELTHREE_H
#define LEVELTHREE_H

#include "LevelThree.h"
#include "timer.h"
#include "player.h"
#include "effects.h"
#include "EnemyManager.h"
#include "turret.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
struct warper
{
  LEVEL_POSITION *drawI;
  LEVEL_POSITION *drawV;
  ENEMY *stats;
  int alive;
  int state;
  int counter;
  int lastMoveTime;
  double Xpos;
  double Ypos;
  double vecX;
  double vecY;
}warper;

struct railgunner
{
  LEVEL_POSITION *guns;
  LEVEL_POSITION *rail;
  CHAR_INFO *info;
  ENEMY *stats;
  GUN *othrgun;
  int alive;
  int counter;
  int lastMoveTime;
  double Xpos1;
  double Xpos2;
  double Ypos;
  double vecX1;
  double vecY1;
  double vecX2;
  double vecY2;
}railgunner;

void clearlvl3();
void updateWarper();
void deleteWarper();
void loadWarper(int xPos, int yPos);
void updateRailGunner();
void levelThree();
void loadRail(int xPos, int yPos);
#endif
