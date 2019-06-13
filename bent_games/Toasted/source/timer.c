/*! ************************************************************************
  \file timer.c
  \author Sheridan Beebe
  \par email: sbeebe@digipen.edu
  \par Game: Toasted
  \date Feburuary 1st, 2007
  \brief  Some modified functions to get the time in game
***************************************************************************/
#include "timer.h"

/* get the time in miliseconds. */



double getCurrentTime()
{
	return currentTime;
}

void loadTimers()
{
	currentTime = (double)timeGetTime()*1.0;
	lastTime = currentTime;
	timerPaused = 0;
}

void updateTimers()
{
	if(!timerPaused)
	{
		currentTime += ((double)timeGetTime()*1.0) - lastTime;
	}
	lastTime = (double)timeGetTime()*1.0;
}

void pauseTimer(int pause)
{
	timerPaused = pause;
}
