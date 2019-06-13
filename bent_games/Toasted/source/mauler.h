/*! ************************************************************************
  \file mauler.h
  \author Chris Howard
  \par email: choward@digipen.edu
  \par Game: Toasted
  \date Feburuary 2nd, 2007
  \brief  the header file for the mauler.c file
***************************************************************************/

#ifndef MAULER_H
#define MAULER_H

#include "LevelManager.h"
#include "EnemyManager.h"

void createMauler(int x, int y);

void moveMauler();
typedef struct 
{
    float X;
    float Y;
}CORD;

#endif
