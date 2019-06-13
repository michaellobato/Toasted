/*******************************************************************************
	Author: Paul Ewers, Cullen McElroy
	Game:	  Toasted
	File:	  LevelTwo.c
	Date:	  February 17th, 2007

	Description:
	This header file has everything we need for LevelTwo.
*******************************************************************************/

#include "LevelTwo.h"
#include "EnemyManager.h"
#include "WeaponManager.h"
#include "LevelManager.h"
#include "player.h"

/* modes for the boss to circle the player */
#define CCW 1
#define CW  2

/*******************************************************************************
   Function:    updateLevelTwoBoss
   
   Description: This function handles all the updaing for the level 2 boss.
   
   Inputs:      None.
   
   Outputs:     None.
*******************************************************************************/
void updateLevelTwoBoss()
{
	double current = getCurrentTime();

	/* check to see if the boss took damage */
	if (levelTwoBoss.stats->health < levelTwoBoss.hpholder)
	{
		levelTwoBoss.flashing = 1;
		levelTwoBoss.hpholder = levelTwoBoss.stats->health;
		levelTwoBoss.startflashing = current;
	}

	/* reset the boss after a set ammount of time */
	else if (levelTwoBoss.stats->health >= levelTwoBoss.hpholder && 
	         levelTwoBoss.startflashing + 50 < current)
    {
	    levelTwoBoss.flashing = 0;
    }

	/* only update the boss if the player is in the correct map location */
	if ((levelTwoBoss.alive != 0) && 
		(current - levelTwoBoss.lastTime >= levelTwoBoss.interval) &&
		(player.X >= 170) &&
		(player.X <= 241) &&
		(player.Y >= 97) &&
		(player.Y <= 198))
	{
		/* set the boss waypoint */
		player.atLevelTwoBoss = 1;

		/* if the boss is dead */
		if (levelTwoBoss.stats->health <= 0)
		{
			/* create and show the weapon */
			/* drop the weapon */
			if(!player.weaponsEnabled[GUN_SUBMACHINEGUN])
			{
				LEVEL_POSITION* weapon_position = createLevelPosition(NULL, POSITION_WEAPON, '[');
				setLevelPosition(PICKUP_LAYER, levelTwoBoss.Xpos, levelTwoBoss.Ypos, weapon_position);
			}

			/* set flag for level two complete */
			player.levelTwoComplete = 1;

			/* clear the walls so the player can leave */
			clearLevelPositions(WALL_LAYER, 'V');

			/* set flag for boss dead and delete him */
			levelTwoBoss.alive = 0;
			deleteLevelTwoBoss();

			/* set the boss waypoint */
			player.atLevelTwoBoss = 0;
		}
		else
		{
			int newX = levelTwoBoss.Xpos;
			int newY = levelTwoBoss.Ypos;
			int shoot_distance_x_short = 15;
			int shoot_distance_y_short = 10;
			int shoot_padding = 5;

			/* *** movement (clawbot esque AI) *** */
			if (player.X <= newX)
				newX = levelTwoBoss.Xpos - 2;

			else if (player.X > newX)
				newX = levelTwoBoss.Xpos + 2;

			if (player.Y >= newY)
				newY = levelTwoBoss.Ypos + 2;

			else if (player.Y < newY)
				newY = levelTwoBoss.Ypos - 2;

			/* *** movement (circle the player) *** */
			/* if the enemy is LEFT of the player */
			if ((newX < player.X - 1) && (abs(player.Y - newY) < shoot_padding))
			{
				if (levelTwoBoss.circleMode == CCW)
				{
					newY = levelTwoBoss.Ypos - 2;
					newX = levelTwoBoss.Xpos;
				}
				else
				{
					newY = levelTwoBoss.Ypos + 2;
					newX = levelTwoBoss.Xpos;
				}
			}

			/* if the enemy is RIGHT of the player */
			else if ((newX > player.X + 1) && (abs(player.Y - newY) < shoot_padding))
			{
				if (levelTwoBoss.circleMode == CCW)
				{
					newY = levelTwoBoss.Ypos + 2;
					newX = levelTwoBoss.Xpos;
				}
				else
				{
					newY = levelTwoBoss.Ypos - 2;
					newX = levelTwoBoss.Xpos;
				}
			}

			/* if the enemy is ABOVE the player */
			if ((newY < player.Y + 1) && (abs(player.X - newX) < shoot_padding))
			{
				if (levelTwoBoss.circleMode == CCW)
				{
					newX = levelTwoBoss.Xpos + 2;
					newY = levelTwoBoss.Ypos;
				}
				else 
				{
					newX = levelTwoBoss.Xpos - 2;
					newY = levelTwoBoss.Ypos;
				}
			}

			/* if the enemy is BELOW the player */
			else if ((newY >= player.Y + 1) && (abs(player.X - newX) < shoot_padding))
			{
				if (levelTwoBoss.circleMode == CCW)
				{
					newX = levelTwoBoss.Xpos - 2;
					newY = levelTwoBoss.Ypos;
				}
				else
				{
					newX = levelTwoBoss.Xpos + 2;
					newY = levelTwoBoss.Ypos;
				}
			}

			/* *** shooting *** */
			/* if the enemy is to the RIGHT of the player (close gun) */
			if ((levelTwoBoss.Xpos - player.X <= shoot_distance_x_short) && 
				(levelTwoBoss.Xpos - player.X >= 0) &&
				(abs(levelTwoBoss.Ypos - player.Y) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelTwoBoss.currentGun, -1, 0, levelTwoBoss.Xpos, levelTwoBoss.Ypos);
			}

			/* if the enemy is to the LEFT of the player (close gun) */
			else if ((levelTwoBoss.Xpos - player.X >= -shoot_distance_x_short) && 
				(levelTwoBoss.Xpos - player.X <= 0) && 
				(abs(levelTwoBoss.Ypos - player.Y) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelTwoBoss.currentGun, 1, 0, levelTwoBoss.Xpos, levelTwoBoss.Ypos);
			}

			/* if the enemy is ABOVE the player (close gun) */
			if ((levelTwoBoss.Ypos - player.Y <= shoot_distance_y_short) && 
				(levelTwoBoss.Ypos - player.Y >= 0) &&
				(abs(levelTwoBoss.Xpos - player.X) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelTwoBoss.currentGun, 0, -1, levelTwoBoss.Xpos, levelTwoBoss.Ypos);
			}

			/* if the enemy is BELOW the player (close gun) */
			else if ((levelTwoBoss.Ypos - player.Y >= -shoot_distance_y_short) && 
				(levelTwoBoss.Ypos - player.Y <= 0) &&
				(abs(levelTwoBoss.Xpos - player.X) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelTwoBoss.currentGun, 0, 1, levelTwoBoss.Xpos, levelTwoBoss.Ypos);
			}

			/* *** update the level position *** */
			/* if the position is open, move there */
			if (isLevelPositionSpaceAll(newX, newY))
			{
				/* write a whitespace in the location where the enemy last was */
				setLevelPosition(ENEMY_LAYER, levelTwoBoss.Xpos, levelTwoBoss.Ypos, whiteSpacePosition);

				/* update the enemy struct with his new position */
				levelTwoBoss.Xpos = newX;
				levelTwoBoss.Ypos = newY;

				/* write the new enemy (with a new position) */
				setLevelPosition(ENEMY_LAYER, levelTwoBoss.Xpos, levelTwoBoss.Ypos, levelTwoBoss.Position);
			}

			/* else, if the position isn't open */
			else
			{
				/* update the enemy struct with his new position */
				levelTwoBoss.Xpos = levelTwoBoss.Xpos;
				levelTwoBoss.Ypos = levelTwoBoss.Ypos;

				/* write the new enemy (with a new position) */
				setLevelPosition(ENEMY_LAYER, levelTwoBoss.Xpos, levelTwoBoss.Ypos, levelTwoBoss.Position);

				/* if the next position is taken, change direction */
				if (levelTwoBoss.circleMode == CCW)
				{
					levelTwoBoss.circleMode = CW;
				}
				else if (levelTwoBoss.circleMode == CW)
				{
					levelTwoBoss.circleMode = CCW;
				}
			}
		} /* end update enemy */

		/* update lastTime with the current time */
		levelTwoBoss.lastTime = current;

	} /* end player in boss */
}

/*******************************************************************************
   Function:    loadLevelTwoBoss
   
   Description: This function is called once to intitially load the level 2
                boss.
   
   Inputs:      xPos, yPos - Position to load in the boss.
   
   Outputs:     None.
*******************************************************************************/
void loadLevelTwoBoss(int xPos, int yPos)
{
	levelTwoBoss.stats = malloc(sizeof(ENEMY));
	levelTwoBoss.stats->health = 150;
	levelTwoBoss.Xpos = xPos;
	levelTwoBoss.Ypos = yPos;
	levelTwoBoss.alive = 1;
	levelTwoBoss.circleMode = CCW;
	levelTwoBoss.hpholder = 150;
	levelTwoBoss.flashing = 0;
	levelTwoBoss.startflashing = 0;

	/* set stats for ranged gun */
	levelTwoBoss.currentGun = gunCopy(&weaponPistol);
	levelTwoBoss.currentGun->owner = OWNER_ENEMY;
	levelTwoBoss.currentGun->damage = 10;
	levelTwoBoss.currentGun->range = 1000;
	levelTwoBoss.currentGun->reloadTime = 250;
	levelTwoBoss.lastTime = getCurrentTime();
	levelTwoBoss.interval = 80.0;

	levelTwoBoss.Position = createLevelPosition(levelTwoBoss.stats, POSITION_ENEMY, 'a');
	setLevelPosition(ENEMY_LAYER, xPos, yPos, levelTwoBoss.Position);
}

/*******************************************************************************
   Function:    deleteLevelTwoBoss
   
   Description: This function handles all the cleanup for the level 2 boss.
   
   Inputs:      None.
   
   Outputs:     None.
*******************************************************************************/
void deleteLevelTwoBoss()
{
	setLevelPosition(ENEMY_LAYER, levelTwoBoss.Xpos, levelTwoBoss.Ypos, whiteSpacePosition);
	free(levelTwoBoss.stats);
	free(levelTwoBoss.Position);
}

/*******************************************************************************
Function:    updateLevelTwoMiniBoss

Description: Handle all of the AI for the minibosses in level two.

Inputs:      None

Outputs:     None
*******************************************************************************/
void updateLevelTwoMiniBoss()
{
	double current = getCurrentTime();

	/* check to see if the boss took damage */
	if (levelTwoMiniBoss.stats->health < levelTwoMiniBoss.hpholder)
	{
		/* make him flash */
		levelTwoMiniBoss.flashing = 1;
		levelTwoMiniBoss.hpholder = levelTwoMiniBoss.stats->health;
		levelTwoMiniBoss.startflashing = current;
	}

	/* reset the boss after a set ammount of time */
	else if (levelTwoMiniBoss.stats->health >= levelTwoMiniBoss.hpholder && 
	         levelTwoMiniBoss.startflashing + 100 < current)
    {
		/* reset his flashing variable */
	    levelTwoMiniBoss.flashing = 0;
    }

	/* only update the mini boss if the player is in the correct map location */
	if ((levelTwoMiniBoss.alive != 0) && 
		(current - levelTwoMiniBoss.lastTime >= levelTwoMiniBoss.interval) &&
		(player.X >= 11) &&
		(player.X <= 81) &&
		(player.Y >= 155) &&
		(player.Y <= 188))
	{
		/* if the mini boss is dead, delete him */
		if (levelTwoMiniBoss.stats->health <= 0)
		{
			/* open the walls so the player can leave */
			clearLevelPositions(WALL_LAYER, 'I');

			/* set his flag to dead and delete him */
			levelTwoMiniBoss.alive = 0;
			deleteLevelTwoMiniBoss();
		}

		/* if the mini boss is not dead, update it */
		else
		{
			int newX = levelTwoMiniBoss.Xpos;
			int newY = levelTwoMiniBoss.Ypos;
			int shoot_distance_x_short = 15;
			int shoot_distance_y_short = 10;
			int shoot_padding = 3;

			/* *** movement (clawbot esque AI) *** */
			if (player.X <= newX)
				newX = levelTwoMiniBoss.Xpos - 1;

			else if (player.X > newX)
				newX = levelTwoMiniBoss.Xpos + 1;

			if (player.Y >= newY)
				newY = levelTwoMiniBoss.Ypos + 1;

			else if (player.Y < newY)
				newY = levelTwoMiniBoss.Ypos - 1;

			/* *** movement (circle the player) *** */
			/* if the enemy is LEFT of the player */
			if ((newX < player.X - 1) && (abs(player.Y - newY) < shoot_padding))
			{
				if (levelTwoMiniBoss.circleMode == CCW)
				{
					newY = levelTwoMiniBoss.Ypos - 1;
					newX = levelTwoMiniBoss.Xpos;
				}
				else
				{
					newY = levelTwoMiniBoss.Ypos + 1;
					newX = levelTwoMiniBoss.Xpos;
				}
			}

			/* if the enemy is RIGHT of the player */
			else if ((newX > player.X + 1) && (abs(player.Y - newY) < shoot_padding))
			{
				if (levelTwoMiniBoss.circleMode == CCW)
				{
					newY = levelTwoMiniBoss.Ypos + 1;
					newX = levelTwoMiniBoss.Xpos;
				}
				else
				{
					newY = levelTwoMiniBoss.Ypos - 1;
					newX = levelTwoMiniBoss.Xpos;
				}
			}

			/* if the enemy is ABOVE the player */
			if ((newY < player.Y + 1) && (abs(player.X - newX) < shoot_padding))
			{
				if (levelTwoMiniBoss.circleMode == CCW)
				{
					newX = levelTwoMiniBoss.Xpos + 1;
					newY = levelTwoMiniBoss.Ypos;
				}
				else 
				{
					newX = levelTwoMiniBoss.Xpos - 1;
					newY = levelTwoMiniBoss.Ypos;
				}
			}

			/* if the enemy is BELOW the player */
			else if ((newY >= player.Y + 1) && (abs(player.X - newX) < shoot_padding))
			{
				if (levelTwoMiniBoss.circleMode == CCW)
				{
					newX = levelTwoMiniBoss.Xpos - 1;
					newY = levelTwoMiniBoss.Ypos;
				}
				else
				{
					newX = levelTwoMiniBoss.Xpos + 1;
					newY = levelTwoMiniBoss.Ypos;
				}
			}

			/* *** shooting *** */
			/* if the enemy is to the RIGHT of the player (close gun) */
			if ((levelTwoMiniBoss.Xpos - player.X <= shoot_distance_x_short) && 
				(levelTwoMiniBoss.Xpos - player.X >= 0) &&
				(abs(levelTwoMiniBoss.Ypos - player.Y) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelTwoMiniBoss.currentGun, -1, 0, levelTwoMiniBoss.Xpos, levelTwoMiniBoss.Ypos);
			}

			/* if the enemy is to the LEFT of the player (close gun) */
			else if ((levelTwoMiniBoss.Xpos - player.X >= -shoot_distance_x_short) && 
				(levelTwoMiniBoss.Xpos - player.X <= 0) && 
				(abs(levelTwoMiniBoss.Ypos - player.Y) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelTwoMiniBoss.currentGun, 1, 0, levelTwoMiniBoss.Xpos, levelTwoMiniBoss.Ypos);
			}

			/* if the enemy is ABOVE the player (close gun) */
			if ((levelTwoMiniBoss.Ypos - player.Y <= shoot_distance_y_short) && 
				(levelTwoMiniBoss.Ypos - player.Y >= 0) &&
				(abs(levelTwoMiniBoss.Xpos - player.X) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelTwoMiniBoss.currentGun, 0, -1, levelTwoMiniBoss.Xpos, levelTwoMiniBoss.Ypos);
			}

			/* if the enemy is BELOW the player (close gun) */
			else if ((levelTwoMiniBoss.Ypos - player.Y >= -shoot_distance_y_short) && 
				(levelTwoMiniBoss.Ypos - player.Y <= 0) &&
				(abs(levelTwoMiniBoss.Xpos - player.X) <= shoot_padding))
			{
				/* call shoot function */
				shoot(levelTwoMiniBoss.currentGun, 0, 1, levelTwoMiniBoss.Xpos, levelTwoMiniBoss.Ypos);
			}

			/* *** update the level position *** */
			/* if the position is open, move there */
			if (isLevelPositionSpaceAll(newX, newY))
			{
				/* write a whitespace in the location where the enemy last was */
				setLevelPosition(ENEMY_LAYER, levelTwoMiniBoss.Xpos, levelTwoMiniBoss.Ypos, whiteSpacePosition);

				/* update the enemy struct with his new position */
				levelTwoMiniBoss.Xpos = newX;
				levelTwoMiniBoss.Ypos = newY;

				/* write the new enemy (with a new position) */
				setLevelPosition(ENEMY_LAYER, levelTwoMiniBoss.Xpos, levelTwoMiniBoss.Ypos, levelTwoMiniBoss.Position);
			}

			/* else, if the position isn't open */
			else
			{
				/* update the enemy struct with his new position */
				levelTwoMiniBoss.Xpos = levelTwoMiniBoss.Xpos;
				levelTwoMiniBoss.Ypos = levelTwoMiniBoss.Ypos;

				/* write the new enemy (with a new position) */
				setLevelPosition(ENEMY_LAYER, levelTwoMiniBoss.Xpos, levelTwoMiniBoss.Ypos, levelTwoMiniBoss.Position);

				/* if the next position is taken, change direction */
				if (levelTwoMiniBoss.circleMode == CCW)
				{
					levelTwoMiniBoss.circleMode = CW;
				}
				else if (levelTwoMiniBoss.circleMode == CW)
				{
					levelTwoMiniBoss.circleMode = CCW;
				}
			}
		} /* end update enemy */

		/* update lastTime with the current time */
		levelTwoMiniBoss.lastTime = current;

	} /* end player in boss */
}

/*******************************************************************************
   Function:    loadLevelTwoMiniBoss
   
   Description: This function handles all the updaing for the level 2 mini boss.
   
   Inputs:      xPos, yPos - Location where to load the boss initially.
   
   Outputs:     None.
*******************************************************************************/
void loadLevelTwoMiniBoss(int xPos, int yPos)
{
	levelTwoMiniBoss.stats = malloc(sizeof(ENEMY));
	levelTwoMiniBoss.stats->health = 150;
	levelTwoMiniBoss.Xpos = xPos;
	levelTwoMiniBoss.Ypos = yPos;
	levelTwoMiniBoss.alive = 1;
	levelTwoMiniBoss.circleMode = CCW;
	levelTwoMiniBoss.hpholder = 150;
	levelTwoMiniBoss.startflashing = 0;
	levelTwoMiniBoss.flashing = 0;

	/* set stats for ranged gun */
	levelTwoMiniBoss.currentGun = gunCopy(&weaponScatterGun);
	levelTwoMiniBoss.currentGun->owner = OWNER_ENEMY;
	levelTwoMiniBoss.currentGun->damage = 4;
	levelTwoMiniBoss.currentGun->range = 1000;
	levelTwoMiniBoss.lastTime = getCurrentTime();
	levelTwoMiniBoss.interval = 80.0;

	levelTwoMiniBoss.Position = createLevelPosition(levelTwoMiniBoss.stats, POSITION_ENEMY, 'n');
	setLevelPosition(ENEMY_LAYER, xPos, yPos, levelTwoMiniBoss.Position);
}

/*******************************************************************************
   Function:    deleteLevelTwoMiniBoss
   
   Description: This function handles all the cleanup for the level 2 miniboss.
   
   Inputs:      None.
   
   Outputs:     None.
*******************************************************************************/
void deleteLevelTwoMiniBoss()
{
	setLevelPosition(ENEMY_LAYER, levelTwoMiniBoss.Xpos, levelTwoMiniBoss.Ypos, whiteSpacePosition);
	free(levelTwoMiniBoss.stats);
	free(levelTwoMiniBoss.Position);
}
