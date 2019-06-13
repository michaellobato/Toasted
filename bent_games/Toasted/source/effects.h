/*! ************************************************************************
  \file effects.h
  \author Chris Howard
  \par email: choward@digipen.edu
  \par Game: Toasted
  \date Feburuary 10th, 2007
  \brief  the header file for the effects.c file
***************************************************************************/

#ifndef EFFECTS_H
#define EFFECTS_H

#include "LevelManager.h"
#include "WeaponManager.h"
#include "timer.h"

typedef struct
{
  CHAR_INFO charinfo;
  int currchar;
  int count;
  float rand;
  float lastMove;
  int switcher;
  float trajX;
  float trajY;
  float Xpos;
  float Ypos;
  float pwr;
  int life;
  int flag;
  double time;
  double tmr;
  LEVEL_POSITION *appear;
  E_OWNER owner;
}EXPLOSION;

typedef struct explosion_linklist
{
  EXPLOSION* data;
	struct explosion_linklist* next;
} EXPLOSION_LINKLIST;

EXPLOSION_LINKLIST *exp_head;

void explosion (int x, int y, int l, E_OWNER owner);
CHAR_INFO expl_anim(LEVEL_POSITION* exp);
void expl_init();
EXPLOSION_LINKLIST* expl_create (int x, int y, float tx, float ty, int l, int flag, WORD color, E_OWNER owner);
EXPLOSION_LINKLIST* expl_delete(EXPLOSION_LINKLIST *targ);
void expl_update();
void expl_move(EXPLOSION_LINKLIST *mover);
void expl_destroy();
void setRand(EXPLOSION_LINKLIST * exp, float r);

#endif
