/*! ************************************************************************
  \file timer.h
  \author Sheridan Beebe
  \par email: sbeebe@digipen.edu
  \par Game: Toasted
  \date Feburuary 1st, 2007
  \brief  the header file for the time.c file
***************************************************************************/

#ifndef TIME_H
#define TIME_H

#include <time.h>
#include <windows.h>

typedef struct 
{
    int count;
}TIMER;

void loadTimers();
double getCurrentTime();
void updateTimers();
void pauseTimer(int pause);

void setTime(TIMER *time);

int getCurrentTimeI(TIMER *time);

int timerPaused;
double currentTime;
double lastTime;

#endif
