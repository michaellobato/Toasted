/*! ************************************************************************
\file player.c
\author Sheridan Beebe and Chris Howard
\par email: choward@digipen.edu
\par Game: Toasted
\date Feburuary 6th, 2007
\brief  contains all of the relevant actions of the player
such as shooting, moving, changing weapons, dying, colliding 
and much more. All player actions are routed through these
functions
***************************************************************************/

#include "player.h"
#include "stdio.h"
#include "LevelManager.h"
#include "GraphicsSystem.h"
#include "timer.h"
#include "hud.h"
#include "Sound.h"

char getPlyrCollidedWith()
{
  char holder = getLevelPosition(WALL_LAYER, player.X, player.Y)->character;

	if(holder != ' ')
	{
		return holder;
	}
	holder = getLevelPosition(PICKUP_LAYER, player.X, player.Y)->character;
	if(holder != ' ')
	{
		return holder;
	}
	holder = getLevelPosition(WEAPON_LAYER, player.X, player.Y)->character;
	if(holder != ' ')
	{
		return holder;
	}

	return holder;
}

/* called every loop to update the player */
void updatePlayer()
{
	/* if the player is using the omega laser, and is currently firing,
		make sure the omega laser sound is playing */
	if(player.currentWeapon == GUN_OMEGALASER && 
		(keyboard.Sdown || keyboard.Sleft || keyboard.Sright || keyboard.Sup))
	{
		pauseSound(&soundLaser, 0);
	}
	else
	{
		pauseSound(&soundLaser, 1);
	}

	if((int)getCurrentTime() < (player.timer+50))
	{
		player.color.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		player.hpholder = player.hp;
	}
	else
	{
		player.color.Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		if(player.hp != player.hpholder)
		{
			player.timer = (int)getCurrentTime();
		}
	}
	if(!player.stopped)
	{
		/* timing */
		if((getCurrentTime() - player.lastMoveTime) >= player.moveSpeed)
		{
			player.lastMoveTime = (int)getCurrentTime();
			/* do movement */
			if(keyboard.up == 1)
			{
				plyrMvUp();
			}
			if(keyboard.down == 1)
			{
				plyrMvDown();
			}
			if(keyboard.left == 1)
			{
				plyrMvLeft();
			}
			if(keyboard.right == 1)
			{
				plyrMvRight();
			}
		}
		if(keyboard.Sup == 1)
		{
			shoot(getCurrentWeapon(player.currentWeapon), 0, -1, player.X, player.Y);
			player.lastDirectionX = 0;
			player.lastDirectionY = -1;
		}
		else if(keyboard.Sdown == 1)
		{
			shoot(getCurrentWeapon(player.currentWeapon), 0, 1, player.X, player.Y);
			player.lastDirectionX = 0;
			player.lastDirectionY = 1;
		}
		else if(keyboard.Sleft == 1)
		{
			shoot(getCurrentWeapon(player.currentWeapon), -1, 0, player.X, player.Y);
			player.lastDirectionX = -1;
			player.lastDirectionY = 0;
		}
		else if(keyboard.Sright == 1)
		{
			shoot(getCurrentWeapon(player.currentWeapon), 1, 0, player.X, player.Y);
			player.lastDirectionX = 1;
			player.lastDirectionY = 0;
		}
		if(keyboard.Gernade == 1)
		{
			shoot(&weaponGernade, player.lastDirectionX, player.lastDirectionY, player.X, player.Y);
		}


		/* see if the player is on a health pack */
		if(getLevelPosition(HEALTH_LAYER, player.X, player.Y)->type == POSITION_HEALTH)
		{
			/* health pack here, do stuff */
			player.hp += 25;

			/* cap the hp at 100 */
			if(player.hp > 100)
			{
				player.hp = 100;
			}

			/* remove the health pack from this spot */
			setLevelPosition(HEALTH_LAYER, player.X, player.Y, whiteSpacePosition);

			/* set the text prompt */
			setTextPrompt("Woot - that\'s a good health pack!");
		}

		/* see if the player is on a weapon */
		if(!isLevelPositionSpace(PICKUP_LAYER, player.X, player.Y))
		{
			char spaceChar = getLevelPosition(PICKUP_LAYER, player.X, player.Y)->character;
			/* switch based on what weapon it is */
			switch(spaceChar)
			{
			case 'W':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_DUALPISTOL] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("Yay, you got the dual pistols!");
				break;
			case ']':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_SPREADGUN] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("Nice - you found the spread gun!");
				break;
			case ')':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_SCATTERGUN] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("Good job, you got the scattergun!");
				break;
			case '[':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_SUBMACHINEGUN] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("Nice - you found the sub machine gun!");
				break;
			case '(':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_HEAVYMACHINEGUN] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("Woot - you found the heavy machine gun!");
				break;
			case '"':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_RIFLE] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("Yay, you found the rifle!");
				break;
			case '\'':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_SNIPER] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("Good job, you picked up the sniper!");
				break;
			case '^':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_ROCKETLAUNCHER] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("Awesome, you got the ROCKET LAUNCHER!");
				break;
			case '~':
				/* set the weapon enabled */
				player.weaponsEnabled[GUN_OMEGALASER] = 1;

				/* remove the weapon from this spot */
				setLevelPosition(PICKUP_LAYER, player.X, player.Y, whiteSpacePosition);

				/* set the text prompt */
				setTextPrompt("WOAH - its the omega laser!!!!");
				break;

				/* edit by paul: msg for when the player picks up the grenade: "Great, you are now ready to go find and defeat the boss!" */
			}
		}

		/* switches */
		/* see if the player is on a switch */
		if(getLevelPosition(SWITCH_LAYER, player.X, player.Y)->character == 's')
		{
			/* get the character used for the switch and turn it into the capital, this
			way we know which switch-walls to remove(they're stored like this) */
			char switchCapital = 'S';

			/* replace all of these letters on the wall layer with spaces to clear the wall */
			clearLevelPositions(WALL_LAYER, switchCapital);

			/* set the text prompt */
			setTextPrompt("You opened a door!");
		}
		if(getLevelPosition(SWITCH_LAYER, player.X, player.Y)->character == 'u')
		{
			/* get the character used for the switch and turn it into the capital, this
			way we know which switch-walls to remove(they're stored like this) */
			char switchCapital = 'U';

			/* replace all of these letters on the wall layer with spaces to clear the wall */
			clearLevelPositions(WALL_LAYER, switchCapital);

			/* set the text prompt */
			setTextPrompt("Be careful, the next room has a spawn\ngenerator. It will continue producing\nrobots until you destroy it.");
		}
		if(getLevelPosition(SWITCH_LAYER, player.X, player.Y)->character == 'v')
		{
			/* get the character used for the switch and turn it into the capital, this
			way we know which switch-walls to remove(they're stored like this) */
			char switchCapital = 'V';

			/* replace all of these letters on the wall layer with spaces to clear the wall */
			clearLevelPositions(WALL_LAYER, switchCapital);

			/* set the text prompt */
			setTextPrompt("You opened a door!");
		}
		if(getLevelPosition(SWITCH_LAYER, player.X, player.Y)->character == 'r')
		{
			/* get the character used for the switch and turn it into the capital, this
			way we know which switch-walls to remove(they're stored like this) */
			char switchCapital = 'R';

			/* replace all of these letters on the wall layer with spaces to clear the wall */
			clearLevelPositions(WALL_LAYER, switchCapital);

			/* set the text prompt */
			setTextPrompt("You opened a door!");
		}
		/* see if the player is on an invisible switch */
		else if(getLevelPosition(SWITCH_LAYER, player.X, player.Y)->character == 'i')
		{
			/* get the character used for the switch and turn it into the capital, this
			way we know which switch-walls to remove(they're stored like this) */
			char switchCapital = 'I';

			/* switch the layers */
			switchCharacterLayers(SWITCH_LAYER, WALL_LAYER, switchCapital);
		}
		/* see if the player is on an invisible switch */
		else if(getLevelPosition(SWITCH_LAYER, player.X, player.Y)->character == 'g')
		{
			/* get the character used for the switch and turn it into the capital, this
			way we know which switch-walls to remove(they're stored like this) */
			char switchCapital = 'G';

			/* switch the layers */
			switchCharacterLayers(SWITCH_LAYER, WALL_LAYER, switchCapital);
		}
	}

	/* assert the players position */
	setLevelPosition(PLAYER_LAYER, player.X, player.Y, playerPosition);
}

/* cycles to the next available weapon */
void nextWeapon(int switcher)
{
	/* see if its been a while since we switched */
	if((getCurrentTime() - player.lastWeaponSwitchTime) >= player.weaponSwitchTime)
  {
		/* start the next weapon at currentWeapon + 1 */
		WEAPON_ID nextWeapon = player.currentWeapon + switcher;

		/* make sure the next weapon is in range */
		if(nextWeapon > (WEAPON_COUNT - 1))
		{
			/*if not, send it back to the begining but keep the same weapon chouce.*
			*so if the player has the upgrades selecte, it will cycle through only*
			*the upgrades.                                                        */
			if ((nextWeapon % 2) == 0)
				nextWeapon = 0;
			else 
				nextWeapon = 1;
		}

		if(nextWeapon < 0)
		{
			if ((nextWeapon % 2) == 0)
				nextWeapon = 8;
			else 
				nextWeapon = 9;
			while(player.weaponsEnabled[nextWeapon] == 0)
			{
				nextWeapon--;
			}
		}

		if (player.weaponsEnabled[nextWeapon] == 0)
    {
      nextWeapon++;
      if(nextWeapon > (WEAPON_COUNT - 1))
		  {
			  /*if not, send it back to the begining but keep the same weapon chouce.*
			  *so if the player has the upgrades selecte, it will cycle through only*
			  *the upgrades.                                                        */
			  if ((nextWeapon % 2) == 0)
				  nextWeapon = 0;
			  else 
				  nextWeapon = 1;
		  }

      while  (player.weaponsEnabled[nextWeapon] == 0)
      {
        nextWeapon++;
        if(nextWeapon > (WEAPON_COUNT - 1))
		    {
			    /*if not, send it back to the begining but keep the same weapon chouce.*
			    *so if the player has the upgrades selecte, it will cycle through only*
			    *the upgrades.                                                        */
			    if ((nextWeapon % 2) == 0)
				    nextWeapon = 0;
			    else 
				    nextWeapon = 1;
		    }
      }
		}

		/* set the current weapon to the next weapon */
		player.currentWeapon = nextWeapon;

		/* set the last switch time to be now */
		player.lastWeaponSwitchTime = (int)getCurrentTime();
	}
}

/*! \brief Move the player left

Checks if the space is open to be moved into, if it is, it moves to that
space

\sa plyrMvLeft()
*/

void plyrMvLeft()
{
	/* wall check */
	if(isLevelPositionSpace(WALL_LAYER,player.X-1,player.Y) && isLevelPositionSpace(ENEMY_LAYER,player.X-1,player.Y))
	{
		/* deletes the player from its old space */
		setLevelPosition(PLAYER_LAYER, player.X, player.Y, whiteSpacePosition);
		player.X -= 1;
		setLevelPosition(PLAYER_LAYER, player.X, player.Y, playerPosition);	
	}
}

/*! \brief Move the player right

Checks if the space is open to be moved into, if it is, it moves to that
space

\sa plyrMvRight()
*/
void plyrMvRight()
{
	/* wall check */
	if(isLevelPositionSpace(WALL_LAYER,player.X+1,player.Y) && isLevelPositionSpace(ENEMY_LAYER,player.X+1,player.Y))
	{
		setLevelPosition(PLAYER_LAYER, player.X, player.Y, whiteSpacePosition);
		player.X += 1;
		setLevelPosition(PLAYER_LAYER, player.X, player.Y, playerPosition);

	}
}

/*! \brief Move the player up

Checks if the space is open to be moved into, if it is, it moves to that
space

\sa plyrMvUp()
*/
void plyrMvUp()
{
	/* wall check */
	if(isLevelPositionSpace(WALL_LAYER,player.X,player.Y-1) && isLevelPositionSpace(ENEMY_LAYER,player.X,player.Y-1))
	{
		setLevelPosition(PLAYER_LAYER, player.X, player.Y, whiteSpacePosition);
		player.Y -= 1;
		setLevelPosition(PLAYER_LAYER, player.X, player.Y, playerPosition);

	}
}

/*! \brief Move the player down

Checks if the space is open to be moved into, if it is, it moves to that
space

\sa plyrMvDown()
*/
void plyrMvDown()
{
	/* wall check */
	if(isLevelPositionSpace(WALL_LAYER,player.X,player.Y+1) && isLevelPositionSpace(ENEMY_LAYER,player.X,player.Y+1))
	{
		setLevelPosition(PLAYER_LAYER, player.X, player.Y, whiteSpacePosition);
		player.Y += 1;
		setLevelPosition(PLAYER_LAYER, player.X, player.Y, playerPosition);

	}
}

int getPlayerX()
{
	return player.X;
}
int getPlayerY()
{
	return player.Y;
}
void setPlayerX(int x)
{
	player.X = x;
}
void setPlayerY(int y)
{
	player.Y = y;
}

/*! \brief Load the player data from a file.

Loads saved game data for a player from a file.

\param fileName - the file name (relative to the exe) of the saved game 
to be loaded.
*/
void loadPlayerData(char *fileName, struct S_PLAYER* sPlayer)
{
	/* first lets load the file up */
	FILE *fileIn;

	/* create a buffer to hold a maximum line length of 500 characters */
	char lineBuffer[501] = {0};

	/* create a buffer to hold each variable name */
	char varName[101] = {0};

	/* create a buffer to hold the value of each variable */
	char varValue[51] = {0};

	/* this is the delimeter used to split the variable name and value */
	char delimiter[] = "=\n";

	/* used to tokenize the string */
	char *varResult = NULL;

	/* used when tokenizing to make sure we stick to a var name and value */
	int tokenCounter;

	/* open the save file */
	fileIn = fopen(fileName, "rt");

	/* if we were able to open the file */
	if(fileIn != NULL)
	{
		/* read in every line of the file */
		while(fgets(&lineBuffer[0], 501, fileIn) != NULL)
		{
			/* determine if this line is a comment or a blank line and if so, skip */
			if(lineBuffer[0] != '\n' && lineBuffer[0] != '\\' && lineBuffer[0] != ' ')
			{
				/* start the var result at NULL */
				varResult = NULL;

				/* start the token counter at 0 */
				tokenCounter = 0;

				/* use strtok to get the first part of the string
				split by the token */
				varResult = strtok(&lineBuffer[0], delimiter);

				/* loop through as long as we havn't gotten both tokens */
				while(varResult != NULL && tokenCounter < 2)
				{
					/* if this is the first token, set the var name */
					if(tokenCounter == 0)
					{
						strcpy(varName, varResult);
					}
					/* if this is the second token, set the var value */
					else if(tokenCounter == 1)
					{
						strcpy(varValue, varResult);
					}
					/* increase the token counter */
					tokenCounter++;

					/* read in the next token */
					varResult = strtok(NULL, delimiter);
				}

				/* call the load var function to process this variable */
				loadPlayerVar(varName, varValue, sPlayer);
			}
		}

		player.lastDirectionX = 1;
		player.lastDirectionY = 0;
	}
	else
	{
		printf("Unabled to load sPlayer data from file %s",fileName);
	}
}

/*! \brief Save the player data to a file.

Saves the current player data to a save game file.

\param fileName - the file name (relative to the exe) of the file to
save the game to.
*/
void savePlayerData(char *fileName, struct S_PLAYER* sPlayer)
{
	/* first we need to open up the specified file */
	FILE *fileOut = fopen(fileName, "wt");

	/* make sure we were able to open the file */
	if(fileOut != NULL)
	{
		/* now we need to write all the data that needs to be saved.
		Data should be saved as variableName=variableValue */
		/* add any sPlayer data that needs to be saved to this area
		using the convenient savePlayerVar function, which will
		format things for us and then write it to the file */

		/* label this section to make the save file easy to read */
		fputs("\\\\ player name data\n", fileOut);
		/* save the sPlayer name data */
		savePlayerVarString("playerName", &sPlayer->name[0], fileOut);

		/* label this section to make the save file easy to read */
		fputs("\n\\\\ level data\n", fileOut);
		/* save the sPlayer level completion data */
		savePlayerVarInt("levelOneComplete", sPlayer->levelOneComplete, fileOut);
		savePlayerVarInt("levelTwoComplete", sPlayer->levelTwoComplete, fileOut);
		savePlayerVarInt("levelThreeComplete", sPlayer->levelThreeComplete, fileOut);
		savePlayerVarInt("levelFourComplete", sPlayer->levelFourComplete, fileOut);
		savePlayerVarInt("levelFiveComplete", sPlayer->levelFiveComplete, fileOut);

		/* label this section to make the save file easy to read */
		fputs("\n\\\\ weapon data\n", fileOut);
		/* save the sPlayer weapons pickup data */
		savePlayerVarInt("singlePistolEnabled", sPlayer->weaponsEnabled[GUN_PISTOL], fileOut);
		savePlayerVarInt("doublePistolEnabled", sPlayer->weaponsEnabled[GUN_DUALPISTOL], fileOut);
		savePlayerVarInt("spreadGunEnabled", sPlayer->weaponsEnabled[GUN_SPREADGUN], fileOut);
		savePlayerVarInt("scatterGunEnabled", sPlayer->weaponsEnabled[GUN_SCATTERGUN], fileOut);
		savePlayerVarInt("subMachineGunEnabled", sPlayer->weaponsEnabled[GUN_SUBMACHINEGUN], fileOut);
		savePlayerVarInt("heavyMachineGunEnabled", sPlayer->weaponsEnabled[GUN_HEAVYMACHINEGUN], fileOut);
		savePlayerVarInt("rifleEnabled", sPlayer->weaponsEnabled[GUN_RIFLE], fileOut);
		savePlayerVarInt("sniperEnabled", sPlayer->weaponsEnabled[GUN_SNIPER], fileOut);
		savePlayerVarInt("rocketLauncherEnabled", sPlayer->weaponsEnabled[GUN_ROCKETLAUNCHER], fileOut);
		savePlayerVarInt("omegaLaserEnabled", sPlayer->weaponsEnabled[GUN_OMEGALASER], fileOut);

		/* label this section to make the save file easy to read */
		fputs("\n\\\\ boss waypoint data\n", fileOut);
		/* save the boss waypoints */
		savePlayerVarInt("levelOneBoss", sPlayer->atLevelOneBoss, fileOut);
		savePlayerVarInt("levelTwoBoss", sPlayer->atLevelTwoBoss, fileOut);
		savePlayerVarInt("levelThreeBoss", sPlayer->atLevelThreeBoss, fileOut);
		savePlayerVarInt("levelFourBoss", sPlayer->atLevelFourBoss, fileOut);
		savePlayerVarInt("levelFiveBoss", sPlayer->atLevelFiveBoss, fileOut);

		/* label this section to make the save file easy to read */
		fputs("\n\\\\ save the sPlayers weapon\n", fileOut);
		/* save the current weapon */
		savePlayerVarInt("currentWeapon", sPlayer->currentWeapon, fileOut);

		/* flush the stream now that we're done, to make sure things get
		written to the file. */
		fflush(fileOut);
	}
	else
	{
		printf("Unable to open save file!\n"); 
	}
}

/*! \brief Load a variable name into the player data.

This function does the job of parsing a variable name and value in
string form and storing the correct values into the player data.
For the most part, this is only a helper function for loadPlayerData()

\param varName - a string containing the variable name to be loaded.
\param varValue - a string containing the variable value to be loaded.

\sa loadPlayerData(), savePlayerData()
*/
void loadPlayerVar(char *varName, char *varValue, struct S_PLAYER* sPlayer)
{
	/* convert the value to an int */
	int value = atoi(varValue);

	/* switch between the sPlayer variable names */
	if(strcmp(varName,"levelOneComplete") == 0)
	{
		sPlayer->levelOneComplete = value;
	}
	else if(strcmp(varName,"levelTwoComplete") == 0)
	{
		sPlayer->levelTwoComplete = value;
	}
	else if(strcmp(varName,"levelThreeComplete") == 0)
	{
		sPlayer->levelThreeComplete = value;
	}
	else if(strcmp(varName,"levelFourComplete") == 0)
	{
		sPlayer->levelFourComplete = value;
	}
	else if(strcmp(varName,"levelFiveComplete") == 0)
	{
		sPlayer->levelFiveComplete = value;
	}
	else if(strcmp(varName,"singlePistolEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_PISTOL] = value;
	}
	else if(strcmp(varName,"doublePistolEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_DUALPISTOL] = value;
	}
	else if(strcmp(varName,"spreadGunEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_SPREADGUN] = value;
	}
	else if(strcmp(varName,"scatterGunEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_SCATTERGUN] = value;
	}
	else if(strcmp(varName,"subMachineGunEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_SUBMACHINEGUN] = value;
	}
	else if(strcmp(varName,"heavyMachineGunEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_HEAVYMACHINEGUN] = value;
	}
	else if(strcmp(varName,"rifleEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_RIFLE] = value;
	}
	else if(strcmp(varName,"sniperEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_SNIPER] = value;
	}
	else if(strcmp(varName,"rocketLauncherEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_ROCKETLAUNCHER] = value;
	}
	else if(strcmp(varName,"omegaLaserEnabled") == 0)
	{
		sPlayer->weaponsEnabled[GUN_OMEGALASER] = value;
	}
	else if(strcmp(varName,"currentWeapon") == 0)
	{
		sPlayer->currentWeapon = value;
	}
	else if(strcmp(varName,"levelOneBoss") == 0)
	{
		sPlayer->atLevelOneBoss = value;
	}
	else if(strcmp(varName,"levelTwoBoss") == 0)
	{
		sPlayer->atLevelTwoBoss = value;
	}
	else if(strcmp(varName,"levelThreeBoss") == 0)
	{
		sPlayer->atLevelThreeBoss = value;
	}
	else if(strcmp(varName,"levelFourBoss") == 0)
	{
		sPlayer->atLevelFourBoss = value;
	}
	else if(strcmp(varName,"levelFiveBoss") == 0)
	{
		sPlayer->atLevelFiveBoss = value;
	}
	else if(strcmp(varName,"playerName") == 0)
	{
		/* the player name is a string, so we want the raw value */
		strcpy(sPlayer->name, varValue);
	}
}

/*! \brief Save a variable name with a value into a save file.

This function formats a string correctly and saves it to a player
data save file given the variable name and variable value passed
to it.

\param varName - a string containing the variable name to be stored.
\param varValue - a string containing the variable value to be stored.

\sa loadPlayerData(), savePlayerData()
*/
void savePlayerVarInt(char *varName, int varValue, FILE *fileOut)
{
	/* store the value converted to a string */
	char value[100] = {0};

	/* make sure the file isn't null */
	if(fileOut != NULL)
	{
		/* convert the var value to a string */
		myItoa(varValue, value);

		/* now that we have a string, save it with the string function */
		savePlayerVarString(varName, &value[0], fileOut);
	}

}
/*! \brief Save a variable name with a value into a save file.

This function formats a string correctly and saves it to a player
data save file given the variable name and variable value passed
to it.

\param varName - a string containing the variable name to be stored.
\param varValue - a string containing the variable value to be stored.

\sa loadPlayerData(), savePlayerData()
*/
void savePlayerVarString(char *varName, char *varValue, FILE *fileOut)
{
	/* buffer used to create the string we are writing to the file */
	char writeBuffer[100] = {0};

	char *seperator = "=";

	char *ending = "\n";

	/* make sure the file isn't null */
	if(fileOut != NULL)
	{
		/* create the string we'll be writing to the file */
		/* make the string start with the variable name */
		strcpy(&writeBuffer[0], varName);

		/* add an '=' to it */
		strcat(&writeBuffer[0], seperator);

		/* add the variable value to it */
		strcat(&writeBuffer[0], varValue);

		/* add a newline to the end */
		strcat(&writeBuffer[0], ending);

		/* write the newly created string to the file */
		fputs(&writeBuffer[0], fileOut);
	}
}

void myItoa(int value, char *valueString)
{
	if(value == 1)
	{
		valueString[0] = '1';
	}
	else
	{
		valueString[0] = '0';
	}
}
