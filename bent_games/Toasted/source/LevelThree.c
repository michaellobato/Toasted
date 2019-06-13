/*! ************************************************************************
  \file LevelThree.C
  \author Chris Howard
  \par email: choward@digipen.edu
  \par Game: Toasted
  \date Febuary 21st, 2007
  \brief

	Contains all the actions for level five, specifically the main boss
    
***************************************************************************/

#include "LevelThree.h"
#include "timer.h"
#include "player.h"
#include "effects.h"
#include "EnemyManager.h"
#include "turret.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>



void loadRail(int xPos, int yPos)
{

  srand(clock());
  railgunner.stats = malloc(sizeof(ENEMY));
  railgunner.stats->lastTime = 0;
  railgunner.stats->health = 500;
  railgunner.alive = 1;
  railgunner.counter = 0;
  railgunner.stats->posX = xPos;
  railgunner.stats->posY = yPos;
  railgunner.Xpos1 = xPos - 5;
  railgunner.Xpos2 = xPos + 5;
  railgunner.vecX1 = 0;
  railgunner.vecY1 = 1;
  railgunner.vecX2 = 0;
  railgunner.vecY2 = 1;
  railgunner.info = malloc(sizeof(ENEMY));
  railgunner.info->Attributes = (BACKGROUND_RED | FOREGROUND_RED | 
                                FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

  railgunner.stats->currentGun = gunCopy(&weaponRocketLauncher);
	railgunner.stats->currentGun->owner = OWNER_ENEMY;
	railgunner.stats->currentGun->range = 1400;
	railgunner.stats->currentGun->damage = 15;

  railgunner.othrgun = gunCopy(&weaponRocketLauncher);
	railgunner.othrgun->owner = OWNER_ENEMY;
	railgunner.othrgun->range = 1400;
	railgunner.othrgun->damage = 15;

  railgunner.guns = createLevelPosition(railgunner.stats, POSITION_WEAPON, '?');
  railgunner.rail = createLevelPosition(railgunner.stats, POSITION_WALL, '?');
  railgunner.stats->enemy_pos = createLevelPosition(railgunner.stats, POSITION_ENEMY, '?');
}

void loadWarper(int xPos, int yPos)
{

  srand(clock());
  warper.stats = malloc(sizeof(ENEMY));
  warper.stats->lastTime = 0;
  warper.stats->health = 500;
  warper.alive = 1;
  warper.state = 1;
  warper.counter = 0;
  warper.Xpos = xPos;
  warper.Ypos = yPos;
  warper.vecX = 0;
  warper.vecY = 1;
  warper.drawI = createLevelPosition(warper.stats, POSITION_ENEMY, ',');
  warper.drawV = createLevelPosition(warper.stats, POSITION_ENEMY, '+');
  warper.stats->enemy_pos = warper.drawV;
}

void getTraj()
{
  double x = getPlayerX() - warper.Xpos;
  double y = getPlayerY() - warper.Ypos;

  warper.vecX = (x/sqrt(x*x + y*y));
  warper.vecY = (y/sqrt(x*x + y*y));
  warper.vecX += (rand()%200 - 100) / 50;
  warper.vecY += (rand()%200 - 100) / 50;
  warper.vecX = (warper.vecX/sqrt(warper.vecX*warper.vecX + warper.vecY*warper.vecY));
  warper.vecY = (warper.vecY/sqrt(warper.vecX*warper.vecX + warper.vecY*warper.vecY));
}

void levelThree()
{
	/* edit by Sheridan - make sure we're in the room with the gunner,
		so that there aren't constant gun noises and it won't kill 
		itself (as often?) */
	if(player.Y > 41 && player.Y < 97 && player.X > 474)
	{
		updateRailGunner();
	}
	/* edit by paul: */
	/* make sure the waypoint gets set correctly */
    if((getPlayerX() > 322) && (getPlayerY() > 116) && (warper.alive != 0))
    {
		/* set the boss waypoint */
		player.atLevelThreeBoss = 1;
        updateWarper();
    }
}

void drawGunner()
{
    int i = -3, j, absi, absj;
    while (!isLevelPositionWall(WALL_LAYER, railgunner.stats->posX + i, railgunner.stats->posY))
    {
        setLevelPosition(WALL_LAYER, railgunner.stats->posX + i, railgunner.stats->posY, railgunner.rail);
        if((railgunner.stats->posX + i - 1) == railgunner.Xpos1)
        {
            i--;
        }
        i--;

    }
    i = 3;
    while (!isLevelPositionWall(WALL_LAYER, railgunner.stats->posX + i, railgunner.stats->posY))
    {
        setLevelPosition(WALL_LAYER, railgunner.stats->posX + i, railgunner.stats->posY, railgunner.rail);
        if((railgunner.stats->posX + i + 1) == railgunner.Xpos2)
        {
            i++;
        }
        i++;
    }
    setLevelPosition(WEAPON_LAYER, (int)railgunner.Xpos1, (int)railgunner.stats->posY, railgunner.guns);
    setLevelPosition(WEAPON_LAYER, (int)railgunner.Xpos2, (int)railgunner.stats->posY, railgunner.guns);
    for(i = -2; i <= 2; i++)
    {
      for(j = -1; j <= 1; j++)
      {
        if(i < 0)
        {
            absi = i * -1;
        }
        else
        {
            absi = i;
        }
        if(j < 0)
        {
            absj = j * -1;
        }
        else
        {
            absj = j;
        }
        if((absi + absj) < 3)
        {
            setLevelPosition(ENEMY_LAYER, railgunner.stats->posX+i, 
                             railgunner.stats->posY + j, railgunner.stats->enemy_pos);
        }
      }
    }
}
void clearlvl3()
{
    free(railgunner.stats);
    free(railgunner.info);
	/* edit by Paul and Sheridan: delete turrets */
	  turret_delete_list();
}
void clearGunner()
{
    int i, j;
    for(i = -63; i < 62; i++)
    {
        for(j = 1; j > -2; j--)
        {
            setLevelPosition(ENEMY_LAYER, railgunner.stats->posX+i, 
                             railgunner.stats->posY + j, whiteSpacePosition);
            setLevelPosition(WALL_LAYER, railgunner.stats->posX+i, 
                             railgunner.stats->posY + j, whiteSpacePosition);
            setLevelPosition(WEAPON_LAYER, railgunner.stats->posX+i, 
                             railgunner.stats->posY + j, whiteSpacePosition);
        }
    }
}
void updateRailGunner()
{
    double trajX, trajY;
    static int lookhealth;

	/* edit by Paul: if the railgunner is dead */
  if(railgunner.stats->health <= 0)
  {
      int i;
		/* create and show the weapon */
		/* drop the weapon */
	  if(!player.weaponsEnabled[GUN_HEAVYMACHINEGUN])
    {
			  LEVEL_POSITION* weapon_position = createLevelPosition(NULL, POSITION_WEAPON, '(');
		    setLevelPosition(PICKUP_LAYER, (int)railgunner.stats->posX, (int)railgunner.stats->posY, weapon_position);
	  }
    for(i = 485; i < 607; i += 5)
    {   
        explosion(i, 95, 500, OWNER_PLAYER);  
    }
    railgunner.alive = 0;
    clearGunner();
    railgunner.stats->health = 1;
  }

  if(((getCurrentTime() - railgunner.lastMoveTime) >= 50) && railgunner.alive)
  {
    if(lookhealth != railgunner.stats->health)
    {
        railgunner.info->Attributes = (BACKGROUND_RED | FOREGROUND_RED | 
                                FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        lookhealth = railgunner.stats->health;

    }
    else
    {
        railgunner.info->Attributes = (FOREGROUND_RED | BACKGROUND_RED | 
                                BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    }
        railgunner.counter++;
        clearGunner();

        if(getLevelPosition(WALL_LAYER, (int)railgunner.Xpos1 - 1, (int)railgunner.stats->posY)->character == 'X')
        {
            railgunner.Xpos1 += 2; 
        }
        else if((railgunner.Xpos1 + 1) > (railgunner.stats->posX-3))
        {
            railgunner.Xpos1 -= 2;
        }
        else
        {
            railgunner.Xpos1 += rand()%3 - 1;
        }
        if(getLevelPosition(WALL_LAYER, (int)railgunner.Xpos2 + 1, (int)railgunner.stats->posY)->character == 'X')
        {
            railgunner.Xpos2 -= 2; 
        }
        else if((railgunner.Xpos2 - 1) < (railgunner.stats->posX+3))
        {
            railgunner.Xpos2 += 2;
        }
        else
        {
            railgunner.Xpos2 += rand()%3 - 1;
        }

        trajY = getPlayerY() - railgunner.stats->posY;
        if((railgunner.counter%2) == 0)
        {
            trajX = (getPlayerX() - railgunner.Xpos1)+rand()%3-1;
            shoot(railgunner.stats->currentGun,(float)trajX*.03f, (float)trajY*.03f,
                                           (int)railgunner.Xpos1,(int)railgunner.stats->posY);
        }
        else
        {

            trajX = (getPlayerX() - railgunner.Xpos2)+rand()%3-1;
            shoot(railgunner.othrgun,(float)trajX*.03f, (float)trajY*.03f,
                                           (int)railgunner.Xpos2,(int)railgunner.stats->posY);
        }


        drawGunner();
        railgunner.lastMoveTime = (int)getCurrentTime();
    }
}
void updateWarper()
{
  static int timeaction = 0;
  static int numtimesdone = 0;
  static int lookhealth = 0;
  if(warper.stats->health <=0)
  {
    setLevelPosition(ENEMY_LAYER, (int)warper.Xpos, (int)warper.Ypos, whiteSpacePosition);
    clearLevelPositions(WALL_LAYER, 'R');

	/* edit by paul: */
	/* check so the boss doesn't drop the gun twice */
	if(!player.weaponsEnabled[GUN_RIFLE])
	{
		LEVEL_POSITION* weapon_position = createLevelPosition(NULL, POSITION_WEAPON, '"');
	    setLevelPosition(PICKUP_LAYER, (int)warper.Xpos, (int)warper.Ypos, weapon_position);
	}

	player.levelThreeComplete = 1;
    warper.state = 1;
    warper.stats->health = 1;
    warper.alive = 0;
	
	/* remove waypoint */
	player.atLevelThreeBoss = 0;

  }
  if((lookhealth != warper.stats->health) && (warper.state != 1)&& (warper.state != 5) && (warper.counter > 1))
  {
     lookhealth = warper.stats->health;
     warper.counter = 0;
     timeaction =  rand()%120+100;
     warper.state = 5;
  }
  if(((getCurrentTime() - warper.lastMoveTime) >= 10) && (warper.alive))
  {
    warper.counter++;
    switch(warper.state)
    {
    case 1:
      getTraj();
      warper.counter = 0;
      warper.state = 2;
      timeaction =  rand()%5 + 6;
      warper.stats->enemy_pos = warper.drawI;
      break;
    case 2:

      if(warper.counter > timeaction)
      {
        numtimesdone++;
        warper.state = 1;
      }
      if(numtimesdone > 5)
      {
        timeaction = rand()%120+40;
        warper.counter = 0;
        warper.state = 4;
        numtimesdone = 0;
        warper.stats->enemy_pos = warper.drawV;
      }
      break;
    case 3:
      warper.vecX = 0;
      warper.vecY = 0;
      if(warper.counter > timeaction)
      {
        warper.state = 1;
      }
      break;
    case 4:
      turret_create((int)warper.Xpos, (int)warper.Ypos, 100, 5000, 1);
      warper.state = 3;
      break;
    case 5:

      if(warper.counter > timeaction)
      {
        warper.state = 1;
      }
      warper.stats->enemy_pos = warper.drawI;
      break;
    }

    warper.lastMoveTime = (int)getCurrentTime();
  
    if(warper.state == 5)
    {

          int randnum = rand()%10;
          if(randnum > 7)
          {
             warper.stats->enemy_pos = warper.drawI;
          }
          else
          {
             warper.stats->enemy_pos = warper.drawV;
          }
        
    }
    if((!isLevelPositionWall(WALL_LAYER, (int)(warper.Xpos + warper.vecX), (int)(warper.Ypos + warper.vecY)))
        && (warper.alive))
    {
        setLevelPosition(ENEMY_LAYER, (int)warper.Xpos, (int)warper.Ypos, whiteSpacePosition);
        warper.Xpos += warper.vecX;
        warper.Ypos += warper.vecY;
        setLevelPosition(ENEMY_LAYER, (int)warper.Xpos, (int)warper.Ypos,warper.stats->enemy_pos);
    }
  
  }
  warper.stats->lastTime = (int)getCurrentTime();
}

void deleteWarper()
{
  free(warper.drawI);
  free(warper.drawV);
  free(warper.stats);
}
