/*! ************************************************************************
  \file LevelManager.c
  \author Sheridan Beebe
  \par email: sbeebe@digipen.edu
  \par Game: Toasted
  \date January 16th, 2007
  \brief

	This file contains all the source code for the level management
	functions. All methods are prototyped in LevelManager.h, and
	all types are declared there as well. The main purpose of this
	file and these functions is to provide the core functionality
	that will allow us to load and manage a level for game play.
    
***************************************************************************/

/* include the LevelManager.h header, contains everything we need. */
#include "LevelManager.h"
#include "player.h"
#include "EnemyManager.h"
#include "GraphicsSystem.h"
#include "LevelOne.h"
#include "LevelTwo.h"
#include "LevelFour.h"
#include "LevelFive.h"
#include "LevelThree.h"
#include "turret.h"
/*! \brief This function performs some necessary initialization.

   It must be called before the level system can ever be used! 

   \sa unloadLevelManager()
*/
void loadLevelManager()
{
	/* create a level position to be used by all whitespace */

	greenColorData = BACKGROUND_GREEN;
	shadowColorData = BACKGROUND_INTENSITY;
	greenIntenseColorData = BACKGROUND_GREEN;
	brownIntenseColorData = BACKGROUND_RED | BACKGROUND_GREEN;

	whiteSpacePosition = createLevelPosition(&greenColorData, POSITION_SPACE, ' ');
	
	/* create a level position to be used by all walls */
	wallPosition = createLevelPosition(NULL, POSITION_WALL, 'X');

	/* blank positions */
	blackSpacePosition = createLevelPosition(&greenColorData, POSITION_WALL, 'A');

	/* invalid position */
	invalidPosition = createLevelPosition(&greenColorData, POSITION_INVALID, 'A');

	/* walk way positions */
	walkWayPositionOne = createLevelPosition(&brownIntenseColorData, POSITION_UNDER, 'B');

	/* shadow positions */
	shadowPosition = createLevelPosition(&shadowColorData, POSITION_UNDER, 'S');
}

/*! \brief This function performs the necessary clean up for the level manager.

   It is important that this be called before the program exits. 
   However, the level manager must NOT be used once this has been called.
   If it is, in any way, we could end up with invalid pointers.
   All level's should be unloaded also before this is called. 

   \sa loadLevelManager()
*/
void unloadLevelManager()
{
	/* free the universal white space struct... */
	free(whiteSpacePosition);
	
	/* free the universal wall position struct... */
	free(wallPosition);

	/* free the black space */
	free(blackSpacePosition);
}

/*! \brief Load a new level

	This is the main function that should be called in order to load a new
	level. Loading a new level will set the level to be the current level.

   \param fileName	- A pointer to a character array that 
					contains the file name of the map to
					be loaded.
	\return Returns a pointer to the LEVEL that was loaded. It is best to not
					use this pointer directly, but instead, use getCurrentLevel()
					to obtain a pointer to the currentLevel (in most cases).
					This way you can be sure you don't use an invalid pointer later.
	\sa unloadLevel()
*/
LEVEL* loadLevel(char *fileName, char *underFileName, int levelNumber)
{
	/* load the file */
	FILE *levelFile = NULL, *underFile = NULL;

	/* line buffer to hold in each line as we read it */
	char lineBuffer[LONGEST_LINE] = {0};
	char underLineBuffer[LONGEST_LINE] = {0};

	int doUnder = 0;

	/* this will hold the longest line we've read - to calculate level width */
	int longestLine = 0;

	/* this will count the number of lines in the file - to calculate the level
		height */
    int lineCount = 0;

	/* this will track the current y value, starts at the top, so 0. */
	int currentY = 0;

	/* these values used for setting all the spaces to white spaces initial */
	int layerCounter = 0, xCounter = 0, yCounter = 0;

	/* time to load a level, woot. */
	/* create a new LEVEL for the level we are loading */
	LEVEL *level = malloc(sizeof(LEVEL));

	/* find out if we need to load the "under" map. */
	if(strcmp("", underFileName) != 0)
	{
		doUnder = 1;
	}

	/* fill in some starting info, the file name for instance. */
	strcpy(&level->fileName[0], fileName);

	/* open the file in read-text mode */
	levelFile = fopen(level->fileName, "rt");

	/* open the under file if needed */
	if(doUnder)
	{
		underFile = fopen(underFileName, "rt");
	}

	/* read through the file once to find out the levels dimensions. */
	if(levelFile == NULL || (doUnder && underFile == NULL))
	{
		/* printf("Error: could not load level file "); */
	}
	else
	{
		/* loop through the entire file and read information on its size and such */
		while(fgets(&lineBuffer[0], LONGEST_LINE, levelFile) != NULL)
		{
			/* store this lines length */
			int lineLength = 0;
			lineLength = (int)strlen(&lineBuffer[0]) - 1;

			/* increase the line count */
			lineCount++;

			/* check to see if this line's length is the longest */
			if(lineLength > longestLine)
			{
				/* set the new longest line length to this line's length */
				longestLine = lineLength;
			}
		}

		/* now that we know the level's width and height, store it */
		level->width = longestLine;
		level->height = lineCount;

		/* set this level to be the current level */
		setCurrentLevel(level);

		/* now that we know the level's size, we need to create an array to store each position */
		level->firstPosition = (LEVEL_POSITION**)malloc(sizeof(LEVEL_POSITION*) * level->width * level->height
			* LAYER_COUNT);

		/* now that we have the space, fill it in with each character */

		/* we'll initialy fill it with spaces. this way we don't have to worry about it later */
		/* loop through every layer */
		for(layerCounter = 0; layerCounter < LAYER_COUNT; layerCounter++)
		{
			/* loop through every line */
			for(yCounter = 0; yCounter < level->height; yCounter++)
			{
				/* loop through each character */
				for(xCounter = 0; xCounter < level->width; xCounter++)
				{
					setLevelPosition(layerCounter, xCounter, yCounter, whiteSpacePosition);
				}
			}
		}
		
		/* rewind the file to the beginning */
		rewind(levelFile);

		/* loop through the entire file and read information and create our level */
		while(fgets(&lineBuffer[0], LONGEST_LINE, levelFile) != NULL)
		{
			int currentX;

			/* if we're loading an under map, read that data in as well */
			if(doUnder)
			{
				fgets(&underLineBuffer[0], LONGEST_LINE, underFile);
			}

			/* keep track of the current x position */
			for(currentX = 0; currentX < level->width; currentX++)
			{
				/* check to see what this space contains. */
				if(lineBuffer[currentX] == ' ')
				{
					/* the positions are already full of white spaces, so theres no need
						to do anything here. */
				}
				else if(lineBuffer[currentX] == 'X')
				{
					/* theres a wall here, set the proper level position to a wall */
					setLevelPosition(WALL_LAYER, currentX, currentY, wallPosition);
				}
				else if(lineBuffer[currentX] == 'C')
				{
					/* create a new LEVEL_POSITION for this enemy */
					LEVEL_POSITION *enemyPosition = createLevelPosition(NULL, POSITION_SPAWN, 'C');

					/* call createSpawn with the correct type of enemy to spawn, position
					  and create a new LEVEL_POSITION */
					createSpawn(spawnCopy(&claw_spawn), enemyPosition, currentX, currentY);

					/* put the enemyPosition into the correct position */
					setLevelPosition(ENEMY_LAYER, currentX, currentY, enemyPosition);

					/* theres an enemy here, call the proper create function */
					/* printf("Found enemy at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == 'Z')
				{
					/* create a new LEVEL_POSITION for this enemy */
					LEVEL_POSITION *enemyPosition = createLevelPosition(NULL, POSITION_SPAWN, 'Z');

					/* call createSpawn with the correct type of enemy to spawn, position
					  and create a new LEVEL_POSITION */
					createSpawn(spawnCopy(&jumper_spawn), enemyPosition, currentX, currentY);

					/* put the enemyPosition into the correct position */
					setLevelPosition(ENEMY_LAYER, currentX, currentY, enemyPosition);

					/* theres an enemy here, call the proper create function */
					/* printf("Found enemy at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == 'T')
				{
					/* create a new LEVEL_POSITION for this enemy */
					LEVEL_POSITION *enemyPosition = createLevelPosition(NULL, POSITION_SPAWN, 'T');

					/* call createSpawn with the correct type of enemy to spawn, position
					  and create a new LEVEL_POSITION */
					createSpawn(spawnCopy(&shooter_spawn), enemyPosition, currentX, currentY);

					/* put the enemyPosition into the correct position */
					setLevelPosition(ENEMY_LAYER, currentX, currentY, enemyPosition);

					/* theres an enemy here, call the proper create function */
					/* printf("Found enemy at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == 'Y')
				{
					/* create a new LEVEL_POSITION for this enemy */
					LEVEL_POSITION *enemyPosition = createLevelPosition(NULL, POSITION_SPAWN, 'Y');

					/* call createSpawn with the correct type of enemy to spawn, position
					  and create a new LEVEL_POSITION */
					createSpawn(spawnCopy(&sniper_spawn), enemyPosition, currentX, currentY);

					/* put the enemyPosition into the correct position */
					setLevelPosition(ENEMY_LAYER, currentX, currentY, enemyPosition);

					/* theres an enemy here, call the proper create function */
					/* printf("Found enemy at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == 'B')
				{
					/* create a new LEVEL_POSITION for this enemy */
					LEVEL_POSITION *enemyPosition = createLevelPosition(NULL, POSITION_SPAWN, 'B');

					/* call createSpawn with the correct type of enemy to spawn, position
					  and create a new LEVEL_POSITION */
					createSpawn(spawnCopy(&bigburly_spawn), enemyPosition, currentX, currentY);

					/* put the enemyPosition into the correct position */
					setLevelPosition(ENEMY_LAYER, currentX, currentY, enemyPosition);

					/* theres an enemy here, call the proper create function */
					/* printf("Found enemy at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == 'Q')
				{
					/* create a new weapon position */
           
					createMauler(currentX, currentY);

					/* store it in the array */
					

				}
				/* found the super tank boss, load it here */
				else if(lineBuffer[currentX] == '&')
				{           
					/* call the load function */
					loadSuperTank(currentX, currentY);
				}
				else if(lineBuffer[currentX] == '?')
				{           
					/* call the load function */
				  loadRail(currentX, currentY);
				}
				else if(lineBuffer[currentX] == '=')
				{           
					/* call the load function */
					fsbOne = createFSB('=', currentX, currentY);
				}
				else if(lineBuffer[currentX] == 'K')
				{           
					/* call the load function */
					fsbTwo = createFSB('K', currentX, currentY);
				}
				else if(lineBuffer[currentX] == 'o')
				{
					/* call the load function */
					fsbThree = createFSB('o', currentX, currentY);
				}
				else if(lineBuffer[currentX] == 'k')
				{           
					/* call the load function */
					fsbFour = createFSB('k', currentX, currentY);
				}
				else if(lineBuffer[currentX] == '<')
				{           
					turret_create (currentX, currentY, 200, 7000, 1);
				}
				else if(lineBuffer[currentX] == '>')
				{           
					turret_create (currentX, currentY, 200, 7000, 7);
				}
				else if(lineBuffer[currentX] == '$')
				{           
					/* call the load function */
					loadMainBoss(currentX, currentY);
				}

				else if(lineBuffer[currentX] == '+')
				{           
					/* call the load function */
					loadWarper(currentX, currentY);
				}

				/* found the level one boss, load it here */
				else if(lineBuffer[currentX] == '#')
				{           
					/* call the load function */
					loadLevelOneBoss(currentX, currentY);
				}

				/* found the level one mini boss, load it here */
				else if(lineBuffer[currentX] == 'm')
				{           
					/* call the load function */
					loadLevelOneMiniBoss(currentX, currentY);
				}

				/* found the level two mini boss, load it here */
				else if(lineBuffer[currentX] == 'n')
				{           
					/* call the load function */
					loadLevelTwoMiniBoss(currentX, currentY);
				}

				/* found the level two mini boss, load it here */
				else if(lineBuffer[currentX] == 'a')
				{           
					/* call the load function */
					loadLevelTwoBoss(currentX, currentY);
				}

				else if(lineBuffer[currentX] == 'H')
				{
					/* theres a health pack here, call the proper create function */

					/* create a new health position */
					LEVEL_POSITION *healthPosition = createLevelPosition(NULL, POSITION_HEALTH, 'H');

					/* store it in the array */
					setLevelPosition(HEALTH_LAYER, currentX, currentY, healthPosition);

					/* printf("Found health pack at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == 'P')
				{
					if((!player.atLevelOneBoss && levelNumber == 1) ||
						(!player.atLevelTwoBoss && levelNumber == 2) ||
						(!player.atLevelThreeBoss && levelNumber == 3) ||
						(!player.atLevelFourBoss && levelNumber == 4) ||
						(!player.atLevelFiveBoss && levelNumber == 5) ||
						(levelNumber == 0))
					{
						player.X = currentX;
						player.Y = currentY;

						/* create a new player position */
						playerPosition = createLevelPosition(&player, POSITION_PLAYER, 'P');

						/* store it in the array */
						setLevelPosition(PLAYER_LAYER, player.X, player.Y, playerPosition);
					}
				}
				else if(lineBuffer[currentX] == 'p')
				{
					if((player.atLevelOneBoss && levelNumber == 1) ||
						(player.atLevelTwoBoss && levelNumber == 2) ||
						(player.atLevelThreeBoss && levelNumber == 3) ||
						(player.atLevelFourBoss && levelNumber == 4) ||
						(player.atLevelFiveBoss && levelNumber == 5))
					{
						player.X = currentX;
						player.Y = currentY;

						/* create a new player position */
						playerPosition = createLevelPosition(&player, POSITION_PLAYER, 'P');

						/* store it in the array */
						setLevelPosition(PLAYER_LAYER, player.X, player.Y, playerPosition);
						
						if(levelNumber == 5)
						{
							/* special case, remove the level 5 wall so they can go back to omega
								laser if they need */
							clearLevelPositions(WALL_LAYER, 'R');
						}
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == 'W')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_DUALPISTOL] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, 'W');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == ']')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_SPREADGUN] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, ']');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == ')')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_SCATTERGUN] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, ')');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == '[')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_SUBMACHINEGUN] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, '[');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == '(')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_HEAVYMACHINEGUN] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, '(');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == '\"')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_RIFLE] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, '\"');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == '\'')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_SNIPER] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, '\'');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == '^')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_ROCKETLAUNCHER] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, '^');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* load weapons */
				else if(lineBuffer[currentX] == '~')
				{
					/* make sure they don't have the gun yet */
					if(player.weaponsEnabled[GUN_OMEGALASER] == 0)
					{
						/* create a new weapon position */
						LEVEL_POSITION *weaponPosition = createLevelPosition(NULL, POSITION_WEAPON, '~');

						/* store it in the array */
						setLevelPosition(PICKUP_LAYER, currentX, currentY, weaponPosition);

						/* theres a weapon here, call the proper create function */
						/* printf("Found weapon at (%i,%i). Creating...\n", currentX, currentY); */
					}
				}
				/* draw black spaces */
				else if(lineBuffer[currentX] == 'A')
				{
					/* store it in the array */
					setLevelPosition(WALL_LAYER, currentX, currentY, blackSpacePosition);
				}
				
				/* switches */
				else if(lineBuffer[currentX] == 'S')
				{
					/* create a new switch wall position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_WALL, 'S');

					/* store it in the array */
					setLevelPosition(WALL_LAYER, currentX, currentY, switchPosition);
				}
				else if(lineBuffer[currentX] == 's')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, 's');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);
				}
				
				/* Q wall */
				else if(lineBuffer[currentX] == 'U')
				{
					/* create a new switch wall position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_WALL, 'U');

					/* store it in the array */
					setLevelPosition(WALL_LAYER, currentX, currentY, switchPosition);
				}
				/* q switch */
				else if(lineBuffer[currentX] == 'u')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, 'u');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);
				}
				
				/* R wall */
				else if(lineBuffer[currentX] == 'R')
				{
					/* create a new switch wall position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_WALL, 'R');

					/* store it in the array */
					setLevelPosition(WALL_LAYER, currentX, currentY, switchPosition);
				}
				/* r switch */
				else if(lineBuffer[currentX] == 'r')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, 'r');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);
				}
				
				/* V wall */
				else if(lineBuffer[currentX] == 'V')
				{
					/* create a new switch wall position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_WALL, 'V');

					/* store it in the array */
					setLevelPosition(WALL_LAYER, currentX, currentY, switchPosition);
				}
				/* v switch */
				else if(lineBuffer[currentX] == 'v')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, 'v');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);
				}

				/* this is an invisible wall switch so we hide it on the switch layer when loaded */
				else if(lineBuffer[currentX] == 'I')
				{
					/* create a new switch wall position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_WALL, 'I');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);
				}

				/* this is an invisible wall switch so we hide it on the switch layer when loaded */
				else if(lineBuffer[currentX] == 'G')
				{
					/* create a new switch wall position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_WALL, 'G');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);
				}

				/* these are invisible switches for the walls created with I characters */
				else if(lineBuffer[currentX] == 'i')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, 'i');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);
				}
				/* these are invisible switches for the walls created with I characters */
				else if(lineBuffer[currentX] == 'g')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, 'g');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);
				}

				else if(lineBuffer[currentX] == '0')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, '0');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);

					/* theres a switch here, call the proper create function */
					/* printf("Found switch at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == '1')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, '1');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);

					/* theres a switch here, call the proper create function */
					/* printf("Found switch at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == '2')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, '2');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);

					/* theres a switch here, call the proper create function */
					/* printf("Found switch at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == '3')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, '3');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);

					/* theres a switch here, call the proper create function */
					/* printf("Found switch at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == '4')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, '4');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);

					/* theres a switch here, call the proper create function */
					/* printf("Found switch at (%i,%i). Creating...\n", currentX, currentY); */
				}
				else if(lineBuffer[currentX] == '5')
				{
					/* create a new switch position */
					LEVEL_POSITION *switchPosition = createLevelPosition(NULL, POSITION_SWITCH, '5');

					/* store it in the array */
					setLevelPosition(SWITCH_LAYER, currentX, currentY, switchPosition);

					/* theres a switch here, call the proper create function */
					/* printf("Found switch at (%i,%i). Creating...\n", currentX, currentY); */
				}

				/* check the under file */
				if(doUnder)
				{
					switch(underLineBuffer[currentX])
					{	
					case ' ':
						break;
					case 'B':
						setLevelPosition(UNDER_LAYER, currentX, currentY, walkWayPositionOne);
						break;
					case 'S':
						setLevelPosition(UNDER_LAYER, currentX, currentY, shadowPosition);
						break;
					}
				}
			}
			
			/* increment the y counter */
			currentY++;
		}

		/* close the file now that we are finished with it */
		fclose(levelFile);
	}

	return level;
}

/*! \brief This function is called to unload a level.

	The current level should always be unloaded before a new one
	is loaded, to ensure no memory leeks are created.

	\param level - A pointer to the level to be unloaded.
	\sa loadLevel()
*/
void unloadLevel(LEVEL *level)
{
	/* Set the current level to NULL, to be on the safe side. This way,
		if anyone tries to use the level pointer when they should not,
		the program will (for sure) crash. Otherwise, it may or may not.
	*/
	setCurrentLevel(NULL);

	/* free the memory */
	free(level);
}

/*! \brief Get a pointer to the struct containing the info for a given level location.

	This is the main method used to determine what character is at a certain position.
	\param layerNumber - The layer number you wish to retrieve the LEVEL_POSITION for.
	\param x - The x position of the LEVEL_POSITION to be retrieved.
	\param y - The y position of the LEVEL_POSITION to be retrieved.
	\return A pointer to the LEVEL_POSITION located at the given values.
	\sa setLevelPosition()
*/
LEVEL_POSITION *getLevelPosition(int layerNumber, int x, int y)
{
	/* get the current level */
	LEVEL *level = getCurrentLevel();

	/* check to make sure the x and y are in the level's bounds,
	   bad things could happen if they werent. */
	if(x < level->width && y < level->height && layerNumber < LAYER_COUNT && x > 0 && y > 0)
	{
		/* calculate the index value based on the x and y. */
		int index = getPositionIndex(layerNumber, x, y);
		LEVEL_POSITION *tmp = level->firstPosition[index];
		return tmp;
	}
	else
	{
		return invalidPosition;
	}
}

/*! \brief Set the pointer to the LEVEL_POSITION containing the info for the given level location.

	This is the main method used to set the LEVEL_POSITION data for the positions in the LEVEL.

	\param layerNumber - The layer number you wish to set the LEVEL_POSITION for.
	\param x - The x value of the LEVEL_POSITION you wish to set.
	\param y - The y value of the LEVEL_POSITION you wish to set.
	\param newLevelPosition - A pointer to the LEVEL_POSITION you wish to set at the given
							location.
	\sa getLevelPosition()
*/
void setLevelPosition(int layerNumber, int x, int y, LEVEL_POSITION *newLevelPosition)
{
	/* get a pointer to the current level */
	LEVEL *level = getCurrentLevel();

	if(x < level->width && y < level->height && layerNumber < LAYER_COUNT && x > 0 && y > 0 )
	{
		/* calculate the index of the supplied x and y */
		int index = getPositionIndex(layerNumber, x, y);

		/* set the correct index to the supplied level position */
		level->firstPosition[index] = newLevelPosition;
	}
	else
	{
		/* printf("Error setting the level Position!\n"); */
	}
}

/*! \brief Quick convenience method to determine whether a location contains a space.
	
	This method is just a quick way to test to see if a certain level position is a space,
	since this will likely be needed a lot.

	\param layerNumber - The layer number of the LEVEL_POSITION you wish to check.
	\param x - The x position of the LEVEL_POSITION you wish to check.
	\param y - The y position of the LEVEL_POSITION you wish to check.
	\return Returns whether this position contains a space. 1 for yes, 0 for no.
	\sa isLevelPositionWall(), getLevelPosition()
*/
int isLevelPositionSpace(int layerNumber, int x, int y)
{
	/* call getLevelPosition, then return whether it was a space. */
	LEVEL_POSITION *position = getLevelPosition(layerNumber, x, y);
	if(position->type == POSITION_SPACE)
	{
		/* return yes */
		return 1;
	}
	else
	{
		/* return no */
		return 0;
	}
}

int isLevelPositionCollision(LEVEL * level, int layerNumber, int x, int y)
{
  LEVEL_POSITION *position;
  if((x >= level->width) || (y >= level->height) || (x < 0) || (y < 0))
  {
    return 0;
  }
	/* call getLevelPosition, then return whether it was a space. */
	position = getLevelPosition(layerNumber, x, y);
	if(position->character == ' ')
	{
		/* return yes */
		return 0;
	}
	else
	{
		/* return no */
		return 1;
	}
}


/*! \brief Quick convenience method to determine whether a location contains a space.
	
	This method is just a quick way to test to see if ANY location is a space

	\param x - The x position of the LEVEL_POSITION you wish to check.
	\param y - The y position of the LEVEL_POSITION you wish to check.
	\return Returns whether this position contains a space. 1 for yes, 0 for no.
*/
int isLevelPositionSpaceAll(int x, int y)
{
  int i;

  for (i = 0; i < LAYER_COUNT; i++)
  {
    if (!isLevelPositionSpace(i, x, y) && i != UNDER_LAYER)
	  return 0;
  }

  return 1;
}

/*! \brief Quick convenience method returns whether a location contains a wall.

	This is just a quick way to determine if a specified position is a wall, since
	this is likely to be needed a lot.

	\param layerNumber - The layer number of the LEVEL_POSITION you wish to check.
	\param x - The x position of the LEVEL_POSITION you wish to check.
	\param y - The y position of the LEVEL_POSITION you wish to check.
	\return Returns whether this position contains a wall. 1 for yes, 0 for no.
	\sa isLevelPositionSpace(), getLevelPosition()
*/
int isLevelPositionWall(int layerNumber, int x, int y)
{
	/* call getLevelPosition, then return whether it was a wall. */
	LEVEL_POSITION *position = getLevelPosition(layerNumber, x, y);
	if(position->type == POSITION_WALL)
	{
		/* return yes */
		return 1;
	}
	else
	{
		/* return no */
		return 0;
	}
}

/*! \brief Returns a pointer to the current level that is being used by the game.

	This method is used to get a pointer to the current level. Usually, this is the
	safest method since you won't be tryint to use an outdated level.

	\return A pointer to the current level in use.
	\sa setCurrentLevel()
*/
LEVEL *getCurrentLevel()
{
	/* return the pointer to the current level */
	return currentLevel;
}

/*! \brief Method for creating a new LEVEL_POSITION.

	This is an easy helper method to be used when creating a new LEVEL_POSITION 
	it uses malloc() so that it won't be released when it leaves scope. This
	means you MUST free it some time later. 
	
	\param data - A pointer to the data to be stored with the new LEVEL_POSITION.
				It can be used for what ever use you need, usualy for storing a
				pointer to this particular items other data, such as for an enemy
				or weapon.
	\param type - The type of object that is in this position, as defined by its
				POSITION_TYPE.
	\param character - The character to be displayed in this LEVEL_POSITION.
	\return A pointer to the newly created LEVEL_POSITION.
	\sa getLevelPosition(), setLevelPosition()
*/
LEVEL_POSITION *createLevelPosition(void *data, POSITION_TYPE type, char character)
{
	/* set asside the memory for the level position */
	LEVEL_POSITION *levelPosition = (LEVEL_POSITION*)malloc(sizeof(LEVEL_POSITION));

	/* ste the values to those that were passed in */
	levelPosition->data = data;
	levelPosition->type = type;
	levelPosition->character = character;

	/* return a pointer to the new level position */
	return levelPosition;
}

/*! \brief Sets the current level that the game should use.

	This is the main way to set the current level to be used. It should be
	noted that in most cases, calling this method directly is uneccesary, as 
	the loadLevel() function calls setCurrentLevel() itself.

	\param level - A pointer to the LEVEL to be set as the current level.
	\sa getCurrentLevel(), loadLevel()
*/
void setCurrentLevel(LEVEL *level)
{
	currentLevel = level;
}

/*! \brief This function calculates the index in the position pointer array
   that the specified layer x and y value is at. 
	
	This function is used to determine the location of a given LEVEL_POSITION
	based on the layer number, x, and y values. For the most part, this method
	should never be used directly. The functions getLevelPosition() and setLevelPosition()
	should provide a sufficient wrapper.

	\param layerNumber - The layer number of the LEVEL_POSITION index to be calculated.
	\param x - The x position of the LEVEL_POSITION index to be calculated.
	\param y - The y position of the LEVEL_POSITION index to be calculated.
	\return The index of the LEVEL_POSITION calculated by the input.
	\sa getLevelPosition(), setLevelPosition()
*/
int getPositionIndex(int layerNumber, int x, int y)
{
	/* get the current level */
	LEVEL *level = getCurrentLevel();

	/* calculate where in the square it is */
	int positionIndex = (level->width * y) + x;

	/* offset it by the layer number */
	positionIndex += (layerNumber * level->width * level->height);

	return positionIndex;
}

/*! \brief Fills in a space with all whiteSpacePostitions
	
	\param x - The x position of the LEVEL_POSITION index to be filled in.
	\param y - The y position of the LEVEL_POSITION index to be filled in.
	\sa fillspace(int x, int y)
*/

/* ** edit Chris */
void fillspace(int x, int y)
{
	/* lol copied from loadLevel() */
	int layerCounter;
	for(layerCounter = 0; layerCounter < LAYER_COUNT; layerCounter++)
	{
		setLevelPosition(layerCounter, x, y, whiteSpacePosition);
	}
}

void loadchars(char *fileName, char *colorName,  CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH])
{
	/* load the files */
	FILE *levelFile;
	FILE *colorFile;

  /* holds all the lines we read in so we can store them individually */
	char lineBuffer[CONSOLE_WIDTH] = {0};

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
		for(i = 0; i < CONSOLE_HEIGHT; i++)
		{
			/* read in all the characters in the line and store their characer values in the array */
			for(j = 0; j < CONSOLE_WIDTH; j++)
			{
        
				    cha[i][j].Char.AsciiChar = ' ';
            cha[i][j].Attributes = (FOREGROUND_RED);
			}
		}
  
    i = 0;

		/* loop through the entire file and read information and create our level */
		while((fgets(&lineBuffer[0], LONGEST_LINE, levelFile )!= NULL))
		{
			int j;
      if( i >= CONSOLE_HEIGHT)
      {
        break;
      }

			for(j = 0; j < CONSOLE_WIDTH; j++)
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
		while((fgets(&lineBuffer[0], LONGEST_LINE, colorFile )!= NULL) && (i < CONSOLE_HEIGHT))
		{
			int j;
			for(j = 0; j < CONSOLE_WIDTH; j++)
			{
        switch (lineBuffer[j])
        {
        case 'b':
    				cha[i][j].Attributes = (FOREGROUND_BLUE);
            break;
        case 'B':
    				cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case 'r':
    				cha[i][j].Attributes = (FOREGROUND_RED);
            break;
        case 'R':
    				cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_INTENSITY);
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
  }
}

void clearLevelPositions(int layerNumber, char clearChar)
{
	/* set up counters */
	int yCounter, xCounter;
	LEVEL *level = getCurrentLevel();

	/* loop through every line */
	for(yCounter = 0; yCounter < level->height; yCounter++)
	{
		/* loop through each character */
		for(xCounter = 0; xCounter < level->width; xCounter++)
		{
			/* if this position contains the character to be cleared */
			if(getLevelPosition(layerNumber, xCounter, yCounter)->character == clearChar)
			{
				setLevelPosition(layerNumber, xCounter, yCounter, whiteSpacePosition);
			}
		}
	}
}


void switchCharacterLayers(int startingLayer, int endingLayer, char switchChar)
{
	/* set up counters */
	int yCounter, xCounter;
	LEVEL *level = getCurrentLevel();

	/* loop through every line */
	for(yCounter = 0; yCounter < level->height; yCounter++)
	{
		/* loop through each character */
		for(xCounter = 0; xCounter < level->width; xCounter++)
		{
			/* store the level position here */
			LEVEL_POSITION *currentLevelPosition = getLevelPosition(startingLayer, xCounter, yCounter);
			/* if this position contains the character to be cleared */
			if(currentLevelPosition->character == switchChar)
			{
				/* clear the character from the starting layer*/
				setLevelPosition(startingLayer, xCounter, yCounter, whiteSpacePosition);

				/* add the character to the ending layer */
				setLevelPosition(endingLayer, xCounter, yCounter, currentLevelPosition);
			}
		}
	}
}
