/*! ************************************************************************
\file driver.c
\author Sheridan Beebe, Chris Howard
\par email: sbeebe@digipen.edu, choward@digipen.edu
\par Game: Toasted
\date January 16th, 2007
\brief

This file is used as the main program file for Toasted. It
contains the main game loop which executes all the code for
each of the game's loops. It sets up the console window as well.

***************************************************************************/
/*!
\mainpage Toasted Level Manager

\section intro Introduction

This is the documentation for the Toasted level management system and such.
Check out the different sections in order to find out info on the methods.
Everything should be commented correctly, if you find any discrepancies, let
me know. Have fun.

\section links Useful Links

The most important page in this documentation is the documentation on the level
management header. Here is a <A HREF="_level_manager_8h.html">link</A>.

\section readme A Short Readme

To use the level management system, you need to include the file LevelManager.h.
Doing so will give you access to all of its functions, types, and defines. Before
the level management system can be used, the function loadLevelManager() should
be called. Usualy this will be called by the game when it first starts, so you
shouldn't have to worry about it. The function unloadLevelManager() should also
be called just before the program exits.

To set the position of something in the level manager, you first need to create
a new LEVEL_POSITION. You can do this using createLevelPosition(). Chances are
how ever, that if you are working with a subsystem, the level loading system
will pass you a pointer to a LEVEL_POSITION when it informs you that you are to
create something new. You should use this LEVEL_POSITION instead of creating a new one.
Once you have a LEVEL_POSITION, you can place it somewhere on the map using the function
setLevelPosition(). On the opposite side, you can get the LEVEL_POSITION at a
specific position using the function getLevelPosition(). If you need to change an
old position into a white space(ie, an empty space) you should set the level position
to the already available LEVEL_POSITION created for all white space. This position is
named whiteSpacePosition. It is a global so it should be accessible after including the
LevelManager.h header.

If you are working with another part of the game, and for what ever reason, you need
to load a LEVEL you should use the function loadLevel(). This function will load the
LEVEL with the specified file name(relative to the application exe) and set it as 
the current LEVEL in use. Before loading a new LEVEL, the old LEVEL should be unloaded
using the function, unloadLevel().

For any other information, see the documentation for the LevelManager.h header.
*/

/* include some things to help us print and such to the console */
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "LevelManager.h"
#include "GraphicsSystem.h"
#include "EnemyManager.h"
#include "timer.h"
#include "WeaponManager.h"
#include "turret.h"
#include "player.h"
#include "mauler.h"
#include "effects.h"
#include <time.h>
#include "MenuSystem.h"
#include "LevelOne.h"
#include "LevelTwo.h"
#include "LevelFour.h"
#include "LevelFive.h"
#include "LevelThree.h"
#include "LoadSaves.h"
#include "turret.h"
#include "Overworld.h"
#include "hud.h"
#include "Sound.h"

int lvl_1(HANDLE hOut, int state);
int lvl_2(HANDLE hOut, int state);
int lvl_3(HANDLE hOut, int state);
int lvl_4(HANDLE hOut, int state);
int lvl_5(HANDLE hOut, int state);
int game_over(HANDLE hOut, int state);
int story_screen(HANDLE hOut, int state);
int victory_screen(HANDLE hOut, int state);
int Overworld(HANDLE hOut, int state);
int enter_game(HANDLE hOut, int state);
int credits(HANDLE hOut, int state);
int start_game(HANDLE hOut, int state);
int Load_game(HANDLE hOut, int state);
int New_game(HANDLE hOut, int state);

/* main method */
int main(void)
{
	/* store the size of the buffer to be used for the console window */
	COORD bufferSize = {CONSOLE_WIDTH, CONSOLE_HEIGHT};

	/* store the size of the window to be created */
	SMALL_RECT windowSize = {0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1};

	/* handle used to print to the console window */
	HANDLE hOut;

	/* cursor info used to manipulate the console's cursor */
	CONSOLE_CURSOR_INFO ConCurInf;

	/* paul: pause menu */
	char* controlText = loadFileToString("maps/pauseMenu.txt");

	/* variable  used to loop forever. Set to 0 if the program
	should exit. */
	STATE game_state = STATE_COPYRIGHT_MENU;

	/* reset pause text */
	setPauseText(controlText);

	/* turn on the sounds ! */
	soundsEnabled = 1;

	/*Mike:Call the function that initializes the cursor's attributes*/
	InitializeCursor();

	srand(clock());

	/*Mike: Initialize the name indexer*/
	saveIndex = 0;
	/*Mike: Initialize the hud flashing bools*/
	HUDInitializer();

	/* init the level manager, MUST be done */
	loadLevelManager();

	expl_init();

	turret_init();

	/* load the enemy manager */
	loadEnemies();

	/* load the weapons system */	
	loadWeapons();

	/* load the sound system */	
	loadSoundSystem();

	/* hide the cursor */
	ConCurInf.dwSize = 100;
	ConCurInf.bVisible = FALSE;

	/* get the output handle for the console window */
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	/* set the window title */
	SetConsoleTitle("Toasted");

	/* set the cursor info and hide it */
	SetConsoleCursorInfo(hOut,&ConCurInf);

	/* set the window size */
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	/* set the console window's buffer size */
	SetConsoleScreenBufferSize(hOut, bufferSize);

	/* set up the text prompt */
	createTextPrompt();

	/* set up initial player vars */
	player.currentWeapon = GUN_PISTOL;
	player.lastMoveTime = 0;
	player.moveSpeed = 100;
	player.lastWeaponSwitchTime = 0;
	player.weaponSwitchTime = 100;

	/* start up the timers */
	loadTimers();

	while(game_state != STATE_END_GAME)
	{
		player.hp = 100;
		player.stopped =  0;
		switch(game_state)
		{
		case STATE_END_GAME:
			/* exit the game */
			return 0;
			break;
		case STATE_MAIN_MENU:
			/*Mike: show the main menu */
			game_state = start_game(hOut, game_state);
			break;
    case STATE_COPYRIGHT_MENU:
      /*Mike: show the main menu */
			game_state = enter_game(hOut, game_state);
			break;
		case STATE_LOAD_MENU:
			/*Mike: show the new game menu*/
			game_state = Load_game(hOut, game_state);
			break;
		case STATE_NEW_MENU:
			/*Mike: show the new game menu*/
			game_state = New_game(hOut, game_state);
			break;
		case STATE_GAME_OVER:
			/* show the game over screen */
			game_state = game_over(hOut, game_state);
			break;
		case STATE_STORY:
			/* show the game over screen */
			game_state = story_screen(hOut, game_state);
			break;
    case STATE_CREDITS_MENU:
      game_state = credits(hOut, game_state);
      break;
		case STATE_VICTORY:
			game_state = victory_screen(hOut, game_state);
			break;
		case STATE_OVER_WORLD:
			/* to the overworld! */
			game_state = Overworld(hOut, game_state);
			break;
		case STATE_LEVEL_ONE:
			/* show level 1 */
			game_state = lvl_1(hOut, game_state);
			break;
		case STATE_LEVEL_TWO:
			/* show level 2 */
			game_state = lvl_2(hOut, game_state);
			break;
		case STATE_LEVEL_THREE:
			/* show level 3 */
			game_state = lvl_3(hOut, game_state);
			break;
			/* show level 4 */
		case STATE_LEVEL_FOUR:
			game_state = lvl_4(hOut, game_state);
			break;
			/* show level 5 */
		case STATE_LEVEL_FIVE:
			game_state = lvl_5(hOut, game_state);
			break;
		default:
			break;
		}
	}

	/* free up stuff */
	unloadSoundSystem();
	/*free(controlText);*/

	/* exit the program */
	return 0;
}

int game_over(HANDLE hOut, int state)
{
	int game_state = state;
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];
	loadchars("maps/gameover.tlf","maps/gameoverC.tlf", cha);

	/* stop the game music */
	stopSound(&soundGame);

	/* play the game over music */
	playSound(&soundGameOver);

	while(game_state == state)
	{
		game_state = processKeys(hOut, state);
		paint(hOut, cha);
	}

	return game_state;
}
int story_screen(HANDLE hOut, int state)
{
	int game_state = state;
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];
	loadchars("maps/Story.tlf","maps/StoryC.tlf", cha);

	/* stop the game sound */
	stopSound(&soundGame);

	/* play the menu sound */
	playSound(&soundMenu);

	paint(hOut, cha);

	while(game_state == state)
	{
		game_state = processKeys(hOut, state);
	}

	return game_state;
}
/**************************************************
////Mike/////
function    victory_screen

input hOut - handle used to print to the console window
      state - enum to the state of the game

return - an int (enum) for the game state
***************************************************/
int victory_screen(HANDLE hOut, int state)
{
	int game_state = state;
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];
	loadchars("maps/Victory.tlf","maps/VictoryC.tlf", cha);

	/* stop the game sound */
	stopSound(&soundGame);

	/* play the menu sound */
	playSound(&soundMenu);

	paint(hOut, cha);

	while(game_state == state)
	{
		game_state = processKeys(hOut, state);
	}

	return game_state;
}

/**************************************************
////Mike/////
function    enter_game
input hOut - handle used to print to the console window
      state - enum to the state of the game
return - an int (enum) for the game state
***************************************************/
int enter_game(HANDLE hOut, int state)
{
	int game_state = state;
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];
  double timer = getCurrentTime();

	loadchars("maps/Copyright.tlf","maps/CopyrightC.tlf", cha);

	/* stop the game sound */
	/*stopSound(&soundGame); */

	/* play the menu sound */
	/* playSound(&soundMenu); */

	while(game_state == state)
	{
		updateSoundSystem();
    updateTimers();
		game_state = processKeys(hOut, state);
		paint(hOut, cha);
    if ((getCurrentTime()) > (timer + 5000))
    {
      game_state = STATE_MAIN_MENU;
    }
	}     
	return game_state;
}

/**************************************************
////Mike/////
function    credits
input hOut - handle used to print to the console window
      state - enum to the state of the game
return - an int (enum) for the game state
***************************************************/
int credits(HANDLE hOut, int state)
{
	int game_state = state;
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];
  double timer = getCurrentTime();

	loadchars("maps/Credits.tlf","maps/CreditsC.tlf", cha);

	while(game_state == state)
	{
		updateSoundSystem();
    updateTimers();
		paint(hOut, cha);
    if ((getCurrentTime()) > (timer + 8000))
    {
      game_state = STATE_MAIN_MENU;
    }
	}     
	return game_state;
}

/**************************************************
////Mike/////
function    start_game
input hOut - handle used to print to the console window
      state - enum to the state of the game
return - an int (enum) for the game state
***************************************************/
int start_game(HANDLE hOut, int state)
{
	int game_state = state;
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];

	loadchars("maps/begin_game.TLF","maps/begin_gameC.TLF", cha);

	/* stop the game sound */
	stopSound(&soundGame);

	/* play the menu sound */
	playSound(&soundMenu);

	while(game_state == state)
	{
		updateSoundSystem();
		game_state = processKeys(hOut, state);
		/*Mike:call the function that will move the cursor*/
		SetCursorInBuff(cha);
		paint(hOut, cha);
	}     
	return game_state;
}


/**************************************************
////Mike/////
function    Load_game
This function handles all of the load
input       hOut - windows handle
state - enum to the state of the game

return      an int (enum) for the game state
***************************************************/
int Load_game(HANDLE hOut, int state)
{
	int game_state = state;
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];

	/*Mike: get the save files in the saves folder    */
	FindAllSaves();

	saved_files.selected_save = 0;

	loadchars("maps/LoadGame.tlf","maps/LoadGameC.tlf", cha);

	/* stop the game sound */
	stopSound(&soundGame);

	/* play the menu sound */
	playSound(&soundMenu);

  /*as long as the game is in the same state*/
	while(game_state == state)
	{
		int i, beat_levels = 0;
		unsigned int j;
		/* char empty[] = "Empty!"; */

		updateSoundSystem();

		game_state = processKeys(hOut, state);

    /*double for loops for writing to the screen buffer*/
		for (i = 0; i < MAX_SAVES; i++)
		{
      beat_levels = saved_files.plyrarray[i].levelOneComplete +
                    saved_files.plyrarray[i].levelTwoComplete +
                    saved_files.plyrarray[i].levelThreeComplete +
                    saved_files.plyrarray[i].levelFourComplete +
                    saved_files.plyrarray[i].levelFiveComplete;

      if (beat_levels == 5)
      {
        cha[4 + (10 * i)][61].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][63].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][65].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][67].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][69].Char.AsciiChar = (CHAR)251;
      }
      else if (beat_levels == 4)
      {
        cha[4 + (10 * i)][61].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][63].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][65].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][67].Char.AsciiChar = (CHAR)251;
      }
      else if (beat_levels == 3)
      {
        cha[4 + (10 * i)][61].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][63].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][65].Char.AsciiChar = (CHAR)251;
      }
      else if (beat_levels == 2)
      {
        cha[4 + (10 * i)][61].Char.AsciiChar = (CHAR)251;
        cha[4 + (10 * i)][63].Char.AsciiChar = (CHAR)251;
      }
      else if (beat_levels == 1)
      {
        cha[4 + (10 * i)][61].Char.AsciiChar = (CHAR)251;
      }

			for (j = 0; j < (strlen(saved_files.plyrarray[i].name)) ; j++)
			{
        cha[4 + (10 * i)][4 + j].Char.AsciiChar = saved_files.plyrarray[i].name[j];

				if (saved_files.selected_save == i)
				{
					cha[4 + (10 * i)][4 + j].Attributes = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				} 
				else
					cha[4 + (10 * i)][4 + j].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			}
		}

		paint(hOut, cha);
  }

	if (game_state == STATE_OVER_WORLD)
	{
		strcpy (&player.saveFile[0], saved_files.file_array[saved_files.selected_save]);
		loadPlayerData(&player.saveFile[0], &player);
	}

	return game_state;
}





/**************************************************
////Mike/////
function    New_game

input       hOut - windows handle
state - enum to the state of the game

return      an int (enum) for the game state
***************************************************/
int New_game(HANDLE hOut, int state)
{
	int game_state = state;
	int i;
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];

	for (i = 0; i < MAX_SAVE_LENGTH ; i++)
	{
		saveFileName[i] = 0;
	}

	saveIndex = 0;

	loadchars("maps/NewGame.TLF","maps/NewGameC.TLF", cha);
	while(game_state == state)
	{
		/* update the sound system */
		updateSoundSystem();

		game_state = processKeys(hOut, state);

		for (i = 0; i <= saveIndex; i++)
		{
			cha[10][30 + i].Char.AsciiChar = saveFileName[i];
			cha[10][30 + i].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
		paint(hOut, cha);
	}


  /*As long as the player is not going back to the main menu*/
	if (game_state != STATE_MAIN_MENU)
	{
    /*copy the selected file name into the player struct*/
		strcpy (&player.saveFile[0], saved_files.file_array[saved_files.selected_save]);

    /*Copy the data from the new game file into the player struct*/
		loadPlayerData("saves/New.txt", &player);

		/* set the player name to the one entered */
		strcpy(&player.name[0], &saveFileName[0]);

		savePlayerData(&player.saveFile[0], &player);
	}

	return game_state;
}

int Overworld(HANDLE hOut, int state)
{
	COORD viewCoord;
	int game_state = state;

	/* reset pause */
	timerPaused = 0;

	/* stop the game over sound */
	stopSound(&soundGameOver);

	/* stop the menu sound */
	stopSound(&soundMenu);

	/* play the game sound */
	playSound(&soundGame);

	/* Load a new level - sets it as the current level */
	loadLevel("maps/Overworld.tlf", "maps/OverworldU.tlf", 0);
	/* not sure - Ask Chris */

	/* reset player movement */
	keyboard.up = 0;
	keyboard.down = 0;
	keyboard.right = 0;
	keyboard.left = 0;
	keyboard.Sup = 0;
	keyboard.Sdown = 0;
	keyboard.Sright = 0;
	keyboard.Sleft = 0;

	viewCoord.X = 1;
	viewCoord.Y = 0;

	/* do start up - remove walls, etc */
	startupOverworld();

	/* set the text prompt depending on what levels have been completed*/
	if (player.levelFiveComplete)
		setTextPrompt("Great work defeating the robots!!\nFeel free to roam around and clean up the\nremaining enemy forces.");
	else if (player.levelFourComplete)
		setTextPrompt("Nice work in the Commissary. Why dont you\ntry heading to the Hanger to the north?");
	else if (player.levelThreeComplete)
		setTextPrompt("Took care of that phase bot in the\nHeadquarters I see. Why not head to the\nCommissary in the east?");
	else if (player.levelTwoComplete)
		setTextPrompt("Now that the Armory is taken care of, I\nthink you should clear out the\nHeadquarters. It is located in the south\neast.");
	else if (player.levelOneComplete)
		setTextPrompt("Awesome, you cleared out the Barracks\nand got a new weapon. Try checking the\nArmory for more powerful weapons. It is\nto the south west.");
	else
		setTextPrompt("Welcome to the battleground. The robots,\nhave overrun everything, so you should\ngo take them out.");

	/* call explosion clear */
	expl_destroy();

	/* main game loop - loop until game_on is false(0) */
	while(game_state == state)
	{
		/* update the timers */
		updateTimers();

		/* update the sound system */
		updateSoundSystem();

		/* call the movement function to process user input. If the user
		hits escape, plyr_movement() will return false and the game will
		exit on the next loop. */
		game_state = processKeys(hOut, state);

		updateBulletList();
		expl_update();

		/* update the player */
		updatePlayer();

		/* call the method that moves and renders the viewport */
		translate(hOut, getCurrentLevel(), &viewCoord);
	}

	clearBullets();

	unloadLevel(getCurrentLevel());

	savePlayerData(&player.saveFile[0], &player);

	return game_state;
}

int lvl_1(HANDLE hOut, int state)
{
	COORD viewCoord;
	int game_state = state;

	/* Load a new level - sets it as the current level */
	loadLevel("maps/level1.tlf", "maps/level1U.tlf", 1);

	/* initialize the number of enemies per level here */
	totalClawbots = 14;

	spawnChecker = 1;
	levelOneBossChecker = 1;

	/* reset player movement */
	keyboard.up = 0;
	keyboard.down = 0;
	keyboard.right = 0;
	keyboard.left = 0;
	keyboard.Sup = 0;
	keyboard.Sdown = 0;
	keyboard.Sright = 0;
	keyboard.Sleft = 0;

	viewCoord.X = 0;
	viewCoord.Y = 0;
	/*Mike: write text for the level in the hud*/
	if (player.levelOneComplete)
		setTextPrompt("They haven\'t set much up in here. Watch\nout for the Clawbots and take out their\nspawn factories.");
	else
		setTextPrompt("This is the Toasted tutorial.\nPlease hit either of those switches on\nthe ground to open the door.");
	/* main game loop - loop until game_on is false(0) */
  turret_create(261, 106, 100, 5000, 4);
  turret_create(301, 97, 100, 5000, 5);
  turret_create(261, 87, 100, 5000, 4);
	while(game_state == state)
	{
		/* update the timers */
		updateTimers();
		/* update the sound system */
		updateSoundSystem();

		/* call the movement function to process user input. If the user
		hits escape, plyr_movement() will return false and the game will
		exit on the next loop. */
		game_state = processKeys(hOut, state);

		expl_update();
    turret_fire();

		/* update the player */
		updatePlayer();

		/* update the spawns */
		updateSpawnList();

		/* update the bullets */
		updateBulletList();

		/* update the enemies */
		updateEnemyList();

		/* used to update the tutorial */
		updateLevelOne();

		/* update the level one boss */
		updateLevelOneBoss();

		/* update the level one mini boss */
		updateLevelOneMiniBoss();

		/* call the method that moves and renders the viewport */
		translate(hOut, getCurrentLevel(), &viewCoord);
	}

  turret_delete_list();
	/* this function clears the enemies and spawn points after the player dies */
	clearEnemies();

	expl_destroy();

	/* clear out the bullets yo. */
	clearBullets();

	unloadLevel(getCurrentLevel());

	/* test saving player data */
	savePlayerData(&player.saveFile[0], &player);

	return game_state;
}

int lvl_2(HANDLE hOut, int state)
{
	COORD viewCoord;
	int game_state = state;

	/* Load a new level - sets it as the current level */
	loadLevel("maps/level2.tlf", "maps/level2U.tlf", 2);
	/* not sure - Ask Chris */

	/* initialize the number of enemies per level here */
	totalClawbots = 15;
	totalJumpers = 6;

	/* reset player movement */
	keyboard.up = 0;
	keyboard.down = 0;
	keyboard.right = 0;
	keyboard.left = 0;
	keyboard.Sup = 0;
	keyboard.Sdown = 0;
	keyboard.Sright = 0;
	keyboard.Sleft = 0;

	viewCoord.X = 0;
	viewCoord.Y = 0;

	/*Mike: write text for the level in the hud*/
	if (!player.levelTwoComplete)
		setTextPrompt("WOW, those Zig-Zag bots are fast and can\nleap over small walls!!");
    turret_create(126, 81, 50, 5000, 7);
	/* main game loop - loop until game_on is false(0) */
	while(game_state == state)
	{
		/* update the timers */
		updateTimers();

		/* update the sound system */
		updateSoundSystem();

		/* call the movement function to process user input. If the user
		hits escape, plyr_movement() will return false and the game will
		exit on the next loop. */
		game_state = processKeys(hOut, state);

		expl_update();
		turret_fire();

		/* update the player */
		updatePlayer();

		/* update the spawns */
		updateSpawnList();

		/* update the bullets */
		updateBulletList();

		/* update the enemies */
		updateEnemyList();

		/* update the level two mini boss */
		updateLevelTwoMiniBoss();

		/* update the level two boss */
		updateLevelTwoBoss();

		/* call the method that moves and renders the viewport */
		translate(hOut, getCurrentLevel(), &viewCoord);
	}
  turret_delete_list();
	/* this function clears the enemies and spawn points after the player dies */
	clearEnemies();

	expl_destroy();

	/* clear out the bullets yo. */
	clearBullets();

	unloadLevel(getCurrentLevel());

	/* test saving player data */
	savePlayerData(&player.saveFile[0], &player);

	return game_state;
}

int lvl_3(HANDLE hOut, int state)
{
	COORD viewCoord;
	int game_state = state;

	/* Load a new level - sets it as the current level */
	loadLevel("maps/level3.tlf", "maps/level3U.tlf", 3);
	/* not sure - Ask Chris */

	/* initialize the number of enemies per level here */
	totalClawbots = 10;
	totalJumpers = 4;
	totalShooters = 4;

	/* reset player movement */
	keyboard.up = 0;
	keyboard.down = 0;
	keyboard.right = 0;
	keyboard.left = 0;
	keyboard.Sup = 0;
	keyboard.Sdown = 0;
	keyboard.Sright = 0;
	keyboard.Sleft = 0;

	viewCoord.X = 0;
	viewCoord.Y = 0;

	/*Mike: write text for the level in the hud*/
	if (!player.levelThreeComplete)
		setTextPrompt("Looks like the enemy found our weapons in\nthe Armory. They now have Shooter bots.");

	/* main game loop - loop until game_on is false(0) */
	while(game_state == state)
	{
		/* update the timers */
		updateTimers();

		/* update the sound system */
		updateSoundSystem();

		/* call the movement function to process user input. If the user
		hits escape, plyr_movement() will return false and the game will
		exit on the next loop. */
		game_state = processKeys(hOut, state);

		expl_update();

		/* update the player */
		updatePlayer();

		/* update the spawns */
		updateSpawnList();

		/* update the bullets */
		updateBulletList();

		/* update the enemies */
		updateEnemyList();

		/* update the warper boss */
		levelThree();

		turret_fire();

		/* call the method that moves and renders the viewport */
		translate(hOut, getCurrentLevel(), &viewCoord);
	}
  turret_delete_list();
	clearlvl3();

	expl_destroy();

	deleteWarper();
	/* this function clears the enemies and spawn points after the player dies */
	clearEnemies();

	/* clear out the bullets yo. */
	clearBullets();

	unloadLevel(getCurrentLevel());

	/* test saving player data */
	savePlayerData(&player.saveFile[0], &player);

	return game_state;
}

int lvl_4(HANDLE hOut, int state)
{
	COORD viewCoord;
	int game_state = state;

	/* start the miniboss count at 0 */
	fsbCount = 0;
	/* start the boss door opened status at 0 */
	bossOpened = 0;

	/* Load a new level - sets it as the current level */
	loadLevel("maps/level4.tlf", "maps/level4U.tlf", 4);
	/* not sure - Ask Chris */

	/* initialize the number of enemies per level here */
	totalClawbots = 9;
	totalJumpers = 4;
	totalShooters = 3;
	totalSnipers = 2;

	/* reset player movement */
	keyboard.up = 0;
	keyboard.down = 0;
	keyboard.right = 0;
	keyboard.left = 0;
	keyboard.Sup = 0;
	keyboard.Sdown = 0;
	keyboard.Sright = 0;
	keyboard.Sleft = 0;

	viewCoord.X = 0;
	viewCoord.Y = 0;

	/*Mike: write text for the level in the hud*/
	if (!player.levelFourComplete)
		setTextPrompt("Watch out for long range on the Sniper\nbots. They will pick you off from a mile\naway.");
  turret_create(120, 206, 100, 5000, 5);
  turret_create(103, 220, 100, 5000, 4);
  turret_create(120, 236, 100, 5000, 5);
  turret_create(103, 252, 100, 5000, 4);
	/* main game loop - loop until game_on is false(0) */
	while(game_state == state)
	{
		/* update the timers */
		updateTimers();

		/* update the sound system */
		updateSoundSystem();

		/* call the movement function to process user input. If the user
		hits escape, plyr_movement() will return false and the game will
		exit on the next loop. */
		game_state = processKeys(hOut, state);

		expl_update();
    turret_fire();

		/* update the player */
		updatePlayer();

		/* update the spawns */
		updateSpawnList();

		/* update the bullets */
		updateBulletList();

		/* update the enemies */
		updateEnemyList();

		/* update the super tank */
		updateSuperTank();

		/* update level four specific things */
		updateLevelFour();

		/* call the method that moves and renders the viewport */
		translate(hOut, getCurrentLevel(), &viewCoord);
	}
  turret_delete_list();
	/* this function clears the enemies and spawn points after the player dies */
	clearEnemies();

	/* clear out the bullets yo. */
	clearBullets();


	unloadLevel(getCurrentLevel());

	/* test saving player data */
	savePlayerData(&player.saveFile[0], &player);

	return game_state;
}

int lvl_5(HANDLE hOut, int state)
{
	COORD viewCoord;

	int game_state = state;

	/* Load a new level - sets it as the current level */
	loadLevel("maps/level5.tlf", "maps/level5U.tlf", 5);
	/* not sure - Ask Chris */

	/* initialize the number of enemies per level here */
	totalClawbots = 20;
	totalJumpers = 20;
	totalShooters = 10;
	totalSnipers = 8;
	totalbigburlys = 10;

	/* reset player movement */
	keyboard.up = 0;
	keyboard.down = 0;
	keyboard.right = 0;
	keyboard.left = 0;
	keyboard.Sup = 0;
	keyboard.Sdown = 0;
	keyboard.Sright = 0;
	keyboard.Sleft = 0;

	viewCoord.X = 0;
	viewCoord.Y = 0;

	/*Mike: write text for the level in the hud*/
	if (!player.levelFiveComplete)
		setTextPrompt("This is it. If you can manage to get past the Burly bots and reach the boss here\nthey will be out of resources. Good Luck!");
  turret_create(224, 242, 800, 9000, 6);
  turret_create(293, 242, 800, 9000, 6);
	/* main game loop - loop until game_on is false(0) */
	while(game_state == state)
	{
		/* update the timers */
		updateTimers();

		/* update the sound system */
		updateSoundSystem();

		/* call the movement function to process user input. If the user
		hits escape, plyr_movement() will return false and the game will
		exit on the next loop. */
		game_state = processKeys(hOut, state);

		expl_update();
    turret_fire();

		/* update the player */
		updatePlayer();

		/* update the spawns */
		updateSpawnList();

		/* update the bullets */
		updateBulletList();

		/* update the enemies */
		updateEnemyList();

		/* update mauler */
		moveMauler();

		game_state = LVL_5(game_state);


		/* call the method that moves and renders the viewport */
		translate(hOut, getCurrentLevel(), &viewCoord);
	}
  turret_delete_list();
	/* this function clears the enemies and spawn points after the player dies */
	clearEnemies();
	expl_destroy();

	/* clear out the bullets yo. */
	clearBullets();

	unloadLevel(getCurrentLevel());

	/* test saving player data */
	savePlayerData(&player.saveFile[0], &player);

	return game_state;
}
