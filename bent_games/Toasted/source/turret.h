/*! ************************************************************************
  \file turret.c
  \author Chris Howard
  \par email: choward@digipen.edu
  \par Game: Toasted
  \date Feburuary 6th, 2007
  \brief  contains all of the definitions for Turret.c.
***************************************************************************/

#ifndef TURRET_H
#define TURRET_H

#include "LevelManager.h"
#include "EnemyManager.h"
#include "timer.h"

typedef struct
{
  ENEMY* stats;
  int Xpos;
  int Ypos; 
  int flag;
  float Xvec;
  int range;
  float Yvec;
} TURRET;

typedef struct turret_linklist
{
  TURRET* data;
	struct turret_linklist* next;
} TURRET_LINKLIST;

TURRET_LINKLIST* turret_head;

void turret_init();
void turret_fire();

/* edit by Paul */
void turret_delete_list();
TURRET_LINKLIST* turret_create (int x, int y, int hp, int rng, int flag);

#endif
