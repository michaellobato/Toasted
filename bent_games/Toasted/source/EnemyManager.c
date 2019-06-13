/***************************************************************************
  Author: Paul Ewers
  Game:	  Toasted
  File:	  EnemyManager.c
  Date:	  January 23rd, 2007
          (modifyed through: January 5th, 2007)
  Description:
          This file contains all the source code for the move enemy
          functions. All methods are prototyped in MoveEnemy.h, and
          all types are declared there as well. The main purpose of this
          file and these functions is to handel the creation and management
          of basic enemies.
***************************************************************************/

#include <stdlib.h>        /* malloc, free, memcpy */
#include "EnemyManager.h"  /* everything we need */
#include "timer.h"         /* getCurrentTime */
#include "player.h"        /* player.X, player.Y */
#include "effects.h"       /* expl_update */

/*******************************************************************************
   Function:    loadEnemies
   
   Description: This is the master enemy function. It contains all the 
				attributes to each particular enemy and spawn point. It passes
				a dummy to each linked list function.
   
   Inputs:      None
   
   Outputs:     Return zero to indicate success.
*******************************************************************************/
int loadEnemies(void)
{
  /* create dummies to occupy the first position in both linked lists */
  ENEMY dummy;
  ENEMY_SPAWN dummy_spawn; 

  /* initialize a flag used to check when to reset the enemies */
  clearEnemiesFlag = 0;

  /* initialize all 'current' enemies to 0 */
  currentClawbots = 0;
  currentJumpers = 0;
  currentShooters = 0;
  currentSnipers = 0;
  currentbigburlys = 0;

  totalClawbotSpawns = 0;

  /* *** spawn point attributes *** */

  /* claw spawn */
  claw_spawn.type = 'C';
  claw_spawn.health = 50;
  claw_spawn.hpholder = 50;
  claw_spawn.flashing = 0;
  claw_spawn.startflashing = 0;
  claw_spawn.lastTime = getCurrentTime();
  claw_spawn.interval = 1300;

  /* jumper spawn */
  jumper_spawn.type = 'Z';
  jumper_spawn.health = 50;
  jumper_spawn.hpholder = 50;
  jumper_spawn.flashing = 0;
  jumper_spawn.startflashing = 0;
  jumper_spawn.lastTime = getCurrentTime();
  jumper_spawn.interval = 2000;

  /* shooter spawn */
  shooter_spawn.type = 'P';
  shooter_spawn.health = 50;
  shooter_spawn.hpholder = 50;
  shooter_spawn.flashing = 0;
  shooter_spawn.startflashing = 0;
  shooter_spawn.lastTime = getCurrentTime();
  shooter_spawn.interval = 3000;

  /* sniper spawn */
  sniper_spawn.type = 'Y';
  sniper_spawn.health = 35;
  sniper_spawn.hpholder = 35;
  sniper_spawn.flashing = 0;
  sniper_spawn.startflashing = 0;
  sniper_spawn.lastTime = getCurrentTime();
  sniper_spawn.interval = 3000;

  /* BigBurly spawn */
  bigburly_spawn.type = 'B';
  bigburly_spawn.health = 50;
  bigburly_spawn.hpholder = 50;
  bigburly_spawn.flashing = 0;
  bigburly_spawn.startflashing = 0;
  bigburly_spawn.lastTime = getCurrentTime();
  bigburly_spawn.interval = 4000;

  /* *** enemy attributes *** */

  /* clawbot */
  claw.type = 'c';
  claw.health = 10;
  claw.damage = 4;
  claw.posX = 0;
  claw.posY = 0;
  claw.lastTime = getCurrentTime();
  claw.interval = 180.0;
  claw.cullDistanceX = 80;
  claw.cullDistanceY = 50;

  /* jumperbot */
  jumper.type = 'z';
  jumper.health = 20;
  jumper.damage = 3;
  jumper.posX = 0;
  jumper.posY = 0;
  jumper.lastTime = getCurrentTime();
  jumper.interval = 200.0;
  jumper.cullDistanceX = 80;
  jumper.cullDistanceY = 45;

  /* shooterbot */
  shooter.type = 'p';
  shooter.health = 40;
  shooter.damage = 7;
  shooter.posX = 0;
  shooter.posY = 0;
  shooter.lastTime = getCurrentTime();
  shooter.interval = 200.0;
  shooter.cullDistanceX = 80;
  shooter.cullDistanceY = 50;

  /* sniperbot */
  sniper.type = 'y';
  sniper.health = 20;
  sniper.damage = 10;
  sniper.posX = 0;
  sniper.posY = 0;
  sniper.lastTime = getCurrentTime();
  sniper.interval = 200.0;
  sniper.cullDistanceX = 80;
  sniper.cullDistanceY = 50;

  /* BirBurly */
  bigburly.type = 'b';
  bigburly.health = 300;
  bigburly.damage = 15;
  bigburly.posX = 0;
  bigburly.posY = 0;
  bigburly.lastTime = getCurrentTime();
  bigburly.interval = 250.0;
  bigburly.cullDistanceX = 50;
  bigburly.cullDistanceY = 50;

  /* send dummies to functions */
  head = createEnemy(&dummy, NULL, 0, 0);
  head_spawn = createSpawn(&dummy_spawn, NULL, 0, 0);
  
  return 0;
}

/*******************************************************************************
   Function:    clearEnemies
   
   Description: This function deletes all the enemies and enemy spaws after
				the player dies. To accomplish this, it calls updateEnemyList
				and updateSpawnList.
   
   Inputs:      None 
   
   Outputs:     No return
*******************************************************************************/
void clearEnemies()
{
  /* set clear flag to true */
  clearEnemiesFlag = 1;

  /* call these two functions with the updated flag to clear the remaining
     enemies and spawns */
  updateEnemyList();
  updateSpawnList();

  /* reset the clear flag when finished clearing */
  clearEnemiesFlag = 0;
}

/*******************************************************************************
   Function:    damageEnemy
   
   Description: This function gets called by the bullet system and should damage
                a specific enemy by a specific ammount.
   
   Inputs:      ENEMY* cur_enemy - A pointer to the input enemy to be hurt.
				int ammount - Ammount of damage to deal to the enemy.
   
   Outputs:     No return
*******************************************************************************/
void damageEnemy(ENEMY* cur_enemy, int ammount)
{
  cur_enemy->health -= ammount;
}

/*******************************************************************************
   Function:    damageSpawn
   
   Description: This function gets called by the bullet system and should damage
                a specific spawn by a specific ammount.
   
   Inputs:      ENEMY_SPAWN* cur_spawn - A pointer to the input enemy to be hurt.
				int ammount - Ammount of damage to deal to the enemy.
   
   Outputs:     No return
*******************************************************************************/
void damageSpawn(ENEMY_SPAWN* cur_spawn, int ammount)
{
  cur_spawn->health -= ammount;
}

/*******************************************************************************
   Function:    updateSpawnList
   
   Description: This function is called by the main game loop. It makes sure
                the spawn linked list has been updated correctly. It also calls
				updateSpawn to update each individual spawn.
   
   Inputs:      None
   
   Outputs:     This function returns head_spawn, a pointer to the first node
                in the linked list. It points to the dummy (who occupies the
			    first element in the list), so this value should never change!
*******************************************************************************/
ENEMY_SPAWN_LINKLIST* updateSpawnList()
{
  /* create two pointers to keep track of where we are in the list */
  ENEMY_SPAWN_LINKLIST* prev = NULL;
  ENEMY_SPAWN_LINKLIST* current = head_spawn;
  
  /* while we aren't at the last spawn */
  while (current != NULL)
  {
      /* check to make sure we aren't pointing at the dummy spawn */
      if (prev != NULL)
	  {
        /* if a spawn point's health is less than or equal to zero */
        if (current->data->health <= 0 || clearEnemiesFlag == 1)
        {
            /* move the previous spawns' pointer to point past the dead spawn 
		        (this keeps the list connected) */
            prev->next = current->next;

			/* call the explosion system whenever a spawn is destroyed */
			explosion(current->data->posX, current->data->posY, 500, OWNER_PLAYER);

		        /* reduce total number of Clawbot spawns  and recalculate total number of enemies that spawn */
		        if (current->data->type == 'C')
		        {
		          currentClawbotSpawns--;
		          totalClawbotSpawns--; 
		        }
		        else if (current->data->type == 'Z')
		        {
		          currentJumperSpawns--;
		        }
				else if (current->data->type == 'P')
		        {
		          currentShooterSpawns--;
		        }
				else if (current->data->type == 'Y')
		        {
		          currentSniperSpawns--;
		        }
				else if (current->data->type == 'B')
		        {
		          currentBigBurlySpawns--;
		        }

		    /* free level position */
		    setLevelPosition(ENEMY_LAYER, current->data->posX, current->data->posY, whiteSpacePosition);
		    free(current->data->lvl_pos);

		    /* free our copy of our spawn */
		    free(current->data);

            /* free our link list entity */
            free(current);

            current = prev;
        }

		/* if the spawn isn't dead, update it! */
		else
		{
		  updateSpawn(current->data);
		}
	  }

      /* keep track of previous spawn */
      prev = current;
            
      /* increment pointer */
      current = current->next;
  }
  
  return head_spawn;
}

/*******************************************************************************
   Function:    updateEnemyList
   
   Description: This function is called by the main game loop. It makes sure
                the enemy linked list has been updated correctly. It also calls
				updateEnemy to update each individual enemy.
   
   Inputs:      None
   
   Outputs:     This function returns head_spawn, a pointer to the first node
                in the linked list. It points to the dummy (who occupies the
			    first element in the list), so this value should never change!
*******************************************************************************/
ENEMY_LINKLIST* updateEnemyList()
{
  /* create two pointers to keep track of where we are in the list */
  ENEMY_LINKLIST* prev = NULL;
  ENEMY_LINKLIST* current = head;
  
  /* while we aren't at the last enemy */
  while (current != NULL)
  {
      /* check to make sure we aren't pointing at the dummy enemy */
      if (prev != NULL)
      {
        /* if an enemy's health is less than or equal to zero  OR 
		 * we reset the level OR
		 * the distance from the enemy to the player is less than the 
		 * cull distance 
		 */
        if ((current->data->health <= 0) || 
			(clearEnemiesFlag == 1) ||
			(abs(player.X - current->data->posX) > current->data->cullDistanceX) ||
			(abs(player.Y - current->data->posY) > current->data->cullDistanceY))
        {
		  /* decrement total enemies based on type */
		  if (current->data->type == 'c')
		    currentClawbots--;

		  /* decrement total enemies based on type */
		  else if (current->data->type == 'z')
		    currentJumpers--;

		  /* decrement total enemies based on type */
		  else if (current->data->type == 'p')
		    currentShooters--;

		  /* decrement total enemies based on type */
		  else if (current->data->type == 'y')
		    currentSnipers--;

		  /* decrement total enemies based on type */
		  else if (current->data->type == 'b')
		    currentbigburlys--;

          /* move the previous enemy's pointer to point past the dead enemy 
		     (this keeps the list connected) */
          prev->next = current->next;
  
		  /* free level position */
		  setLevelPosition(ENEMY_LAYER, current->data->posX, current->data->posY, whiteSpacePosition);
		  free(current->data->enemy_pos);

		  /* free our copy of our enemy */
		  free(current->data);

          /* free our dead enemy */
          free(current);

          current = prev;
        }

		/* if the enemy isn't dead, update it! */
		else
		  updateEnemy(current->data);
      }

      /* keep track of previous enemy */
      prev = current;
            
      /* increment pointer */
      current = current->next;
  }
  
  return head;
}

/*******************************************************************************
   Function:    updateSpawn
   
   Description: This function cycles through all the spawnpoints and gets called
                from updateSpawnList. This function handles all of the "AI" for
				the spawn points. i.e. it tells the spawnpoints when to spawn
				enemies. This function calls createEnemy.
   
   Inputs:      ENEMY_SPAWN* enemy_spawn - pointer to an enemy spawn point.
   
   Outputs:     No return
*******************************************************************************/
void updateSpawn(ENEMY_SPAWN* enemy_spawn)
{
  double current = getCurrentTime();

  /* check to see if the spawnpoint took damage */
  if (enemy_spawn->health < enemy_spawn->hpholder)
  {
	  enemy_spawn->flashing = 1;
	  enemy_spawn->hpholder = enemy_spawn->health;
	  enemy_spawn->startflashing = current;
  }
  
  /* reset the spawnpoint after a set ammount of time */
  else if (enemy_spawn->health >= enemy_spawn->hpholder && 
	       enemy_spawn->startflashing + 50 < current)
  {
	  enemy_spawn->flashing = 0;
  }

  /* limit the number of enemies on screen and spawn at the correct time */
  if ((currentClawbots <= totalClawbots) && 
	  (current - enemy_spawn->lastTime >= enemy_spawn->interval) && 
	  (enemy_spawn->type == 'C'))
  {
	/* creat a copy of a pointer to an ENEMY */
	ENEMY* copy = enemyCopy(&claw);

	 /* creat a new LEVEL_POSITION to hold an enemy */
	LEVEL_POSITION* new_pos = createLevelPosition(copy, POSITION_ENEMY, 'c');

	/* call createEnemy with the position of 1 to the right of the spawn */
	createEnemy(copy, new_pos, enemy_spawn->posX + 1, enemy_spawn->posY);

	/* update lastTime with the current time */
	enemy_spawn->lastTime = current;

	/* increment the current number of clawbots on the map */
	currentClawbots++;
  }

  else if ((currentJumpers <= totalJumpers) && 
		   (current - enemy_spawn->lastTime >= enemy_spawn->interval) && 
		   (enemy_spawn->type == 'Z'))
  {
	/* creat a copy of a pointer to an ENEMY */
	ENEMY* copy = enemyCopy(&jumper);

	/* creat a new LEVEL_POSITION to hold an enemy */
	LEVEL_POSITION* new_pos = createLevelPosition(copy, POSITION_ENEMY, 'z');

	/* call createEnemy with the position of 1 to the right of the spawn */
	createEnemy(copy, new_pos, enemy_spawn->posX + 1, enemy_spawn->posY);

	/* update lastTime with the current time */
	enemy_spawn->lastTime = current;

	/* increment the current number of jumpers on the map */
	currentJumpers++;
  }

  else if ((currentShooters <= totalShooters) && 
		   (current - enemy_spawn->lastTime >= enemy_spawn->interval) && 
		   (enemy_spawn->type == 'P'))
  {
	/* creat a copy of a pointer to an ENEMY */
	ENEMY* copy = enemyCopy(&shooter);

	/* creat a new LEVEL_POSITION to hold an enemy */
	LEVEL_POSITION* new_pos = createLevelPosition(copy, POSITION_ENEMY, 't');

	/* call createEnemy with the position of 1 to the right of the spawn */
	createEnemy(copy, new_pos, enemy_spawn->posX + 1, enemy_spawn->posY);

	/* update lastTime with the current time */
	enemy_spawn->lastTime = current;

	/* increment the current number of jumpers on the map */
	currentShooters++;
  }

  else if ((currentSnipers <= totalSnipers) && 
		   (current - enemy_spawn->lastTime >= enemy_spawn->interval) && 
		   (enemy_spawn->type == 'Y'))
  {
	/* creat a copy of a pointer to an ENEMY */
	ENEMY* copy = enemyCopy(&sniper);

	/* creat a new LEVEL_POSITION to hold an enemy */
	LEVEL_POSITION* new_pos = createLevelPosition(copy, POSITION_ENEMY, 'y');

	/* call createEnemy with the position of 1 to the right of the spawn */
	createEnemy(copy, new_pos, enemy_spawn->posX + 1, enemy_spawn->posY);

	/* update lastTime with the current time */
	enemy_spawn->lastTime = current;

	/* increment the current number of jumpers on the map */
	currentSnipers++;
  }

  else if ((currentbigburlys <= totalbigburlys) && 
		   (current - enemy_spawn->lastTime >= enemy_spawn->interval) && 
		   (enemy_spawn->type == 'B'))
  {
	/* creat a copy of a pointer to an ENEMY */
	ENEMY* copy = enemyCopy(&bigburly);

	/* creat a new LEVEL_POSITION to hold an enemy */
	LEVEL_POSITION* new_pos = createLevelPosition(copy, POSITION_ENEMY, 'b');

	/* call createEnemy with the position of 1 to the right of the spawn */
	createEnemy(copy, new_pos, enemy_spawn->posX + 1, enemy_spawn->posY);

	/* update lastTime with the current time */
	enemy_spawn->lastTime = current;

	/* increment the current number of jumpers on the map */
	currentbigburlys++;
  }
}

/*******************************************************************************
   Function:    updateEnemy
   
   Description: This function updates each enemy. It contains the AI for the 
                enemies.
    
   Inputs:      ENEMY* enemy - pointer to the enemy to update.
   
   Outputs:     None
*******************************************************************************/
void updateEnemy(ENEMY* enemy)
{
  double current = getCurrentTime();
  int shoot_distance_x = 0;
  int shoot_distance_y = 0;

  /* limit the number of enemies on screen and spawn at the correct time */
  if (current - enemy->lastTime >= enemy->interval)
  {
	  /* new x and y locations for our enemy */
	  int newX = enemy->posX;
	  int newY = enemy->posY;

	  /* get the position of the player */
	  int plrX = player.X;
	  int plrY = player.Y;

	  /* select a random movement pattern */
	  int direction = rand() % 4;

	  /* damage the player if the enemy is adjacent the player */
	  if ((abs(enemy->posX - player.X) <= 1) && (abs(enemy->posY - player.Y) <= 1))
	  {
		player.hp -= enemy->damage;
	  }
	  
	  /* else if the enemy isn't adjacent the player */
	  else
	  {	  
		  /* *** Jumper AI *** */
		  if (enemy->type == 'z')
		  {
			  /* Jumper AI (best path) */
			  if ((direction == 0) || (direction == 1))
			  {
				  if (plrX <= newX)
					newX = enemy->posX - 2;

				  else if (plrX >= newX)
					newX = enemy->posX + 2;

				  if (plrY >= newY)
					newY = enemy->posY + 2;

				  else if (plrY <= newY)
					newY = enemy->posY - 2;
			  }

			  /* Jumper AI (second best path) */
			  else if (direction == 2)
			  {
				if (plrX <= newX)
					newX = enemy->posX - 2;

				else if (plrX >= newX)
				  newX = enemy->posX + 2;

				newY = enemy->posY;
			  }

			  /* Jumper AI (third best path) */
			  else if (direction == 3)
			  {
				if (plrY >= newY)
				  newY = enemy->posY + 2;

				else if (plrY <= newY)
				  newY = enemy->posY - 2;

				newX = enemy->posX;
			  }
		  }

		  /* *** Clawbot AI *** */
		  /* if clawbot, OR if the zigzag location is NOT valid, OR if a shooter is a certian distance
		     from the player, OR a BigBurly bot AI */
		  if ((enemy->type == 'p') || 
			  (enemy->type == 'c') ||
			  (enemy->type == 'b') ||
			  (!isLevelPositionSpaceAll(newX, newY)))
		  {
			  /* Clawbot AI (best path) */
			  if ((direction == 0) || (direction == 1))
			  {
				  if (plrX <= newX)
					newX = enemy->posX - 1;

				  else if (plrX >= newX)
					newX = enemy->posX + 1;

				  if (plrY >= newY)
					newY = enemy->posY + 1;

				  else if (plrY <= newY)
					newY = enemy->posY - 1;
			  }

			  /* Clawbot AI (second best path) */
			  else if (direction == 2)
			  {
				if (plrX <= newX)
				  newX = enemy->posX - 1;

				else if (plrX >= newX)
				  newX = enemy->posX + 1;

				newY = enemy->posY;
			  }

			  /* Clawbot AI (third best path) */
			  else if (direction == 3)
			  {
				if (plrY >= newY)
				  newY = enemy->posY + 1;

				else if (plrY <= newY)
				  newY = enemy->posY - 1;

				newX = enemy->posX;
			  }
		  }

		  /* *** Shooter / Sniper AI *** */
		  if ((enemy->type == 'p') || (enemy->type == 'y') || (enemy->type == 'b'))
		  {
			/* if shooter bot, set length */
			if (enemy->type == 'p')
			{
			  shoot_distance_x = 25;
			  shoot_distance_y = 20;
			}
			/* if sniper bot, set length */
			else if (enemy->type == 'y')
			{
			  shoot_distance_x = 35;
			  shoot_distance_y = 30;
			}
			/* if burly bot, set length */
			else if (enemy->type == 'b')
			{
			  shoot_distance_x = 10;
			  shoot_distance_y = 7;
			}

			/* if the enemy is to the right of the player */
			if ((enemy->posX - player.X <= shoot_distance_x) && (enemy->posX - player.X > -2) &&
				(abs(enemy->posY - player.Y) <= 2))
			{
			  /* call shoot function */
			  shoot(enemy->currentGun, -1, 0, enemy->posX, enemy->posY);

			  /* if not a BurlyBot, back off */
			  if (enemy->type != 'b')
			  {
			    newX = enemy->posX + 1;
			    newY = enemy->posY;
			  }
			}

			/* if the enemy is to the left of the player */
			else if ((enemy->posX - player.X >= -shoot_distance_x) && (enemy->posX - player.X < -2) && 
					 (abs(enemy->posY - player.Y) <= 2))
			{
			  /* call shoot function */
			  shoot(enemy->currentGun, 1, 0, enemy->posX, enemy->posY);

			  /* if not a BurlyBot, back off */
			  if (enemy->type != 'b')
			  {
			    newX = enemy->posX - 1;
			    newY = enemy->posY;
			  }
			}

			/* if the enemy is above the player */
			else if ((enemy->posY - player.Y >= -shoot_distance_y) && (enemy->posY - player.Y < -2) &&
					 (abs(enemy->posX - player.X) <= 2))
			{
			  /* call shoot function */
			  shoot(enemy->currentGun, 0, 1, enemy->posX, enemy->posY);

			  /* if not a BurlyBot, back off */
			  if (enemy->type != 'b')
			  {
			    newX = enemy->posX;
			    newY = enemy->posY - 1;
			  }
			}

			/* if the enemy is below the player */
			else if ((enemy->posY - player.Y <= shoot_distance_y) && (enemy->posY - player.Y > 2) &&
					 (abs(enemy->posX - player.X) <= 2))
			{
			  /* call shoot function */
			  shoot(enemy->currentGun, 0, -1, enemy->posX, enemy->posY);

			  /* if not a BurlyBot, back off */
			  if (enemy->type != 'b')
			  {
			    newX = enemy->posX;
			    newY = enemy->posY + 1;
			  }
			}
		  }

		  /* If the next location is open */
		  if ((isLevelPositionSpaceAll(newX, newY)) || !(isLevelPositionSpace(SWITCH_LAYER, newX, newY)))
		  {
			/* write a whitespace in the location where the enemy last was */
			setLevelPosition(ENEMY_LAYER, enemy->posX, enemy->posY, whiteSpacePosition);

			/* update the enemy struct with his new position */
			enemy->posX = newX;
			enemy->posY = newY;

			/* write the new enemy (with a new position) */
			setLevelPosition(ENEMY_LAYER, newX, newY, enemy->enemy_pos);
		  }

		  /* else, if the position isn't open and the enemy isn't adjacent the player */
		  else
		  {
			/* update the enemy struct with his new position */
			enemy->posX = enemy->posX;
			enemy->posY = enemy->posY;

			/* write the new enemy (with a new position) */
			setLevelPosition(ENEMY_LAYER, enemy->posX, enemy->posY, enemy->enemy_pos);
		  }
	  }

	/* update lastTime with the current time */
	enemy->lastTime = current;
  }
}

/*******************************************************************************
   Function:    createSpawn
   
   Description: This function is called directly from the LevelManager. It is
                responsible for creating a new spawn point and setting the
				spawn points attributes.
   
   Inputs:      ENEMY_SPAWN* inputData - Data to be put in the enemy spawn.
                LEVEL_POSITION* lvl_pos - pointer to the LEVEL_POSITION
				int x - where to place the spawn.
				int y - where to place the spawn.
   
   Outputs:     This function returns a pointer to the newly created spawn
                node.
*******************************************************************************/
ENEMY_SPAWN_LINKLIST* createSpawn(ENEMY_SPAWN* inputData, LEVEL_POSITION* lvl_pos, int x, int y)
{
  /* allocate memory for a new node in the list */
  ENEMY_SPAWN_LINKLIST* newNode = (ENEMY_SPAWN_LINKLIST*) malloc (sizeof(ENEMY_SPAWN_LINKLIST));
  ENEMY_SPAWN_LINKLIST* current = head_spawn; 

  /* so we don't create a spawn in the dummy */
  if (lvl_pos != NULL)
  {
    /* fill the node with its new information */
    lvl_pos->data = inputData;
  }

  /* if we are given the memory */
  if (newNode)
  {
    /* fill the node with the spawns' information */
    newNode->data = inputData;
    newNode->next = NULL;
	newNode->data->lvl_pos = lvl_pos;
	newNode->data->posX = x;
	newNode->data->posY = y;
      
    /* if we aren't at the end of the list */
    if (current != NULL)
    {
      /* loop until we are at the end of the list */
      while (current->next != NULL)
        current = current->next;

      /* point to our new node */
      current->next = newNode;  
    }
  }

  /* if we are creating a clawbot spawn */
  if (newNode->data->type == 'C')
  {
	/* increment current clawbot spawns */
	currentClawbotSpawns++;

	/* recalculate how many total clawbots to spawn */
	totalClawbotSpawns++;
  }
	
  /* if we are creating a jumper spawn */
  else if (newNode->data->type == 'Z')
  {
	/* increment current jumper spawns */
	currentJumperSpawns++;
  }

  /* if we are creating a shooter spawn */
  else if (newNode->data->type == 'P')
  {
	/* increment current shooter spawns */
	currentShooterSpawns++;
  }

  /* if we are creating a sniper spawn */
  else if (newNode->data->type == 'Y')
  {
	/* increment current sniper spawns */
	currentSniperSpawns++;
  }

  /* if we are creating a BigBurly spawn */
  else if (newNode->data->type == 'B')
  {
	/* increment current BigBurly spawns */
	currentBigBurlySpawns++;
  }
  
  return newNode;
}

/*******************************************************************************
   Function:    createEnemy
   
   Description: This function is called from updateSpawn. It is
                responsible for creating a new enemy and setting the
				nescessary information about the enemy.
   
   Inputs:      ENEMY_SPAWN* inputData - Data to be put in the enemy.
                LEVEL_POSITION* lvl_pos - pointer to the LEVEL_POSITION
				int x - where to place the enemy.
				int y - where to place the enemy.
   
   Outputs:     This function returns a pointer to the newly created enemy
                node.
*******************************************************************************/
ENEMY_LINKLIST* createEnemy(ENEMY* inputData, LEVEL_POSITION* lvl_pos, int x, int y)
{
  /* allocate memory for a new node in the list */
  ENEMY_LINKLIST* newNode = (ENEMY_LINKLIST*) malloc (sizeof(ENEMY_LINKLIST));
  ENEMY_LINKLIST* current = head; 
  
  /* so we don't create an enemy in the dummy */
  if (lvl_pos != NULL)
  {
    lvl_pos->data = inputData;
	setLevelPosition(ENEMY_LAYER, x, y, lvl_pos);
  }

  /* if we are given the memory */
  if (newNode)
  {
    /* fill the node with the enemy's information */
    newNode->data = inputData;
    newNode->next = NULL;
	newNode->data->posX = x;
	newNode->data->posY = y;
	newNode->data->enemy_pos = lvl_pos;

	if (newNode->data->type == 'p')
	{
	  newNode->data->currentGun = gunCopy(&weaponSubMachineGun);
	  newNode->data->currentGun->owner = OWNER_ENEMY;
	  newNode->data->currentGun->range = 1400;
	  newNode->data->currentGun->damage = newNode->data->damage;
	  newNode->data->currentGun->reloadTime = 450;
	}
	else if (newNode->data->type == 'y')
	{
	  newNode->data->currentGun = gunCopy(&weaponRifle);
	  newNode->data->currentGun->owner = OWNER_ENEMY;
	  newNode->data->currentGun->damage = newNode->data->damage;
	}
	else if (newNode->data->type == 'b')
	{
	  newNode->data->currentGun = gunCopy(&weaponScatterGun);
	  newNode->data->currentGun->owner = OWNER_ENEMY;
	  newNode->data->currentGun->damage = newNode->data->damage;
	}
      
    /* if we aren't at the end of the list */
    if (current != NULL)
    {
      /* loop until we are at the end of the list */
      while (current->next != NULL)
        current = current->next;

      /* point to our new node */
      current->next = newNode;  
    }
  }
  
  return newNode;
}

/*******************************************************************************
   Function:    spawnCopy
   
   Description: This function makes a copy of a spawnpoint.
   
   Inputs:      ENEMY_SPAWN* spawn - pointer to an enemy spawn struct.
   
   Outputs:     Returns a pointer to the copy we make.
*******************************************************************************/
ENEMY_SPAWN* spawnCopy(ENEMY_SPAWN* spawn)
{
  ENEMY_SPAWN* newspawn = (ENEMY_SPAWN* )malloc(sizeof(ENEMY_SPAWN));

  memcpy(newspawn, spawn, sizeof(ENEMY_SPAWN));

  return newspawn;
}

/*******************************************************************************
   Function:    enemyCopy
   
   Description: This function makes a copy of an enemy.
   
   Inputs:      ENEMY* enemy - pointer to an enemy struct.
   
   Outputs:     Returns a pointer to the copy we make.
*******************************************************************************/
ENEMY* enemyCopy(ENEMY* enemy)
{
  ENEMY* newenemy = (ENEMY* )malloc(sizeof(ENEMY));

  memcpy(newenemy, enemy, sizeof(ENEMY));

  return newenemy;
}
