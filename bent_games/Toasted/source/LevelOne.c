/*******************************************************************************
	Author: Paul Ewers
	Game:	  Toasted
	File:	  LevelOne.c
	Date:	  February 17th, 2007

	Description:
	This header file has everything we need for LevelOne.
*******************************************************************************/

#include "LevelOne.h"
#include "EnemyManager.h"
#include "WeaponManager.h"
#include "LevelManager.h"
#include "player.h"
#include "hud.h"

/*******************************************************************************
Function:    updateLevelOne

Description: This file handles all of the nescesary updates that level one,
             the tutorial level, needs.

Inputs:      None

Outputs:     None
*******************************************************************************/
void updateLevelOne()
{
  if (totalClawbotSpawns == 7 && spawnChecker == 1)
  {
	  setTextPrompt("You destroyed a spawnpoint! Now go find\nthe weapon upgrade and defeat the boss.");
	  spawnChecker = 0;
  }
  if ((player.Y < 27) && (levelOneBossChecker == 1))
  {
  	  setTextPrompt("Be careful, this guy looks dangerous!\nShould you ever die in a boss battle,\nyou'll restart at the boss until he\'s\ndead.");
	  levelOneBossChecker = 0;
  }
}

/*******************************************************************************
Function:    updateLevelOneBoss

Description: Handle all of the AI for the boss in level one.

Inputs:      None

Outputs:     None
*******************************************************************************/
void updateLevelOneBoss()
{
	double current = getCurrentTime();

	/* check to see if the boss took damage */
	if (levelOneBoss.stats->health < levelOneBoss.hpholder)
	{
		/* make him flash */
		levelOneBoss.flashing = 1;
		levelOneBoss.hpholder = levelOneBoss.stats->health;
		levelOneBoss.startflashing = current;
	}

	/* reset the boss after a set ammount of time */
	else if (levelOneBoss.stats->health >= levelOneBoss.hpholder && 
	        levelOneBoss.startflashing + 50 < current)
    {
		/* reset his flashing variable */
	    levelOneBoss.flashing = 0;
    }

	/* only update the boss if the player is in the correct map location */
	if ((player.Y < 27) && (levelOneBoss.alive != 0))
	{
		player.atLevelOneBoss = 1;

		/* if the boss is dead, delete him */
		if (levelOneBoss.stats->health <= 0)
		{
			/* create and show the weapon */
			/* drop the weapon */
			if(!player.weaponsEnabled[GUN_SPREADGUN])
			{
			    LEVEL_POSITION* weapon_position = createLevelPosition(NULL, POSITION_WEAPON, ']');
			    setLevelPosition(PICKUP_LAYER, levelOneBoss.Xpos, levelOneBoss.Ypos, weapon_position);
			}

			/* set flag for level one complete */
			player.levelOneComplete = 1;

			/* clear the walls so the player can leave the battle */
			clearLevelPositions(WALL_LAYER, 'S');

			/* set flag for boss dead and delete him */
			levelOneBoss.alive = 0;
			deleteLevelOneBoss();

			/* remove boss checkpoint */
			player.atLevelOneBoss = 0;
		}

		/* if the boss is not dead, update it */
		else
		{
			int newX = levelOneBoss.Xpos;
			int newY = levelOneBoss.Ypos;
			int shoot_distance_x = 30;
			int shoot_distance_y = 15;
			int shoot_distance_x_short = 9;
			int shoot_distance_y_short = 7;
			int shoot_padding = 2;

			/* *** movement *** */
			/* if the player moved right... */
			if (player.X - levelOneBoss.oldPlayerX == 1)
			{
				newX = levelOneBoss.Xpos - 1;
			}

			/* if the player moved left... */
			else if (player.X - levelOneBoss.oldPlayerX == -1)
			{
				newX = levelOneBoss.Xpos + 1;
			}

			/* if the player moved up... */
			if (player.Y - levelOneBoss.oldPlayerY == 1)
			{
				newY = levelOneBoss.Ypos - 1;
			}

			/* if the player moved down... */
			else if (player.Y - levelOneBoss.oldPlayerY == -1)
			{
				newY = levelOneBoss.Ypos + 1;
			}

			/* *** shooting (ranged gun) *** */
			/* if the enemy is to the RIGHT of the player (ranged gun) */
			if ((levelOneBoss.Xpos - player.X <= shoot_distance_x) && 
				(levelOneBoss.Xpos - player.X > shoot_distance_x_short) &&
				(abs(levelOneBoss.Ypos - player.Y) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelOneBoss.rangedGun, -1, 0, levelOneBoss.Xpos, levelOneBoss.Ypos);
			}

			/* if the enemy is to the LEFT of the player (ranged gun) */
			else if ((levelOneBoss.Xpos - player.X >= -shoot_distance_x) && 
				(levelOneBoss.Xpos - player.X < -shoot_distance_x_short) && 
				(abs(levelOneBoss.Ypos - player.Y) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelOneBoss.rangedGun, 1, 0, levelOneBoss.Xpos, levelOneBoss.Ypos);
			}

			/* if the enemy is ABOVE the player (ranged gun) */
			else if ((levelOneBoss.Ypos - player.Y >= -shoot_distance_y) && 
				(levelOneBoss.Ypos - player.Y < -shoot_distance_y_short) &&
				(abs(levelOneBoss.Xpos - player.X) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelOneBoss.rangedGun, 0, 1, levelOneBoss.Xpos, levelOneBoss.Ypos);
			}

			/* if the enemy is BELOW the player (ranged gun) */
			else if ((levelOneBoss.Ypos - player.Y <= shoot_distance_y) && 
				(levelOneBoss.Ypos - player.Y > shoot_distance_y_short) &&
				(abs(levelOneBoss.Xpos - player.X) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelOneBoss.rangedGun, 0, -1, levelOneBoss.Xpos, levelOneBoss.Ypos);
			}

			/* *** shooting (close gun) *** */
			/* if the enemy is to the RIGHT of the player (close gun) */
			if ((levelOneBoss.Xpos - player.X <= shoot_distance_x_short) && 
				(levelOneBoss.Xpos - player.X >= 0) &&
				(abs(levelOneBoss.Ypos - player.Y) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelOneBoss.closeGun, -1, 0, levelOneBoss.Xpos, levelOneBoss.Ypos);
			}

			/* if the enemy is to the LEFT of the player (close gun) */
			else if ((levelOneBoss.Xpos - player.X >= -shoot_distance_x_short) && 
				(levelOneBoss.Xpos - player.X <= 0) && 
				(abs(levelOneBoss.Ypos - player.Y) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelOneBoss.closeGun, 1, 0, levelOneBoss.Xpos, levelOneBoss.Ypos);
			}

			/* if the enemy is ABOVE the player (close gun) */
			if ((levelOneBoss.Ypos - player.Y <= shoot_distance_y_short) && 
				(levelOneBoss.Ypos - player.Y >= 0) &&
				(abs(levelOneBoss.Xpos - player.X) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelOneBoss.closeGun, 0, -1, levelOneBoss.Xpos, levelOneBoss.Ypos);
			}

			/* if the enemy is BELOW the player (close gun) */
			else if ((levelOneBoss.Ypos - player.Y >= -shoot_distance_y_short) && 
				(levelOneBoss.Ypos - player.Y <= 0) &&
				(abs(levelOneBoss.Xpos - player.X) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelOneBoss.closeGun, 0, 1, levelOneBoss.Xpos, levelOneBoss.Ypos);
			}

			/* *** update the level position *** */
			/* if the position is open, move there */
			if (isLevelPositionSpaceAll(newX, newY))
			{
				/* write a whitespace in the location where the enemy last was */
				setLevelPosition(ENEMY_LAYER, levelOneBoss.Xpos, levelOneBoss.Ypos, whiteSpacePosition);

				/* update the enemy struct with his new position */
				levelOneBoss.Xpos = newX;
				levelOneBoss.Ypos = newY;

				/* write the new enemy (with a new position) */
				setLevelPosition(ENEMY_LAYER, levelOneBoss.Xpos, levelOneBoss.Ypos, levelOneBoss.Position);
			}

			/* else, if the position isn't open */
			else
			{
				/* update the enemy struct with his new position */
				levelOneBoss.Xpos = levelOneBoss.Xpos;
				levelOneBoss.Ypos = levelOneBoss.Ypos;

				/* write the new enemy (with a new position) */
				setLevelPosition(ENEMY_LAYER, levelOneBoss.Xpos, levelOneBoss.Ypos, levelOneBoss.Position);
			}

			levelOneBoss.oldPlayerX = player.X;
			levelOneBoss.oldPlayerY = player.Y;
		} /* end update player */
	} /* end player in boss range */
}

/*******************************************************************************
Function:    updateLevelOneMiniBoss

Description: Handle all of the AI for the minibosses in level one.

Inputs:      None

Outputs:     None
*******************************************************************************/
void updateLevelOneMiniBoss()
{
	double current = getCurrentTime();

	/* check to see if the boss took damage */
	if (levelOneMiniBoss.stats->health < levelOneMiniBoss.hpholder)
	{
		/* make him flash */
		levelOneMiniBoss.flashing = 1;
		levelOneMiniBoss.hpholder = levelOneMiniBoss.stats->health;
		levelOneMiniBoss.startflashing = current;
	}

	/* reset the boss after a set ammount of time */
	else if (levelOneMiniBoss.stats->health >= levelOneMiniBoss.hpholder && 
	         levelOneMiniBoss.startflashing + 100 < current)
    {
		/* reset his flashing variable */
	    levelOneMiniBoss.flashing = 0;
    }

	/* only update the boss if the player is in the correct map location */
	if ((player.X >= 182) &&
		(player.X <= 248) &&
		(player.Y >= 98) &&
		(player.Y <= 109) && 
		(levelOneMiniBoss.alive != 0) && 
		(current - levelOneMiniBoss.lastTime >= levelOneMiniBoss.interval))
	{
		/* if the mini boss is dead, delete him */
		if (levelOneMiniBoss.stats->health <= 0)
		{
			setTextPrompt("You defeated the robot, but that was only\na miniboss. Now go defeat the final boss\ndirectly to the North.");
			
			/* open the gate / gate to the boss */
			clearLevelPositions(WALL_LAYER, 'G');
			clearLevelPositions(WALL_LAYER, 'V');

			/* set his flag to dead and delete him */
			levelOneMiniBoss.alive = 0;
			deleteLevelOneMiniBoss();
		}

		/* if the mini boss is not dead AND he is not touching the player, update it */
		else
		{
			int newX = levelOneMiniBoss.Xpos;
			int newY = levelOneMiniBoss.Ypos;

			/* damage the player if the enemy is adjacent the player */
			if ((abs(levelOneMiniBoss.Xpos - player.X) <= 1) && (abs(levelOneMiniBoss.Ypos - player.Y) <= 1))
			{
				player.hp -= levelOneMiniBoss.rangedGun->damage;
			}

			/* *** movement *** */
			/* Miniboss AI (best path) */
			if (player.X <= newX)
				newX = levelOneMiniBoss.Xpos - 1;

			else if (player.X >= newX)
				newX = levelOneMiniBoss.Xpos + 1;

			if (player.Y >= newY)
				newY = levelOneMiniBoss.Ypos + 1;

			else if (player.Y <= newY)
				newY = levelOneMiniBoss.Ypos - 1;

			/* *** shooting *** */
			/* if the enemy is to the LEFT of the player */
			if ((levelOneMiniBoss.Xpos - player.X >= -20) && 
				(levelOneMiniBoss.Xpos - player.X <= -2) &&
				(abs(levelOneMiniBoss.Ypos - player.Y) <= 3))
			{
				/* call shoot function */
				shoot(levelOneMiniBoss.rangedGun, 1, 0, levelOneMiniBoss.Xpos, levelOneMiniBoss.Ypos);
			}

			/* if the enemy is to the RIGHT of the player */
			if ((levelOneMiniBoss.Xpos - player.X <= 20) && 
				(levelOneMiniBoss.Xpos - player.X >= -2) &&
				(abs(levelOneMiniBoss.Ypos - player.Y) <= 3))
			{
				/* call shoot function */
				shoot(levelOneMiniBoss.rangedGun, -1, 0, levelOneMiniBoss.Xpos, levelOneMiniBoss.Ypos);
			}

			/* if the enemy is ABOVE the player */
			if ((levelOneMiniBoss.Ypos - player.Y >= -20) && 
				(levelOneMiniBoss.Ypos - player.Y <= -2) &&
				(abs(levelOneMiniBoss.Xpos - player.X) <= 3))
			{
				/* call shoot function */
				shoot(levelOneMiniBoss.rangedGun, 0, 1, levelOneMiniBoss.Xpos, levelOneMiniBoss.Ypos);
			}

			/* if the enemy is BELOW the player */
			if ((levelOneMiniBoss.Ypos - player.Y <= 20) && 
				(levelOneMiniBoss.Ypos - player.Y >= -2) &&
				(abs(levelOneMiniBoss.Xpos - player.X) <= 3))
			{
				/* call shoot function */
				shoot(levelOneMiniBoss.rangedGun, 0, -1, levelOneMiniBoss.Xpos, levelOneMiniBoss.Ypos);
			}

			/* *** update the level position *** */
			/* if the position is open, move there */
			if (isLevelPositionSpaceAll(newX, newY))
			{
				/* write a whitespace in the location where the enemy last was */
				setLevelPosition(ENEMY_LAYER, levelOneMiniBoss.Xpos, levelOneMiniBoss.Ypos, whiteSpacePosition);

				/* update the enemy struct with his new position */
				levelOneMiniBoss.Xpos = newX;
				levelOneMiniBoss.Ypos = newY;

				/* write the new enemy (with a new position) */
				setLevelPosition(ENEMY_LAYER, levelOneMiniBoss.Xpos, levelOneMiniBoss.Ypos, levelOneMiniBoss.Position);
			}

			/* else, if the position isn't open */
			else
			{
				/* update the enemy struct with his new position */
				levelOneMiniBoss.Xpos = levelOneMiniBoss.Xpos;
				levelOneMiniBoss.Ypos = levelOneMiniBoss.Ypos;

				/* write the new enemy (with a new position) */
				setLevelPosition(ENEMY_LAYER, levelOneMiniBoss.Xpos, levelOneMiniBoss.Ypos, levelOneMiniBoss.Position);
			}
		} /* end update enemy */

		/* update lastTime with the current time */
		levelOneMiniBoss.lastTime = current;
	} /* end player in boss */
}

/*******************************************************************************
   Function:    loadLevelOneBoss
   
   Description: This function is called to load the level 1 boss initially.
   
   Inputs:      xPos, yPos - initial position of the boss.
   
   Outputs:     None.
*******************************************************************************/
void loadLevelOneBoss(int xPos, int yPos)
{
	levelOneBoss.stats = malloc(sizeof(ENEMY));
	levelOneBoss.stats->health = 100;
	levelOneBoss.Xpos = xPos;
	levelOneBoss.Ypos = yPos;
	levelOneBoss.alive = 1;
	levelOneBoss.hpholder = 100;
	levelOneBoss.startflashing = 0;
	levelOneBoss.flashing = 0;

	/* set stats for scatter gun */
	levelOneBoss.closeGun = gunCopy(&weaponSpreadGun);
	levelOneBoss.closeGun->owner = OWNER_ENEMY;
	levelOneBoss.closeGun->damage = 10;
	levelOneBoss.closeGun->range = 600;

	/* set stats for ranged gun */
	levelOneBoss.rangedGun = gunCopy(&weaponDualPistol);
	levelOneBoss.rangedGun->owner = OWNER_ENEMY;
	levelOneBoss.rangedGun->damage = 5;
	levelOneBoss.rangedGun->range = 1500;

	levelOneBoss.Position = createLevelPosition(levelOneBoss.stats, POSITION_ENEMY, '#');
	setLevelPosition(ENEMY_LAYER, xPos, yPos, levelOneBoss.Position);
}

/*******************************************************************************
   Function:    loadLevelOneMiniBoss
   
   Description: This function is called to load the level 1 miniboss initially.
   
   Inputs:      xPos, yPos - initial position of the mini boss.
   
   Outputs:     None.
*******************************************************************************/
void loadLevelOneMiniBoss(int xPos, int yPos)
{
	levelOneMiniBoss.stats = malloc(sizeof(ENEMY));
	levelOneMiniBoss.stats->health = 70;
	levelOneMiniBoss.Xpos = xPos;
	levelOneMiniBoss.Ypos = yPos;
	levelOneMiniBoss.alive = 1;
	levelOneMiniBoss.hpholder = 50;
	levelOneMiniBoss.startflashing = 0;
	levelOneMiniBoss.flashing = 0;

	/* set stats for ranged gun */
	levelOneMiniBoss.rangedGun = gunCopy(&weaponPistol);
	levelOneMiniBoss.rangedGun->owner = OWNER_ENEMY;
	levelOneMiniBoss.rangedGun->damage = 5;
	levelOneMiniBoss.rangedGun->range = 1000;
	levelOneMiniBoss.lastTime = getCurrentTime();
	levelOneMiniBoss.interval = 180.0;

	levelOneMiniBoss.Position = createLevelPosition(levelOneMiniBoss.stats, POSITION_ENEMY, 'm');
	setLevelPosition(ENEMY_LAYER, xPos, yPos, levelOneMiniBoss.Position);
}

/*******************************************************************************
   Function:    deleteLevelOneBoss
   
   Description: This function is handles all the cleanup for the level 1 boss.
   
   Inputs:      None.
   
   Outputs:     None.
*******************************************************************************/
void deleteLevelOneBoss()
{
	setLevelPosition(ENEMY_LAYER, levelOneBoss.Xpos, levelOneBoss.Ypos, whiteSpacePosition);
	free(levelOneBoss.stats);
	free(levelOneBoss.Position);
}

/*******************************************************************************
   Function:    deleteLevelOneMiniBoss
   
   Description: This function is handles all the cleanup for the level 1 
                miniboss.
   
   Inputs:      None.
   
   Outputs:     None.
*******************************************************************************/
void deleteLevelOneMiniBoss()
{
	setLevelPosition(ENEMY_LAYER, levelOneMiniBoss.Xpos, levelOneMiniBoss.Ypos, whiteSpacePosition);
	free(levelOneMiniBoss.stats);
	free(levelOneMiniBoss.Position);
}
