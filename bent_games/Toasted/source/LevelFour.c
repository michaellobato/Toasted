/*! ************************************************************************
  \file LevelFour.C
  \author Sheridan Beebe
  \par email: sbeebe@digipen.edu
  \par Game: Toasted
  \date Febuary 12th, 2007
  \brief

	This is the source file for LevelFour.c. It holds functions used
	specificaly for level four, for such things as bosses and special wall
	switching stuff, and minibosses.
    
***************************************************************************/

#include "LevelFour.h"
#include "timer.h"
#include "player.h"
#include "hud.h"

/* include enemy manager so that we can monitor things */
#include "EnemyManager.h"

/* main update function for level four */
void updateLevelFour()
{	
	/* update the super tank */
	updateSuperTank();

	/* update the minibosses */
	updateFSB(fsbOne);
	updateFSB(fsbTwo);
	updateFSB(fsbThree);
	updateFSB(fsbFour);
}

/* update an individual miniboss */
void updateFSB(FlyingSlashBot *fsb)
{
	int newX, newY;
	/* make sure the tank is alive before we do anything else */
	if(fsb->alive)
	{
		if(fsb->enemyStats->health >= 0)
		{
			/* see if its time to go to the next frame */
			if((getCurrentTime() - fsb->lastFrameTime) >= FSB_FRAME_DELAY && !fsb->animationPaused)
			{
				/* change the current frame */
				fsb->currentFrame++;

				/* see if we need to go back to 0 */
				if(fsb->currentFrame == FSB_FRAME_COUNT)
				{
					fsb->currentFrame = 0;
				}

				fsb->lastFrameTime = (int)getCurrentTime();
			}

			switch(fsb->currentState)
			{
			case FSB_NOTHING:
				if(player.Y == 116 && player.X >= 85 && player.X <= 125)
				{
					fsb->currentState = FSB_ATTACKING;
					fsb->animationPaused = 0;
				}
				break;
			case FSB_ATTACKING:
				/* see if we can move again */
				if((getCurrentTime() - fsb->lastMoveTime) >= FSB_MOVE_DELAY)
				{
					
					if(checkFSBCollisionPlayer(fsb))
					{
						player.hp -= 20;
						if(player.hp < 0)
						{
							player.hp = 0;
						}
					}

					/* move towards the player */
					newX = fsb->xPosition;
					newY = fsb->yPosition;
					if(player.X < fsb->xPosition)
					{
						newX--;
					}
					else if(player.X > fsb->xPosition)
					{
						newX++;
					}
					if(player.Y < fsb->yPosition)
					{
						newY--;
					}
					else if(player.Y > fsb->yPosition)
					{
						newY++;
					}
					/* if we've moved */
					if(newX != fsb->xPosition || newY != fsb->yPosition)
					{
						clearFSB(fsb);

						/* if the space is clear */
						if(!checkFSBCollision(newX, newY))
						{
							fsb->xPosition = newX;
							fsb->yPosition = newY;
						}
						else if(!checkFSBCollision(newX, fsb->xPosition))
						{
							fsb->xPosition = newX;
						}
						else if(!checkFSBCollision(fsb->yPosition, newY))
						{
							fsb->yPosition = newY;
						}
			
						drawFSB(fsb);
					}
					fsb->lastMoveTime = (int)getCurrentTime();
				}

				break;
			default:
				break;
			}

		}
		/* the miniboss just died, do stuff */
		else
		{
			fsb->alive = 0;
			fsbCount--;
			clearFSB(fsb);

			/* see if the minibosses are dead, if they are, open the door */
			if(fsbCount <= 0)
			{
				/* put down the switch for the weapon room */
				/* create a new switch position */
				LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, 'r');

				/* store it in the array */
				setLevelPosition(SWITCH_LAYER, fsb->xPosition, fsb->yPosition, switchPosition);

				clearLevelPositions(WALL_LAYER, 'G');
			}
		}
	}
}

/* load up the super tank 
	param: xPosition - X position to load tank at
		   yPosition - Y position to load tank at
*/
void loadSuperTank(int xPosition, int yPosition)
{
	/* set the position of the super tank */
	superTank.xPosition = xPosition;
	superTank.yPosition = yPosition;

	superTank.startingX = xPosition;
	superTank.startingY = yPosition;

	/* set the super tank to be alive */
	superTank.alive = 1;
	superTank.animationPaused = 0;

	/* set the frames up */
	superTank.currentFrame = 1;
	superTank.lastFrameTime = 0;
	superTank.currentState = SUPERTANK_NOTHING;
	superTank.stateSwitchTimer = 50000;
	superTank.lastTime = (int)getCurrentTime();

	/* create the enemy data */
	superTank.enemyStats = malloc(sizeof(ENEMY));
	superTank.enemyStats->health = 2000;
	superTank.lastHP = superTank.enemyStats->health;
	superTank.currentGun = gunCopy(&weaponHeavyMachineGun);
	superTank.currentGun->owner = OWNER_ENEMY;

	superTank.lastMoveTime = 0;
	superTank.moveDelay = MOVE_DELAY;

	superTank.lastSpawnTime = 0;
	superTank.spawnTimeLimit = 2000;
	superTank.spawnTotal = 24;
	superTank.spawnCount = 0;
	superTank.ramTotalWaitTime = 2000;
	superTank.ramWaiting = 0;

	/* create our level positions to be used when drawing the tank */
	superTank.superTankPosition = createLevelPosition(superTank.enemyStats, POSITION_ENEMY, '&');

	loadBossChars("bosses/level4/supertankSit.txt", "bosses/level4/supertankSitC.txt", 
		superTank.frame[0]);

	loadBossChars("bosses/level4/supertankAttack.txt", "bosses/level4/supertankAttackC.txt", 
		superTank.frame[1]);

	loadBossChars("bosses/level4/supertankHit.txt", "bosses/level4/supertankHitC.txt", 
		superTank.frame[2]);
	superTank.currentFrame = 0;
	drawSuperTank();
}

/* create a flying slashbot 
	param: fsbChar - character to associate with this FSB
		   xPosition - X position of the FSB
		   yPosition - Y position of the FSB
*/
FlyingSlashBot *createFSB(char fsbChar, int xPosition, int yPosition)
{
	FlyingSlashBot *newFSB = malloc(sizeof(FlyingSlashBot));

	/* set the position of the super tank */
	newFSB->xPosition = xPosition;
	newFSB->yPosition = yPosition;

	/* set the super tank to be alive */
	newFSB->alive = 1;
	newFSB->animationPaused = 1;

	/* set the frames up */
	newFSB->currentFrame = 1;
	newFSB->lastFrameTime = 0;
	newFSB->currentState = FSB_NOTHING;
	newFSB->lastTime = (int)getCurrentTime();

	/* create the enemy data */
	newFSB->enemyStats = malloc(sizeof(ENEMY));
	newFSB->enemyStats->health = 1000;
	newFSB->lastHP = newFSB->enemyStats->health;

	newFSB->lastMoveTime = 0;
	newFSB->moveDelay = 10;

	/* create our level positions to be used when drawing the tank */
	newFSB->fsbPosition = createLevelPosition(newFSB->enemyStats, POSITION_ENEMY, fsbChar);

	loadFSBChars("bosses/level4/fsbFly1.txt", "bosses/level4/fsbFly1C.txt", 
		newFSB->frame[0]);

	loadFSBChars("bosses/level4/fsbFly2.txt", "bosses/level4/fsbFly1C.txt", 
		newFSB->frame[1]);

	loadFSBChars("bosses/level4/fsbFly3.txt", "bosses/level4/fsbFly1C.txt", 
		newFSB->frame[2]);

	loadFSBChars("bosses/level4/fsbFly4.txt", "bosses/level4/fsbFly1C.txt", 
		newFSB->frame[3]);

	newFSB->currentFrame = 0;
	drawFSB(newFSB);

	/* increase the fsb count */
	fsbCount++;

	return newFSB;
}

/* redraw a FSB 
	param: fsb - the FSB to draw 
*/
void drawFSB(FlyingSlashBot *fsb)
{
	int x, y;
	/* put the level positions in the right spot */
	for(y = 0; y < FSB_HEIGHT; y++)
	{
		for(x = 0; x < FSB_WIDTH; x++)
		{
			if(fsb->frame[fsb->currentFrame][y][x].Char.AsciiChar != ' ')
			{
				setLevelPosition(ENEMY_LAYER, fsb->xPosition + x, fsb->yPosition + y, 
					fsb->fsbPosition);
			}
		}
	}
}

/* clear a FSB from the screen 
	param: fsb - the FSB to be cleared
*/
void clearFSB(FlyingSlashBot *fsb)
{
	int x, y;
	/* put the level positions in the right spot */
	for(y = 0; y < FSB_HEIGHT; y++)
	{
		for(x = 0; x < FSB_WIDTH; x++)
		{
			if(fsb->frame[fsb->currentFrame][y][x].Char.AsciiChar != ' ')
			{
				setLevelPosition(ENEMY_LAYER, fsb->xPosition + x, fsb->yPosition + y, 
					whiteSpacePosition);
			}
		}
	}
}

/* check collision for a FSB
	param: fsbX - the X position of the FSB to be checked 
		   fsbY - the Y position of the FSB to be checked
*/
int checkFSBCollision(int fsbX, int fsbY)
{
	int x, y;
	/* put the level positions in the right spot */
	for(y = 0; y < FSB_HEIGHT; y++)
	{
		for(x = 0; x < FSB_WIDTH; x++)
		{
			/*if(fsb->frame[fsb->currentFrame][y][x].Char.AsciiChar != ' ')*/
			{
				if(!isLevelPositionSpace(WALL_LAYER, fsbX + x,
					fsbY + y) || !isLevelPositionSpace(ENEMY_LAYER, 
					fsbX + x, fsbY + y))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

/* check collision betwen a FSB and the player 
	param: fsb - the FSB to check collision with
*/
int checkFSBCollisionPlayer(FlyingSlashBot *fsb)
{
	if(player.X >= fsb->xPosition - 1 && player.X <= fsb->xPosition + FSB_WIDTH
		&& player.Y >= fsb->yPosition - 1 && player.Y <= fsb->yPosition + FSB_HEIGHT)
	{
		return 1;
	}
	return 0;
}

/* update the super tank */
void updateSuperTank()
{
	/* make sure the tank is alive before we do anything else */
	if(superTank.alive)
	{
		if(superTank.enemyStats->health >= 0)
		{
			/* see what mode we're in */
			switch(superTank.currentState)
			{
			case SUPERTANK_SPAWN:	

				/* make sure we're set to the sitting frame */
				superTank.currentFrame = 0;

				/* shoot at the player */
				superTankShoot();

				/* invincible */
				if(superTankLostHealth())
				{
					superTank.enemyStats->health = superTank.lastHP;
				}
				
				/* see if its time to spawn some more enemies */
				if((getCurrentTime() - superTank.lastSpawnTime) >= superTank.spawnTimeLimit)
				{
					/* see if we've reached our spawn limit */
					if(superTank.spawnCount < superTank.spawnTotal)
					{
						/* spawn some enemies */
						spawnSubEnemy();
						superTank.lastSpawnTime = (int)getCurrentTime();
					}
					else
					{
						/* wait for the spawn to die */
						superTank.currentState = SUPERTANK_WAITING;

						/* reset the spawn counter to 0 */
						superTank.spawnCount = 0;
					}
				}

				break;
			case SUPERTANK_NOTHING:
				/* make sure that cheating player isn't shooting the
					tank while its still asleep */
				if(superTankLostHealth())
				{
					superTank.enemyStats->health = superTank.lastHP;
				}
				/* see if the player is close enough to start up */
				if(player.Y >= (superTank.yPosition - 19))
				{
					superTank.currentState = SUPERTANK_SPAWN;
					clearEnemies();
					
					/* set the text prompt */
					setTextPrompt("Watch out, the super tank is\nonly vulnerable when he\'s ramming!");

					/* set the boss checkpoint */
					player.atLevelFourBoss = 1;
				}
				break;
			case SUPERTANK_WAITING:
				
				/* make sure we're using the sitting frame */
				superTank.currentFrame = 0;

				/* shoot at the player */
				superTankShoot();

				/* invincible */
				if(superTankLostHealth())
				{
					superTank.enemyStats->health = superTank.lastHP;
				}

				/* see if all the bots are dead */
				if(currentJumpers <= 0)
				{
					/* change to our ramming sequence */
					/* reset up */
					superTank.moveDelay = MOVE_DELAY;
					superTank.currentState = SUPERTANK_RAM_RIGHT;
					superTank.ramWaiting = 0;
				}
				break;
			case SUPERTANK_RAM_FORWARD:
				
				/* do our flash/attack animation */
				superTankFlash();

				/* if we're not waiting */
				if(!superTank.ramWaiting)
				{
					superTankRamUp();
				}
				else
				{
					/* if we've waited long enough */
					if((getCurrentTime() - superTank.ramWaitStartTime)
						>= superTank.ramTotalWaitTime)
					{
						/* if we're not back in the center */
						if(superTank.xPosition != superTank.startingX || 
							superTank.yPosition != superTank.startingY)
						{
							superTankMoveToCenter();
						}
						/* else, go to the next state */
						else
						{
							/* reset speed */
							superTank.moveDelay = MOVE_DELAY;
							superTank.currentState = SUPERTANK_RAM_BACK;
							superTank.ramWaiting = 0;
						}
					}
				}
				break;
			case SUPERTANK_RAM_RIGHT:

				/* do our flash/attack animation */
				superTankFlash();

				/* if we're not waiting */
				if(!superTank.ramWaiting)
				{
					superTankRamRight();
				}
				else
				{
					/* if we've waited long enough */
					if((getCurrentTime() - superTank.ramWaitStartTime)
						>= superTank.ramTotalWaitTime)
					{
						/* if we're not back in the center */
						if(superTank.xPosition != superTank.startingX || 
							superTank.yPosition != superTank.startingY)
						{
							superTankMoveToCenter();
						}
						/* else, go to the next state */
						else
						{
							/* reset speed */
							superTank.moveDelay = MOVE_DELAY;
							superTank.currentState = SUPERTANK_RAM_LEFT;
							superTank.ramWaiting = 0;
						}
					}
				}
				break;
			case SUPERTANK_RAM_BACK:

				/* do our flash/attack animation */
				superTankFlash();

				/* if we're not waiting */
				if(!superTank.ramWaiting)
				{
					superTankRamBack();
				}
				else
				{
					/* if we've waited long enough */
					if((getCurrentTime() - superTank.ramWaitStartTime)
						>= superTank.ramTotalWaitTime)
					{
						/* if we're not back in the center */
						if(superTank.xPosition != superTank.startingX || 
							superTank.yPosition != superTank.startingY)
						{
							superTankMoveToCenter();
						}
						/* else, go to the next state */
						else
						{
							/* reset speed */
							superTank.moveDelay = MOVE_DELAY;
							superTank.currentState = SUPERTANK_SPAWN;
							/* superTank.lastHP = superTank.enemyStats->health; */
							superTank.ramWaiting = 0;
						}
					}
				}
				break;
			case SUPERTANK_RAM_LEFT:

				/* do our flash/attack animation */
				superTankFlash();

				/* if we're not waiting */
				if(!superTank.ramWaiting)
				{
					superTankRamLeft();
				}
				else
				{
					/* if we've waited long enough */
					if((getCurrentTime() - superTank.ramWaitStartTime)
						>= superTank.ramTotalWaitTime)
					{
						/* if we're not back in the center */
						if(superTank.xPosition != superTank.startingX || 
							superTank.yPosition != superTank.startingY)
						{
							superTankMoveToCenter();
						}
						/* else, go to the next state */
						else
						{
							/* reset the speed */
							superTank.moveDelay = MOVE_DELAY;
							superTank.currentState = SUPERTANK_RAM_FORWARD;
							superTank.ramWaiting = 0;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		/* the tank has just died, clear all its positions */
		else
		{	
			/* reset the boss waypoint */
			player.atLevelFourBoss = 0;

			/* open the out door */
			clearLevelPositions(WALL_LAYER, 'S');

			/* open the boss door */
			clearLevelPositions(WALL_LAYER, 'I');
			
			/* clear the super tank position */
			clearSuperTank();

			/* set the supertank to be dead */
			superTank.alive = 0;

			/* set the level four complete flag */
			player.levelFourComplete = 1;

			/* set the text prompt */
			setTextPrompt("Good job, you beat the super tank!");

			/* drop the weapon */
			if(!player.weaponsEnabled[GUN_ROCKETLAUNCHER])
			{
				LEVEL_POSITION *weapon_position = createLevelPosition(NULL, POSITION_WEAPON, '^');
				setLevelPosition(PICKUP_LAYER, superTank.xPosition, superTank.yPosition, weapon_position);
			}
		}

		/* save what hp the tank had this loop */
		superTank.lastHP = superTank.enemyStats->health;
	}
}

/* draw the super tank in its current state */
void drawSuperTank()
{
	int x, y;
	/* put the level positions in the right spot */
	for(y = 0; y < TANK_HEIGHT; y++)
	{
		for(x = 0; x < TANK_WIDTH; x++)
		{
			if(superTank.frame[superTank.currentFrame][y][x].Char.AsciiChar != ' ')
			{
				setLevelPosition(ENEMY_LAYER, superTank.xPosition + x, superTank.yPosition + y, 
					superTank.superTankPosition);
			}
		}
	}
}

/* clear the super tank from the screen */
void clearSuperTank()
{
	int x, y;
	/* put the level positions in the right spot */
	for(y = 0; y < TANK_HEIGHT; y++)
	{
		for(x = 0; x < TANK_WIDTH; x++)
		{
			if(superTank.frame[superTank.currentFrame][y][x].Char.AsciiChar != ' ')
			{
				setLevelPosition(ENEMY_LAYER, superTank.xPosition + x, superTank.yPosition + y, 
					whiteSpacePosition);
			}
		}
	}
}

/* loads character and color data for the boss from a file 
	param: fileName - file name of the boss' character data
		   colorName - file name of the boss' color data
		   cha - the char info array to store the data in 
*/
void loadBossChars(char *fileName, char *colorName,  CHAR_INFO cha[TANK_HEIGHT][TANK_WIDTH])
{
	/* load the files */
	FILE *levelFile;
	FILE *colorFile;

  /* holds all the lines we read in so we can store them individually */
	char lineBuffer[LONGEST_LINE] = {0};

  /* counters to get across the screen */
	int i = 0, j = 0;

	/* open the file in read-text mode */
	levelFile = fopen(fileName, "rt");

	/* read through the file once to find out the levels dimensions. */
	if(levelFile == NULL)
	{
		/* printf("Error: could not load level file "); */
	}
	else
	{
		/* fill in the new array with trivial characters, so there is always a default */
		/* go through each line */
		for(i = 0; i < TANK_HEIGHT; i++)
		{
			/* read in all the characters in the line and store their characer values in the array */
			for(j = 0; j < TANK_WIDTH; j++)
			{
        
				    cha[i][j].Char.AsciiChar = 'P';
					cha[i][j].Attributes = (FOREGROUND_RED);
			}
		}
  
		i = 0;

		/* loop through the entire file and read information and create our level */
		while((fgets(&lineBuffer[0], LONGEST_LINE, levelFile )!= NULL) && i < TANK_HEIGHT)
		{
			for(j = 0; j < TANK_WIDTH; j++)
			{
    				cha[i][j].Char.AsciiChar = lineBuffer[j];         
			}
			i++;
		}
  }
	/* close the file now that we are finished with it */
  fclose(levelFile);  
  
  /* we are repeating most of the process with the color file */
  colorFile = fopen(colorName, "rt");
  if(colorFile == NULL)
  {
		/* printf("Error: could not load level file "); */
  }
  else
  {
    /* we dont need to re-clear the screen, so that first loop is not present */
    i = 0;
		/* loop through the entire file and read information and create our level */
		while((fgets(&lineBuffer[0], LONGEST_LINE, colorFile )!= NULL) && (i < TANK_HEIGHT))
		{
			for(j = 0; j < TANK_WIDTH; j++)
			{
				switch (lineBuffer[j])
				{
				case 'b':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN);
					break;
				case 'B':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
					break;
				case 'V':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | BACKGROUND_BLUE);
					break;
				case 'r':
    						cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_INTENSITY |
								BACKGROUND_INTENSITY | BACKGROUND_RED);
					break;
				case 'R':
    						cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN);
					break;
				case 'g':
    						cha[i][j].Attributes = (FOREGROUND_GREEN);
					break;
				case 'G':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					break;
				case 'o':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				case 'O':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
					break;
				case 'p':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_RED);
					break;
				case 'P':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
					break;
				case 'y':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE);
					break;
				case 'Y':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					break;
				case 'w':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
					break;
				case 'W':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED| FOREGROUND_INTENSITY);
					break;
				}
			}
			i++;
		}
		fclose(colorFile);
  }
}

/* load the character data for a FSB
	param: fileName - the file name containing the character data
		   colorName - the file name containing the color data
		   cha - the char info array to store the data in 
*/
void loadFSBChars(char *fileName, char *colorName,  CHAR_INFO cha[FSB_HEIGHT][FSB_WIDTH])
{
	/* load the files */
	FILE *levelFile;
	FILE *colorFile;

  /* holds all the lines we read in so we can store them individually */
	char lineBuffer[LONGEST_LINE] = {0};

  /* counters to get across the screen */
	int i = 0, j = 0;

	/* open the file in read-text mode */
	levelFile = fopen(fileName, "rt");

	/* read through the file once to find out the levels dimensions. */
	if(levelFile == NULL)
	{
		/* printf("Error: could not load level file "); */
	}
	else
	{
		/* fill in the new array with trivial characters, so there is always a default */
		/* go through each line */
		for(i = 0; i < FSB_HEIGHT; i++)
		{
			/* read in all the characters in the line and store their characer values in the array */
			for(j = 0; j < FSB_WIDTH; j++)
			{
        
				    cha[i][j].Char.AsciiChar = 'P';
					cha[i][j].Attributes = (FOREGROUND_RED);
			}
		}
  
		i = 0;

		/* loop through the entire file and read information and create our level */
		while((fgets(&lineBuffer[0], LONGEST_LINE, levelFile )!= NULL) && i < FSB_HEIGHT)
		{
			for(j = 0; j < FSB_WIDTH; j++)
			{
    				cha[i][j].Char.AsciiChar = lineBuffer[j];         
			}
			i++;
		}
  }
	/* close the file now that we are finished with it */
  fclose(levelFile);  
  
  /* we are repeating most of the process with the color file */
  colorFile = fopen(colorName, "rt");
  if(colorFile == NULL)
  {
		/* printf("Error: could not load level file "); */
  }
  else
  {
    /* we dont need to re-clear the screen, so that first loop is not present */
    i = 0;
		/* loop through the entire file and read information and create our level */
		while((fgets(&lineBuffer[0], LONGEST_LINE, colorFile )!= NULL) && (i < FSB_HEIGHT))
		{
			for(j = 0; j < FSB_WIDTH; j++)
			{
				switch (lineBuffer[j])
				{
				case 'b':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN);
					break;
				case 'B':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
					break;
				case 'V':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | BACKGROUND_BLUE);
					break;
				case 'r':
    						cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_INTENSITY |
								BACKGROUND_INTENSITY | BACKGROUND_RED);
					break;
				case 'R':
    						cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN);
					break;
				case 'g':
    						cha[i][j].Attributes = (FOREGROUND_GREEN);
					break;
				case 'G':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					break;
				case 'o':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				case 'O':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
					break;
				case 'p':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_RED);
					break;
				case 'P':
    						cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
					break;
				case 'y':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE);
					break;
				case 'Y':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					break;
				case 'w':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
					break;
				case 'W':
    						cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED| FOREGROUND_INTENSITY);
					break;
				}
			}
			i++;
		}
		fclose(colorFile);
  }
}

/* get the character at a certain x and y of the super tank
	param: x - the x position of the tank
		   y - the y position of the tank
	returns: a char info struct containing the data at the position
*/
CHAR_INFO *getSuperTankChar(int x, int y)
{
	return &(superTank.frame[superTank.currentFrame][y][x]);
}

/* get the character at a certain x and y of a FSB
	param: fsb - the FSB to get the data for
		   x - the x position of the tank
		   y - the y position of the tank
	returns: a char info struct containing the data at the position
*/
CHAR_INFO *getFSBChar(FlyingSlashBot *fsb, int x, int y)
{
	return &(fsb->frame[fsb->currentFrame][y][x]);
}

/* spawn the sub enemies for the boss */
void spawnSubEnemy()
{
	int spawnPosX1, spawnPosY1, spawnPosX2, spawnPosY2, 
		spawnPosX3, spawnPosY3, spawnPosX4, spawnPosY4;

	/* creat a copy of a pointer to an ENEMY */
	ENEMY* copy1 = enemyCopy(&jumper);
	ENEMY* copy2 = enemyCopy(&jumper);
	ENEMY* copy3 = enemyCopy(&jumper);
	ENEMY* copy4 = enemyCopy(&jumper);

	 /* creat a new LEVEL_POSITION to hold an enemy */
	LEVEL_POSITION* new_pos1 = createLevelPosition(copy1, POSITION_ENEMY, 'z');
	LEVEL_POSITION* new_pos2 = createLevelPosition(copy2, POSITION_ENEMY, 'z');
	LEVEL_POSITION* new_pos3 = createLevelPosition(copy3, POSITION_ENEMY, 'z');
	LEVEL_POSITION* new_pos4 = createLevelPosition(copy4, POSITION_ENEMY, 'z');

	/* spawn some enemies */
	spawnPosX1 = superTank.xPosition + 8;
	spawnPosY1 = superTank.yPosition - 4;
	spawnPosX2 = superTank.xPosition + 12;
	spawnPosY2 = superTank.yPosition - 4;
	spawnPosX3 = superTank.xPosition + 8;
	spawnPosY3 = superTank.yPosition + TANK_HEIGHT + 4;
	spawnPosX4 = superTank.xPosition + 12;
	spawnPosY4 = superTank.yPosition + TANK_HEIGHT + 4;

	/* call createEnemy with the position of 1 to the right of the spawn */
	createEnemy(copy1, new_pos1, spawnPosX1, spawnPosY1);
	createEnemy(copy2, new_pos2, spawnPosX2, spawnPosY2);
	createEnemy(copy3, new_pos3, spawnPosX3, spawnPosY3);
	createEnemy(copy4, new_pos4, spawnPosX4, spawnPosY4);

	/* increase the spawn counter */
	superTank.spawnCount += 4;
	/* increment the current number of clawbots on the map */
	currentJumpers += 4;
}

/* make the super tank shoot at the player */
void superTankShoot()
{
	/* shoot at the player if they get in front of guns */
	if(player.X < (superTank.xPosition + 4) && player.Y >= superTank.yPosition 
		&& player.Y <= (superTank.yPosition + 6))
	{
		shoot(superTank.currentGun, -1, 0, superTank.xPosition + 3, superTank.yPosition + 4);
	}
	else if(player.X > (superTank.xPosition + TANK_WIDTH - 4)  && player.Y >= superTank.yPosition 
		&& player.Y <= (superTank.yPosition + 6))
	{
		shoot(superTank.currentGun, 1, 0, superTank.xPosition + TANK_WIDTH - 3, superTank.yPosition + 4);
	}
	/* shoot at the player if they get in front of guns */
	if(player.X < (superTank.xPosition + 4)  && player.Y >= (superTank.yPosition + 6) 
		&& player.Y <= (superTank.yPosition + TANK_HEIGHT - 1))
	{
		shoot(superTank.currentGun, -1, 0, superTank.xPosition + 3, superTank.yPosition + 10);
	}
	else if(player.X > (superTank.xPosition + TANK_WIDTH - 4)  && player.Y >= (superTank.yPosition + 6) 
		&& player.Y <= (superTank.yPosition + TANK_HEIGHT - 1))
	{
		shoot(superTank.currentGun, 1, 0, superTank.xPosition + TANK_WIDTH - 3, superTank.yPosition + 10);
	}
}

/* ram the super tank to the right */
void superTankRamRight()
{
	int newX, newY;
	int collision = 0;
	/* see if the move delay has passed */
	if((getCurrentTime() - superTank.lastMoveTime) >= superTank.moveDelay)
	{
		/* clear out the old tank drawing */
		clearSuperTank();

		/* change the tanks position */
		newX = superTank.xPosition + 1;
		newY = superTank.yPosition;

		/* check collision */
		collision = checkCollisionRight(newX, newY);

		/* see if we can move to the right */
		if(!collision)
		{
			superTank.xPosition = newX;
			superTank.yPosition = newY;

			/* store last move time */
			superTank.lastMoveTime = (int)getCurrentTime();
			
			/* speed up */
			superTank.moveDelay -= ST_ACCEL;
		}
		else
		{
			/* set the ram wait start time */
			superTank.ramWaitStartTime = (int)getCurrentTime();
			superTank.ramWaiting = 1;

			/* reset the speed */
			superTank.moveDelay = MOVE_DELAY;
		}
	}
	
	/* draw the tank */
	drawSuperTank();
}

/* ram the super tank to the left */
void superTankRamLeft()
{
	int newX, newY;
	int collision = 0;
	/* see if the move delay has passed */
	if((getCurrentTime() - superTank.lastMoveTime) >= superTank.moveDelay)
	{
		/* clear out the old tank drawing */
		clearSuperTank();

		/* change the tanks position */
		newX = superTank.xPosition - 1;
		newY = superTank.yPosition;

		/* check collision with the left */
		collision = checkCollisionLeft(newX, newY);

		/* see if we can move to the right */
		if(!collision)
		{
			superTank.xPosition = newX;
			superTank.yPosition = newY;

			/* store last move time */
			superTank.lastMoveTime = (int)getCurrentTime();
			
			/* speed up */
			superTank.moveDelay -= ST_ACCEL;
		}
		else
		{
			/* set the ram wait start time */
			superTank.ramWaitStartTime = (int)getCurrentTime();
			superTank.ramWaiting = 1;

			/* reset the speed */
			superTank.moveDelay = MOVE_DELAY;
		}
	}
	
	/* draw the tank */
	drawSuperTank();
}

/* ram the super tank up */
void superTankRamUp()
{
	int newX, newY;
	int collision = 0;
	/* see if the move delay has passed */
	if((getCurrentTime() - superTank.lastMoveTime) >= superTank.moveDelay)
	{
		/* clear out the old tank drawing */
		clearSuperTank();

		/* change the tanks position */
		newX = superTank.xPosition;
		newY = superTank.yPosition - 1;

		/* check collision with the left */
		collision = checkCollisionUp(newX, newY);

		/* see if we can move to the right */
		if(!collision)
		{
			superTank.xPosition = newX;
			superTank.yPosition = newY;

			/* store last move time */
			superTank.lastMoveTime = (int)getCurrentTime();
			
			/* speed up */
			superTank.moveDelay -= ST_ACCEL;
		}
		else
		{
			/* set the ram wait start time */
			superTank.ramWaitStartTime = (int)getCurrentTime();
			superTank.ramWaiting = 1;

			/* reset the speed */
			superTank.moveDelay = MOVE_DELAY;
		}
	}
	
	/* draw the tank */
	drawSuperTank();
}

/* ram the super tank down */
void superTankRamBack()
{
	int newX, newY;
	int collision = 0;
	/* see if the move delay has passed */
	if((getCurrentTime() - superTank.lastMoveTime) >= superTank.moveDelay)
	{
		/* clear out the old tank drawing */
		clearSuperTank();

		/* change the tanks position */
		newX = superTank.xPosition;
		newY = superTank.yPosition + 1;

		/* check collision with the left */
		collision = checkCollisionDown(newX, newY);

		/* see if we can move to the right */
		if(!collision)
		{
			superTank.xPosition = newX;
			superTank.yPosition = newY;

			/* store last move time */
			superTank.lastMoveTime = (int)getCurrentTime();
			
			/* speed up */
			superTank.moveDelay -= ST_ACCEL;
		}
		else
		{
			/* set the ram wait start time */
			superTank.ramWaitStartTime = (int)getCurrentTime();
			superTank.ramWaiting = 1;

			/* reset the speed */
			superTank.moveDelay = MOVE_DELAY;
		}
	}
	
	/* draw the tank */
	drawSuperTank();
}

/* move the super tank back to the center */
void superTankMoveToCenter()
{
	/* recenter the tank */
	int newX = superTank.xPosition, 
		newY = superTank.yPosition;
	if(superTank.xPosition < superTank.startingX)
	{
		newX = superTank.xPosition + 1;
	}
	else if(superTank.xPosition > superTank.startingX)
	{
		newX = superTank.xPosition - 1;
	}
	if(superTank.yPosition < superTank.startingY)
	{
		newY = superTank.yPosition + 1;
	}
	else if(superTank.yPosition > superTank.startingY)
	{
		newY = superTank.yPosition - 1;
	}

	if((getCurrentTime() - superTank.lastMoveTime) >= superTank.moveDelay)
	{
		/* see if theres any collisions */
		if(!checkCollisionRight(newX, newY) && !checkCollisionLeft(newX, newY)
			&& !checkCollisionUp(newX, newY) && !checkCollisionDown(newX, newY))
		{				
			clearSuperTank();

			superTank.xPosition = newX;
			superTank.yPosition = newY;

			/* store last move time */
			superTank.lastMoveTime = (int)getCurrentTime();

			/* speed up */
			superTank.moveDelay -= ST_ACCEL;
			
			drawSuperTank();
		}
	}
}

/* check collision of the super tank to the right
	param: newX - the new x of the tank to be checked
		   newY - the new y of the tank to be checked
   return: 1 if there is a collision, 0 if not
*/
int checkCollisionRight(int newX, int newY)
{
	/* some int vars */
	int collision = 0, i, oldPlayerPosX;
	/* check collision */
	for(i = 0; i < TANK_HEIGHT; i++)
	{
		if(isLevelPositionWall(WALL_LAYER, newX + TANK_WIDTH - 1, newY + i))
		{
			collision = 1;
		}
	}

	/* move the player over if we hit them */
	for(i = 0; i < TANK_HEIGHT; i++)
	{
		/* check for the inside parts */
		if((i > 2 && i < 6) || (i > 8 && i < 12))
		{
			if(getLevelPosition(PLAYER_LAYER,  newX + TANK_WIDTH - 5, newY + i)->type == POSITION_PLAYER)
			{
				oldPlayerPosX = player.X;
				plyrMvRight();
				/* see if the player moved */
				if(oldPlayerPosX == player.X)
				{
					/* they didn't move, which means they're between the boss and a wall, so
						we don't wanna move the boss over */
					collision = 1;
				}
				break;
			}
		}
		/* check for spike parts */
		else
		{
			if(getLevelPosition(PLAYER_LAYER,  newX + TANK_WIDTH - 1, newY + i)->type == POSITION_PLAYER)
			{
				player.hp -= ST_DAMAGE;
				if(player.hp < 0)
				{
					player.hp = 0;
				}
				oldPlayerPosX = player.X;
				plyrMvRight();
				/* see if the player moved */
				if(oldPlayerPosX == player.X)
				{
					/* they didn't move, which means they're between the boss and a wall, so
						we don't wanna move the boss over */
					collision = 1;
				}
				break;
			}
		}
	}

	return collision;
}
/* check collision of the super tank to the left
	param: newX - the new x of the tank to be checked
		   newY - the new y of the tank to be checked
   return: 1 if there is a collision, 0 if not
*/
int checkCollisionLeft(int newX, int newY)
{
	/* collision */
	int collision = 0, i, oldPlayerPosX;

	/* check collision */
	for(i = 0; i < TANK_HEIGHT; i++)
	{
		if(isLevelPositionWall(WALL_LAYER, newX, newY + i))
		{
			collision = 1;
		}
	}

	/* move the player over if we hit them */
	for(i = 0; i < TANK_HEIGHT; i++)
	{
		/* check for the inside parts */
		if((i > 2 && i < 6) || (i > 8 && i < 12))
		{
			if(getLevelPosition(PLAYER_LAYER,  newX + 4, newY + i)->type == POSITION_PLAYER)
			{
				oldPlayerPosX = player.X;
				plyrMvLeft();
				/* see if the player moved */
				if(oldPlayerPosX == player.X)
				{
					/* they didn't move, which means they're between the boss and a wall, so
						we don't wanna move the boss over */
					collision = 1;
				}
				break;
			}
		}
		/* check for spike parts */
		else
		{
			if(getLevelPosition(PLAYER_LAYER,  newX, newY + i)->type == POSITION_PLAYER)
			{
				player.hp -= ST_DAMAGE;
				if(player.hp < 0)
				{
					player.hp = 0;
				}
				oldPlayerPosX = player.X;
				plyrMvLeft();
				/* see if the player moved */
				if(oldPlayerPosX == player.X)
				{
					/* they didn't move, which means they're between the boss and a wall, so
						we don't wanna move the boss over */
					collision = 1;
				}
				break;
			}
		}
	}

	return collision;
}

/* check collision of the super tank to the top
	param: newX - the new x of the tank to be checked
		   newY - the new y of the tank to be checked
   return: 1 if there is a collision, 0 if not
*/
int checkCollisionUp(int newX, int newY)
{
	/* some int vars */
	int collision = 0, i, oldPlayerPosY;
	/* check collision */
	for(i = 0; i < TANK_WIDTH; i++)
	{
		if(isLevelPositionWall(WALL_LAYER, newX + i, newY))
		{
			collision = 1;
		}
	}

	/* move the player over if we hit them */
	for(i = 0; i < TANK_WIDTH; i++)
	{	
		/* check for the inside parts */
		if(((i >= 0 && i < 5) || (i > 15 && i < 20)))
		{
			if((getLevelPosition(PLAYER_LAYER,  newX + i, newY + 3)->type == POSITION_PLAYER)
				|| (getLevelPosition(PLAYER_LAYER, newX + i, newY + 9)->type == POSITION_PLAYER))
			{
				oldPlayerPosY = player.Y;
				plyrMvUp();
				/* see if the player moved */
				if(oldPlayerPosY == player.Y)
				{
					/* they didn't move, which means they're between the boss and a wall, so
						we don't wanna move the boss over */
					collision = 1;
				}
				break;
			}
		}
		/* check for spike parts */
		else
		{
			if(getLevelPosition(PLAYER_LAYER,  newX + i, newY)->type == POSITION_PLAYER)
			{
				player.hp -= ST_DAMAGE;
				if(player.hp < 0)
				{
					player.hp = 0;
				}
				oldPlayerPosY = player.Y;
				plyrMvUp();
				/* see if the player moved */
				if(oldPlayerPosY == player.Y)
				{
					/* they didn't move, which means they're between the boss and a wall, so
						we don't wanna move the boss over */
					collision = 1;
				}
				break;
			}
		}
	}

	return collision;
}

/* check collision of the super tank to the bottom
	param: newX - the new x of the tank to be checked
		   newY - the new y of the tank to be checked
   return: 1 if there is a collision, 0 if not
*/
int checkCollisionDown(int newX, int newY)
{
	/* some int vars */
	int collision = 0, i, oldPlayerPosY;
	/* check collision */
	for(i = 0; i < TANK_WIDTH; i++)
	{
		if(isLevelPositionWall(WALL_LAYER, newX + i, newY + TANK_HEIGHT - 1))
		{
			collision = 1;
		}
	}

	/* move the player over if we hit them */
	for(i = 0; i < TANK_WIDTH; i++)
	{
		if(getLevelPosition(PLAYER_LAYER,  newX + i, newY + TANK_HEIGHT - 1)->type == POSITION_PLAYER)
		{
			player.hp -= ST_DAMAGE;
			if(player.hp < 0)
			{
				player.hp = 0;
			}
			oldPlayerPosY = player.Y;
			plyrMvDown();
			/* see if the player moved */
			if(oldPlayerPosY == player.Y)
			{
				/* they didn't move, which means they're between the boss and a wall, so
					we don't wanna move the boss over */
				collision = 1;
			}
			break;
		}		
	}

	return collision;
}

/* determine if the super tank lost health since the last update */
int superTankLostHealth()
{
	if(superTank.enemyStats->health != superTank.lastHP)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* flash the super tank red when damaged */
void superTankFlash()
{
	/* make sure we're using the right frame */
	if(superTank.flash)
	{
		superTank.currentFrame = 2;
	}
	else
	{
		superTank.currentFrame = 1;
	}

	/* see if we've lost health, if so, set the flash */
	if(superTankLostHealth())
	{
		superTank.flash = 1;
		superTank.flashTime = (int)getCurrentTime();
	}
	/* we havn't lost life, see if we previously did and
		are still flashing */
	else
	{
		/* we're still flashing */
		if(superTank.flash)
		{
			/* see if we've flashed long enough */
			if((getCurrentTime() - superTank.flashTime) >= FLASH_TIME)
			{
				/* turn off the flashing */
				superTank.flash = 0;
			}
		}
	}
}
