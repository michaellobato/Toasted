/*! ************************************************************************
\file GraphicsSystem.c
\author Chris Howard
\par email: choward@digipen.edu
\par Game: Toasted
\date January 16th, 2007
\brief Handles all of the Input and Output between the windows.h
functions
***************************************************************************/

#include <windows.h>
#include "LevelManager.h"
#include "player.h"
#include "GraphicsSystem.h"
#include <stdio.h>
#include "EnemyManager.h"
#include "WeaponManager.h"
#include "hud.h"
#include "effects.h"
#include "MenuSystem.h"
#include "LevelFour.h"
#include "LevelFive.h"
#include "Sound.h"
#include "LoadSaves.h"
#include "LevelThree.h"
#include "LevelOne.h"
#include "LevelTwo.h"

/*! \brief Handles the key presses the player enters appropriately.

\param HANDLE hOut - The window we are currently operating on
\return An int acting as a boolean value to stop the loop in driver.c 
when 'x' is pressed
\sa plyr_movement(HANDLE hOut)
*/

/**************************************************
////Mike/////
function    mainMenuControls
Discription handles all of the controls when in the main menu
input       InRec - if and what buttons have been pressed
            state - current game state
return      int - new game state
***************************************************/
int mainMenuControls(INPUT_RECORD InRec, int state)
{
	if(InRec.Event.KeyEvent.bKeyDown)
	{
		/*Mike: stuff to move the cursor down*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_DOWN) && (menu_cursor.selection != EXIT_GAME))
		{
			MoveCursor(MOVE_CURSOR_DOWN);
		}
		/*Mike: stuff to move the cursor up*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_UP) && (menu_cursor.selection != NEW_GAME))
		{
			MoveCursor(MOVE_CURSOR_UP);
		}
		/*Mike: changed the if to include the new menu cursor*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) && (menu_cursor.selection == NEW_GAME))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_LOAD_MENU;
		}
		/*Mike: changed the if to include the new menu cursor*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) && (menu_cursor.selection == LOAD_GAME))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_LOAD_MENU;
		}
		/*Mike: changed the if to include the new menu cursor*/
		if ( ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) && 
			(menu_cursor.selection == EXIT_GAME)) || 
			(InRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_END_GAME;
		}
	}
	return state;
}

/**************************************************
////Mike/////
function    enterGameControls
Discription handles all of the controls when at the copyright screen
input       InRec - if and what buttons have been pressed
            state - current game state
return      int - new game state
***************************************************/
int enterGameControls(INPUT_RECORD InRec)
{
  if(InRec.Event.KeyEvent.bKeyDown)
	{
		return STATE_MAIN_MENU;
  }
  return STATE_COPYRIGHT_MENU;
}

/*! \brief Handles the key presses the player enters appropriately.

\param HANDLE hOut - The window we are currently operating on
\return An int acting as a boolean value to stop the loop in driver.c 
when 'x' is pressed
\sa plyr_movement(HANDLE hOut)
*/
int gameOverControls(INPUT_RECORD InRec, int state)
{

	if(InRec.Event.KeyEvent.bKeyDown)
	{
		/*Mike: go back to the overworl if r is pressed, added capital R *
		*because I thought the controls were broken but realized caps   *
		* lock was on                                                   */
		if ((InRec.Event.KeyEvent.uChar.AsciiChar == 'r') ||
			(InRec.Event.KeyEvent.uChar.AsciiChar == 'R'))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_OVER_WORLD;
		}
		/*go back to the overworld if enter is pressed*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_OVER_WORLD;
		}
		/*Mike: go back to the main menu if escape pressed*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_MAIN_MENU;
		}
	}
	return state;
}
/*! \brief Handles the key presses the player enters appropriately.

\param HANDLE hOut - The window we are currently operating on
\return An int acting as a boolean value to stop the loop in driver.c 
when 'x' is pressed
\sa plyr_movement(HANDLE hOut)
*/
int storyScreenControls(INPUT_RECORD InRec, int state)
{

	if(InRec.Event.KeyEvent.bKeyDown)
	{
		/*go back to the overworld if enter is pressed*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_OVER_WORLD;
		}
	}
	return state;
}
/**************************************************
////Mike/////
function    VictoryScreenControls
input InRec - information about what is pressed
      state - enum to the state of the game
return an int (enum) for the game state
***************************************************/
int VictoryScreenControls(INPUT_RECORD InRec, int state)
{

	if(InRec.Event.KeyEvent.bKeyDown)
	{
		/*go back to the overworld if enter is pressed*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_CREDITS_MENU;
		}
		/*go back to the main menu*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_CREDITS_MENU;
		}
	}
	return state;
}
/**************************************************
////Mike/////
function    LoadGameControls
input  InRec - information about what is pressed
       state - enum to the state of the game
return      an int (enum) for the game state
***************************************************/
int LoadMenuControls(INPUT_RECORD InRec, int state)
{
	if(InRec.Event.KeyEvent.bKeyDown)
	{
		/*go back to the main menu*/
		if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
		{
			/* play the select menu sound */
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_MAIN_MENU;
		}
		/*continue on to the game if a save has been selected*/
		if ( (InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) && 
			(menu_cursor.selection == LOAD_GAME) )
		{
			/* play the select menu sound */
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_OVER_WORLD;
		}
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
			(menu_cursor.selection == NEW_GAME))
		{
			/* play the select menu sound */
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_NEW_MENU;
		}

		/*stuff to move the cursor down*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_DOWN) && (saved_files.selected_save < 4))
		{
			/* play the change menu sound */
			playSoundCopy(&soundMenuChange, 100);
			saved_files.selected_save++;
		}
		/*stuff to move the cursor up*/
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_UP) && (saved_files.selected_save > 0))
		{
			/* play the change menu sound */
			playSoundCopy(&soundMenuChange, 100);
			saved_files.selected_save--;
		}
	}
	return state;
}



/**************************************************
////Mike/////
function    NewGameControls
input InRec - information about what is pressed
      state - enum to the state of the game
return      an int (enum) for the game state
***************************************************/
int NewGameControls(INPUT_RECORD InRec, int state)
{

	if(InRec.Event.KeyEvent.bKeyDown)
	{
		/*go back to the main menu*/
		if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
		{
			return STATE_MAIN_MENU;
		}
		/*continue on with the game if a name has been entered*/
		else if ( (InRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) && (saveIndex > 0))
		{
			playSoundCopy(&soundMenuSelect, 100);
			return STATE_STORY;
		}
		/*if backspace is pressed*/
		else if (InRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
		{	
			playSoundCopy(&soundMenuChange, 100);
			/*if there is a character in the save name buffer*/
			if (saveIndex > 0)
			{
				saveIndex--;
				saveFileName[saveIndex] = 0;
			}
		}
		else
		{
			/*allow the player to enter characters for their name if there is room*/
			if (saveIndex <  MAX_NAME_LENGTH)
			{
				char in_char;

				in_char = InRec.Event.KeyEvent.uChar.AsciiChar;

				if ( (in_char >= 'A') && (in_char <= 'z') && (in_char != '\\') &&
					(in_char != '[') && (in_char != ']') && (in_char != '`') &&
					(in_char != '^'))
				{
					playSoundCopy(&soundMenuChange, 100);
					saveFileName[saveIndex] = in_char;
					saveIndex++;
				}
			}
		}
	}
	return state;
}

/*! \brief Handles the key presses the player enters appropriately.

\param HANDLE hOut - The window we are currently operating on
\return An int acting as a boolean value to stop the loop in driver.c 
when 'x' is pressed
\sa plyr_movement(HANDLE hOut)
*/
int processKeys(HANDLE hOut, int state)
{
	unsigned int i;	
	/* struct holding what was done on the keyboard */
	INPUT_RECORD InRec;

	/* struct holding where input will be read from */
	HANDLE hIn;

	/* a record of what has been read*/
	DWORD NumRead;

	/* store the number of events */
	DWORD numEvents;

	hOut = hOut;

	/* sets the input to the keyboard */
	hIn = GetStdHandle(STD_INPUT_HANDLE);

	/* check to see how many console events have happened,
	necessary to prevent blocking */
	GetNumberOfConsoleInputEvents(hIn, &numEvents);


	/* see if the player is out of health */
	if(player.hp <= 0)
	{
		/* go to game over screen */
		return STATE_GAME_OVER;
	}

	/* if there were events, process the keyboard ones. */
	if(numEvents > 0)
	{
		/* reads input from the keyboard, and stores it in InRec */
		ReadConsoleInput(hIn,&InRec,1,&NumRead);

		for(i = 0; i < NumRead; i++);
		{
			/* if something happens on the keyboard */
			if(InRec.EventType == KEY_EVENT)
			{	
				/* send the input to the appropriate place */
				if(state >= STATE_OVER_WORLD)
				{
					/* send it to the player moment */
					return plyr_movement(InRec, state);
				}
        else if(state == STATE_COPYRIGHT_MENU)
        {
          return enterGameControls(InRec);
        }
				else if(state == STATE_MAIN_MENU)
				{
					return mainMenuControls(InRec, state);
				}
				else if(state == STATE_LOAD_MENU)
				{
					return LoadMenuControls(InRec, state);
				}
				else if(state == STATE_NEW_MENU)
				{
					return NewGameControls(InRec, state);
				}
				else if(state == STATE_GAME_OVER)
				{
					return gameOverControls(InRec, state);
				}
				else if(state == STATE_STORY)
				{
					return storyScreenControls(InRec, state);
				}
				else if(state == STATE_VICTORY)
				{
					return VictoryScreenControls(InRec, state);
				}
			}
		}
	}
	return state;
}

/*! \brief Handles the key presses the player enters appropriately.

\param HANDLE hOut - The window we are currently operating on
\return An int acting as a boolean value to stop the loop in driver.c 
when 'x' is pressed
\sa plyr_movement(HANDLE hOut)
*/

int plyr_movement(INPUT_RECORD InRec, int state)
{
	switch (getLevelPosition(SWITCH_LAYER, player.X, player.Y)->character)
	{
	case '0':
		return STATE_OVER_WORLD;
	case '1':
		return STATE_LEVEL_ONE;
	case '2':
		return STATE_LEVEL_TWO;
	case '3':
		return STATE_LEVEL_THREE;
	case '4':
		return STATE_LEVEL_FOUR;
	case '5':
		return STATE_LEVEL_FIVE;
	}
	switch(InRec.Event.KeyEvent.bKeyDown)
	{
	case 1:

		/*Mike: Use e to cycle right between weapons*/
		if ((InRec.Event.KeyEvent.uChar.AsciiChar == 'e') || 
        (InRec.Event.KeyEvent.uChar.AsciiChar == 'E'))
		{
			nextWeapon(2);
		}

		/*Mike: Use q to cycle left between weapons*/
		if ((InRec.Event.KeyEvent.uChar.AsciiChar == 'q') || 
        (InRec.Event.KeyEvent.uChar.AsciiChar == 'Q'))
		{
			nextWeapon(-2);
		}

		/* change weapon */
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == VK_SPACE))
		{
			nextWeapon(1);
		}		
		
		/* change weapon */
		if ((InRec.Event.KeyEvent.uChar.AsciiChar == 'p') ||
			(InRec.Event.KeyEvent.uChar.AsciiChar == 'P'))
		{
			pauseTimer(!timerPaused);
		}

		if ((InRec.Event.KeyEvent.wVirtualKeyCode == 0x31))
		{
			if(player.weaponsEnabled[0])
			{
				if(player.currentWeapon == GUN_PISTOL)
				{
					if(player.weaponsEnabled[1])
					{
						player.currentWeapon = GUN_DUALPISTOL;
					}
				}
				else
				{
					player.currentWeapon = GUN_PISTOL;
				}
			}
		}
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == 0x32))
		{
			if(player.weaponsEnabled[2])
			{
				if(player.currentWeapon == GUN_SPREADGUN)
				{
					if(player.weaponsEnabled[3])
					{
						player.currentWeapon = GUN_SCATTERGUN;
					}
				}
				else
				{
					player.currentWeapon = GUN_SPREADGUN;
				}
			}
		}
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == 0x33))
		{
			if(player.weaponsEnabled[4])
			{
				if(player.currentWeapon == GUN_SUBMACHINEGUN)
				{
					if(player.weaponsEnabled[5])
					{
						player.currentWeapon = GUN_HEAVYMACHINEGUN;
					}
				}
				else
				{
					player.currentWeapon = GUN_SUBMACHINEGUN;
				}
			}
		}
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == 0x34))
		{
			if(player.weaponsEnabled[6])
			{
				if(player.currentWeapon == GUN_RIFLE)
				{
					if(player.weaponsEnabled[7])
					{
						player.currentWeapon = GUN_SNIPER;
					}
				}
				else
				{
					player.currentWeapon = GUN_RIFLE;
				}
			}
		}
		if ((InRec.Event.KeyEvent.wVirtualKeyCode == 0x35))
		{
			if(player.weaponsEnabled[8])
			{
				if(player.currentWeapon == GUN_ROCKETLAUNCHER)
				{
					if(player.weaponsEnabled[9])
					{
						player.currentWeapon = GUN_OMEGALASER;
					}
				}
				else
				{
					player.currentWeapon = GUN_ROCKETLAUNCHER;
				}
			}
		}

		/* movement and firing */
		switch(InRec.Event.KeyEvent.wVirtualKeyCode)
		{
		case VK_ESCAPE:
			return STATE_MAIN_MENU;
		case UP_KEY_CODE:
			keyboard.up = 1;
			break;
		case DOWN_KEY_CODE:
			keyboard.down = 1;
			break;
		case LEFT_KEY_CODE:
			keyboard.left = 1;
			break;
		case RIGHT_KEY_CODE:
			keyboard.right = 1;
			break;
			/* shooting controls */
		case VK_RIGHT:
			keyboard.Sright = 1;
			break;
		case VK_LEFT:
			keyboard.Sleft = 1;
			break;
		case VK_UP:
			keyboard.Sup = 1;
			break;	
		case VK_DOWN:
			keyboard.Sdown = 1;
			break;
		case VK_SHIFT:
			keyboard.Gernade = 1;
			break;
		}

		break;
	case 0:
		switch (InRec.Event.KeyEvent.wVirtualKeyCode)
		{
		case UP_KEY_CODE:
			keyboard.up = 0;
			break;
		case DOWN_KEY_CODE:
			keyboard.down = 0;
			break;
		case LEFT_KEY_CODE:
			keyboard.left = 0;
			break;
		case RIGHT_KEY_CODE:
			keyboard.right = 0;
			break;
			/* shooting controls */
		case VK_RIGHT:
			keyboard.Sright = 0;
			break;
		case VK_LEFT:
			keyboard.Sleft = 0;
			break;
		case VK_UP:
			keyboard.Sup = 0;
			break;	
		case VK_DOWN:
			keyboard.Sdown = 0;
			break;
		case VK_SHIFT:
			keyboard.Gernade = 0;
			break;
		}

		break;
	}

	return state;
}

/*! \brief Takes the virtual map and translates it onto the screen 
directly on the screen buffer

\param HANDLE hOut - The window we are currently operating on
\param LEVEL *lvl - The current level we are on, to get the 
total level size
\param COORD *viewCoord - The X and Y of the top left corner of the
viewable area of the map
\return TRUE if the thing didnt fail

\sa  int translate(HANDLE hOut, LEVEL *lvl, COORD *viewCoord)
*/

int translate(HANDLE hOut, LEVEL *lvl, COORD *viewCoord)
{
	/* simple counters */
	int i = 6;
	int j = 0;

	/* windows BS thingy that allows me to get the viewing rectangle */
	CONSOLE_SCREEN_BUFFER_INFO info; 
	/* holds the size of the array of chars(viewport size) */
	COORD arraysize;
	/* holds the top left coordinate of where to start drawing from in the array
	(always 0,0)
	*/
	COORD screensize;
	/* the rectangle which holds the dimensions of the viewing rectangle */
	SMALL_RECT viewRect;
	/* An array of chars (with color formatting) that make up what is printed on screen*/
	CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH];

	/* set the arraysize to he size of the screen */
	arraysize.X = CONSOLE_WIDTH;
	arraysize.Y = CONSOLE_HEIGHT;

	/* sets the top left to 0,0 */
	screensize.X = 0;
	screensize.Y = 0;

	/* basically this gets the size of the viewing rectangle */
	GetConsoleScreenBufferInfo(hOut, &info);
	viewRect = info.srWindow; 

	WriteHUD(cha);

	/* checks to see if the player is outside of the middle of the screen
	and that if it moves itself to keep the player inside the center that
	it won't be tryint to draw outside of the lvl, because that causes 
	runtime errors
	*/
	if (player.X - viewCoord->X > CONSOLE_WIDTH - (CONSOLE_WIDTH / 3))
	{
		if(viewCoord->X+(CONSOLE_WIDTH + 1) < lvl->width)
		{
			viewCoord->X = viewCoord->X + 1;
		}	
	}
	if (player.X - viewCoord->X < CONSOLE_WIDTH -  3 * (CONSOLE_WIDTH / 5))
	{
		if(viewCoord->X-1 > 0)
		{
			viewCoord->X = viewCoord->X - 1;
		}	
	}

	if (player.Y - viewCoord->Y > CONSOLE_HEIGHT - (CONSOLE_HEIGHT / 2))
	{
		if(viewCoord->Y+(CONSOLE_HEIGHT)-5 < lvl->height)
		{
			viewCoord->Y = viewCoord->Y + 1;
		}	
	}

	if (player.Y - viewCoord->Y < CONSOLE_HEIGHT -  3 * (CONSOLE_HEIGHT / 5))
	{
		if(viewCoord->Y > 0)
		{
			viewCoord->Y = viewCoord->Y - 1;
		}	
	}


	/* the two nested loops go from the top left of the screen to the bottom right of the screen*/
	for(; i < CONSOLE_HEIGHT; i++)
	{
		for(; j < CONSOLE_WIDTH; j++)
		{
			/* background color attributes */
			WORD backColor = *((WORD*)(getLevelPosition(UNDER_LAYER, j+viewCoord->X,i+viewCoord->Y-5)->data));
			/* checks to see what is in the current space we are looking at
			and sets that area to the appropriate char and appropriate colors
			*/
			if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'X')
			{
				cha[i][j].Char.AsciiChar = 'X';
				cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED | backColor | BACKGROUND_BLUE);
			}

			else if(getLevelPosition(PLAYER_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'P')
			{
				cha[i][j].Char.AsciiChar = 'X';
				cha[i][j].Attributes = backColor | player.color.Attributes;
			}

			/* If we find an empty */
			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'A')
			{
				cha[i][j].Char.AsciiChar = ' ';
				cha[i][j].Attributes = 0;
			}
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '!')
			{
				cha[i][j].Char.AsciiChar = 'T';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			/* if we find an enemy (clawbot) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'c')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY*)(lvl_pos->data))->type;
				cha[i][j].Attributes = (backColor | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}

			/* if we find an enemy (jumper) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'z')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY*)(lvl_pos->data))->type;
				cha[i][j].Attributes = (backColor | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}

			/* if we find an enemy (shooter) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 't')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY*)(lvl_pos->data))->type;
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}

			/* if we find an enemy (sniper) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'y')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY*)(lvl_pos->data))->type;
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}

			/* if we find an enemy (BigBurly) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'b')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY*)(lvl_pos->data))->type;
				cha[i][j].Attributes = (backColor | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			/* If we find a spawn point (clawbot) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'C')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY_SPAWN*)(lvl_pos->data))->type;

				if (((ENEMY_SPAWN*)(lvl_pos->data))->flashing == 1)
				{
					cha[i][j].Attributes = (BACKGROUND_RED | BACKGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_INTENSITY);
				}
			}

			/* If we find a spawn point (jumper) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'Z')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY_SPAWN*)(lvl_pos->data))->type;

				if (((ENEMY_SPAWN*)(lvl_pos->data))->flashing == 1)
				{
					cha[i][j].Attributes = (BACKGROUND_RED | backColor | BACKGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				}
			}
			/* If we find a spawn point (shooter) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'T')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY_SPAWN*)(lvl_pos->data))->type;

				if (((ENEMY_SPAWN*)(lvl_pos->data))->flashing == 1)
				{
					cha[i][j].Attributes = (BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
				}
			}

			/* If we find a spawn point (sniper) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'Y')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY_SPAWN*)(lvl_pos->data))->type;

				if (((ENEMY_SPAWN*)(lvl_pos->data))->flashing == 1)
				{
					cha[i][j].Attributes = (BACKGROUND_BLUE | backColor | BACKGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				}
			}

			/* If we find a spawn point (BigBurly) */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'B')
			{
				LEVEL_POSITION* lvl_pos = getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5);
				cha[i][j].Char.AsciiChar = ((ENEMY_SPAWN*)(lvl_pos->data))->type;

				if (((ENEMY_SPAWN*)(lvl_pos->data))->flashing == 1)
				{
					cha[i][j].Attributes = (BACKGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (FOREGROUND_INTENSITY);
				}
			}
			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '?')
			{
				cha[i][j].Char.AsciiChar = '=';
				cha[i][j].Attributes = (FOREGROUND_RED);
			}
			else if(getLevelPosition(WEAPON_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '?')
			{
				cha[i][j].Char.AsciiChar = 'A';
				cha[i][j].Attributes = (BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '?')
			{
				cha[i][j].Char.AsciiChar = '|';
				cha[i][j].Attributes = railgunner.info->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'q')
			{
				cha[i][j].Char.AsciiChar = '*';
				cha[i][j].Attributes = (BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED);
			}

			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'Q')
			{
				cha[i][j].Char.AsciiChar = 'Q';
				cha[i][j].Attributes = (BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
			else if(getLevelPosition(WEAPON_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '/')
			{
				cha[i][j].Char.AsciiChar = '/';
				cha[i][j].Attributes = (backColor | FOREGROUND_RED | 
					FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			else if(getLevelPosition(WEAPON_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '\\')
			{
				cha[i][j].Char.AsciiChar = '\\';
				cha[i][j].Attributes = (backColor | FOREGROUND_RED | 
					FOREGROUND_INTENSITY);
			}
			/* display health  */
			else if(getLevelPosition(HEALTH_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'H')
			{
				cha[i][j].Char.AsciiChar = '+';
				cha[i][j].Attributes = (BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}

			/* display switch u */
			else if(getLevelPosition(SWITCH_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'u')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display switch s */
			else if(getLevelPosition(SWITCH_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 's')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display switch r */
			else if(getLevelPosition(SWITCH_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'r')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display switch v */
			else if(getLevelPosition(SWITCH_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'v')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display switch walls for S */
			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'S')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_BLUE);
			}
			/* display switch walls for U */
			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'U')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_BLUE);
			}
			/* display switch walls for V */
			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'V')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_BLUE);
			}
			/* display switch walls for R */
			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'R')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_BLUE);
			}


			/* display switch walls for invisible switches once visible */
			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'I')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_BLUE);
			}
			/* display switch walls for invisible switches once visible */
			else if(getLevelPosition(WALL_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'G')
			{
				cha[i][j].Char.AsciiChar = 'O';
				cha[i][j].Attributes = (BACKGROUND_BLUE | FOREGROUND_BLUE);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'W')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == ']')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == ')')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '[')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '(')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '\"')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '\'')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '^')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(PICKUP_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '~')
			{
				cha[i][j].Char.AsciiChar = 'W';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* display Weapon  */
			else if(getLevelPosition(WEAPON_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '*')
			{
				BULLET* thisBullet = (BULLET*)getLevelPosition(WEAPON_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->data;
				cha[i][j].Char.AsciiChar = thisBullet->graphic;

				/* make the gernade black */
				if (thisBullet->ID == GUN_GERNADE)
				{
					cha[i][j].Attributes = (backColor);
				}

				/* make the laser red */
				if (thisBullet->ID == GUN_OMEGALASER)
				{
					cha[i][j].Attributes = (BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_INTENSITY);
				}

				/* make all other bullets blue */
				if (thisBullet->owner == OWNER_PLAYER && !(thisBullet->ID == GUN_OMEGALASER)
					&& !(thisBullet->ID == GUN_GERNADE))
				{
					cha[i][j].Attributes = (backColor | FOREGROUND_BLUE);
				}

				/* if the bullet came from an enemy, make it white */
				else if (thisBullet->owner == OWNER_ENEMY)
				{
					cha[i][j].Attributes = (backColor | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				}
			}
			/* this is the representation of the explosion */
			else if(getLevelPosition(WEAPON_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '.')
			{
				CHAR_INFO temp = expl_anim(getLevelPosition(WEAPON_LAYER,j+viewCoord->X,i+viewCoord->Y-5));
				cha[i][j].Char.AsciiChar = temp.Char.AsciiChar;
				cha[i][j].Attributes = backColor | temp.Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == ',')
			{
				cha[i][j].Char.AsciiChar = ',';
				cha[i][j].Attributes = (backColor | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '+')
			{
				cha[i][j].Char.AsciiChar = '+';
				cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}

			/* LevelOneBoss */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == '#')
			{
				cha[i][j].Char.AsciiChar = 'X';

				if (levelOneBoss.flashing == 1)
				{
					cha[i][j].Attributes = (backColor | FOREGROUND_RED | FOREGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (backColor);
				}
			}

			/* LevelOneMiniBoss */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'm')
			{
				cha[i][j].Char.AsciiChar = 'M';

				if (levelOneMiniBoss.flashing == 1)
				{
					cha[i][j].Attributes = (backColor | FOREGROUND_RED | FOREGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (backColor);
				}
			}

			/* LevelTwoMiniBoss */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'n')
			{
				cha[i][j].Char.AsciiChar = '@';

				if (levelTwoMiniBoss.flashing == 1)
				{
					cha[i][j].Attributes = (backColor | FOREGROUND_RED | FOREGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (backColor);
				}
			}

			/* LevelTwoBoss */
			else if(getLevelPosition(ENEMY_LAYER,j+viewCoord->X,i+viewCoord->Y-5)->character == 'a')
			{
				cha[i][j].Char.AsciiChar = '?';

				if (levelTwoBoss.flashing == 1)
				{
					cha[i][j].Attributes = (backColor | FOREGROUND_RED | FOREGROUND_INTENSITY);
				}
				else
				{
					cha[i][j].Attributes = (backColor);
				}
			}

			/* super tank stuff */
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '&')
			{
				CHAR_INFO *superTankChar = 
					getSuperTankChar((j+viewCoord->X)-superTank.xPosition, 
					(i+viewCoord->Y-5) - superTank.yPosition);
				cha[i][j].Char.AsciiChar = superTankChar->Char.AsciiChar;
				cha[i][j].Attributes = superTankChar->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '=')
			{
				CHAR_INFO *fsbChar = 
					getFSBChar(fsbOne, (j+viewCoord->X) - fsbOne->xPosition, 
					(i+viewCoord->Y-5) - fsbOne->yPosition);
				cha[i][j].Char.AsciiChar = fsbChar->Char.AsciiChar;
				cha[i][j].Attributes = fsbChar->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'K')
			{
				CHAR_INFO *fsbChar = 
					getFSBChar(fsbTwo, (j+viewCoord->X) - fsbTwo->xPosition, 
					(i+viewCoord->Y-5) - fsbTwo->yPosition);
				cha[i][j].Char.AsciiChar = fsbChar->Char.AsciiChar;
				cha[i][j].Attributes = fsbChar->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'o')
			{
				CHAR_INFO *fsbChar = 
					getFSBChar(fsbThree, (j+viewCoord->X) - fsbThree->xPosition, 
					(i+viewCoord->Y-5) - fsbThree->yPosition);
				cha[i][j].Char.AsciiChar = fsbChar->Char.AsciiChar;
				cha[i][j].Attributes = fsbChar->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'k')
			{
				CHAR_INFO *fsbChar = 
					getFSBChar(fsbFour, (j+viewCoord->X) - fsbFour->xPosition, 
					(i+viewCoord->Y-5) - fsbFour->yPosition);
				cha[i][j].Char.AsciiChar = fsbChar->Char.AsciiChar;
				cha[i][j].Attributes = fsbChar->Attributes;
			}
			else if(getLevelPosition(WALL_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '$')
			{
				CHAR_INFO *MainBossChar = 
					getMainBossChar((j+viewCoord->X)-mainbossX(), 
					(i+viewCoord->Y-5) - mainbossY());
				cha[i][j].Char.AsciiChar = MainBossChar->Char.AsciiChar;
				cha[i][j].Attributes = MainBossChar->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '$')
			{
				CHAR_INFO *MainBossChar = 
					getMainBossChar((j+viewCoord->X)-mainbossX(), 
					(i+viewCoord->Y-5) - mainbossY());
				cha[i][j].Char.AsciiChar = MainBossChar->Char.AsciiChar;
				cha[i][j].Attributes = MainBossChar->Attributes;
			}
			else if(getLevelPosition(WALL_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '~')
			{
				CHAR_INFO *RarmChar = 
					getRarmChar((j+viewCoord->X)-RarmX(), 
					(i+viewCoord->Y-5) - RarmY());
				cha[i][j].Char.AsciiChar = RarmChar->Char.AsciiChar;
				cha[i][j].Attributes = RarmChar->Attributes;
			}
			else if(getLevelPosition(WALL_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'L')
			{
				CHAR_INFO *LarmChar = 
					getLarmChar((j+viewCoord->X)-LarmX(), 
					(i+viewCoord->Y-5) - LarmY());
				cha[i][j].Char.AsciiChar = LarmChar->Char.AsciiChar;
				cha[i][j].Attributes = LarmChar->Attributes;
			}

			else if(getLevelPosition(WALL_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'l')
			{
				CHAR_INFO *LhandChar = 
					getLhandChar((j+viewCoord->X)-LhandX(), 
					(i+viewCoord->Y-5) - LhandY());
				cha[i][j].Char.AsciiChar = LhandChar->Char.AsciiChar;
				cha[i][j].Attributes = LhandChar->Attributes;
			}
			else if(getLevelPosition(WALL_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '`')
			{
				CHAR_INFO *RhandChar = 
					getRhandChar((j+viewCoord->X)-RhandX(), 
					(i+viewCoord->Y-5) - RhandY());
				cha[i][j].Char.AsciiChar = RhandChar->Char.AsciiChar;
				cha[i][j].Attributes = RhandChar->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'l')
			{
				CHAR_INFO *LhandChar = 
					getLhandChar((j+viewCoord->X)-LhandX(), 
					(i+viewCoord->Y-5) - LhandY());
				cha[i][j].Char.AsciiChar = LhandChar->Char.AsciiChar;
				cha[i][j].Attributes = LhandChar->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '`')
			{
				CHAR_INFO *RhandChar = 
					getRhandChar((j+viewCoord->X)-RhandX(), 
					(i+viewCoord->Y-5) - RhandY());
				cha[i][j].Char.AsciiChar = RhandChar->Char.AsciiChar;
				cha[i][j].Attributes = RhandChar->Attributes;
			}

			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '~')
			{
				CHAR_INFO *RarmChar = 
					getRarmChar((j+viewCoord->X)-RarmX(), 
					(i+viewCoord->Y-5) - RarmY());
				cha[i][j].Char.AsciiChar = RarmChar->Char.AsciiChar;
				cha[i][j].Attributes = RarmChar->Attributes;
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'L')
			{
				CHAR_INFO *LarmChar = 
					getLarmChar((j+viewCoord->X)-LarmX(), 
					(i+viewCoord->Y-5) - LarmY());
				cha[i][j].Char.AsciiChar = LarmChar->Char.AsciiChar;
				cha[i][j].Attributes = LarmChar->Attributes;
			}

			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '>')
			{
				cha[i][j].Char.AsciiChar = '>';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '-')
			{
				cha[i][j].Char.AsciiChar = '-';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			else if(getLevelPosition(ENEMY_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == '|')
			{
				cha[i][j].Char.AsciiChar = '|';
				cha[i][j].Attributes = (backColor | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			/* end super tank stuff */

			/* after everything else, we render the 'under' layer */
			/* Paths (under map) */
			else if(getLevelPosition(UNDER_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'B')
			{
				cha[i][j].Char.AsciiChar = ' ';
				cha[i][j].Attributes = (BACKGROUND_GREEN | BACKGROUND_RED);
			}
			/* Shadows (under map) */
			else if(getLevelPosition(UNDER_LAYER, j+viewCoord->X, i+viewCoord->Y-5)->character == 'S')
			{
				cha[i][j].Char.AsciiChar = ' ';
				cha[i][j].Attributes = (BACKGROUND_INTENSITY);
			}
			/* Default (under map) */
			else
			{
				cha[i][j].Char.AsciiChar = ' ';
				cha[i][j].Attributes = (backColor);
			}
		}

		j = 0;

		/* draw the pause menu on top of everything else */
		if(timerPaused)
		{
			drawPauseScreen(cha);
		}
	}
	/* writes the array we just made to the screen all at once
	SUPER FAST STYLE!!!!!!
	*/
	WriteConsoleOutput(hOut, (CHAR_INFO*)&cha, arraysize, screensize, &viewRect);


	return TRUE;
}

/*! \brief takes in a pre made character array containing one screen of info
and puts it on the screen

\param HANDLE hOut - The window we are currently operating on
\param CHAR_INFO cha - The character array to be rendered
\return TRUE if the thing didnt fail

\sa  int paint(HANDLE hOut, CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH])
*/

int paint(HANDLE hOut, CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH])
{
	int i;
	int j;

	CHAR_INFO trans[CONSOLE_HEIGHT][CONSOLE_WIDTH];
	/* windows BS thingy that allows me to get the viewing rectangle */
	CONSOLE_SCREEN_BUFFER_INFO info; 
	/* holds the size of the array of chars(viewport size) */
	COORD arraysize;
	/* holds the top left coordinate of where to start drawing from in the array
	(always 0,0)
	*/
	COORD screensize;
	/* the rectangle which holds the dimensions of the viewing rectangle */
	SMALL_RECT viewRect;

	/* set the arraysize to he size of the screen */
	arraysize.X = CONSOLE_WIDTH;
	arraysize.Y = CONSOLE_HEIGHT;

	/* sets the top left to 0,0 */
	screensize.X = 0;
	screensize.Y = 0;

	/* basically this gets the size of the viewing rectangle */
	GetConsoleScreenBufferInfo(hOut, &info);
	viewRect = info.srWindow; 

	for(i = 0; i < CONSOLE_HEIGHT; i++)
	{
		/* read in all the characters in the line and store their characer values in the array */
		for(j = 0; j < CONSOLE_WIDTH; j++)
		{
			trans[i][j] = cha[i][j];
		}
	}

	/* writes the array we just made to the screen all at once
	SUPER FAST STYLE!!!!!!
	*/
	WriteConsoleOutput(hOut, (CHAR_INFO*)&trans, arraysize, screensize, &viewRect);

	return TRUE;
}

void setPauseText(char *newText)
{
	strcpy(pauseText, newText);
}

void drawPauseScreen(CHAR_INFO cha[][CONSOLE_WIDTH])
{
	int pauseX = 10;
	int pauseY = 13;
	int pauseWidth = 60, pauseHeight = 30;
	int i, j, x, y;
	unsigned int textCounter, pauseTextX, pauseTextY;

	CHAR_INFO pauseChar;

	char *contText = "- Press ( P ) To Continue -";

	pauseChar.Char.AsciiChar = ' ';
	pauseChar.Attributes = (BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE 
		| FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	for(i = 0; i < pauseHeight; i++)
	{
		for(j = 0; j < pauseWidth; j++)
		{
			if(j == 0 || j == pauseWidth - 1)
			{
				cha[i + pauseY][j + pauseX].Char.AsciiChar = (CHAR)186;
			}
			else if(i == 0 || i == pauseHeight - 1)
			{
				cha[i + pauseY][j + pauseX].Char.AsciiChar = (CHAR)205;
			}
			else
			{
				cha[i + pauseY][j + pauseX].Char.AsciiChar = ' ';
			}

			cha[i + pauseY][j + pauseX].Attributes = pauseChar.Attributes;
		}
	}

	/* draw the corners */
	cha[pauseY][pauseX].Char.AsciiChar = (CHAR)201;

	cha[pauseY][pauseX].Attributes = pauseChar.Attributes;

	cha[pauseY + pauseHeight - 1][pauseX].Char.AsciiChar = (CHAR)200;

	cha[pauseY + pauseHeight - 1][pauseX].Attributes = pauseChar.Attributes;

	cha[pauseY][pauseX + pauseWidth - 1].Char.AsciiChar = (CHAR)187;

	cha[pauseY][pauseX + pauseWidth - 1].Attributes = pauseChar.Attributes;
	
	cha[pauseY + pauseHeight - 1][pauseX + pauseWidth - 1].Char.AsciiChar = (CHAR)188;

	cha[pauseY + pauseHeight - 1][pauseX + pauseWidth - 1].Attributes = pauseChar.Attributes;

	for(i = 0; i < (signed)strlen(contText); i++)
	{
		cha[pauseY + pauseHeight - 3][pauseX + 17 + i].Char.AsciiChar = contText[i];

		cha[pauseY + pauseHeight - 3][pauseX + 17 + i].Attributes = pauseChar.Attributes;
	}

	/* write the text */
	pauseTextX = pauseX + 4;
	pauseTextY = pauseY + 4;
	textCounter = 0;

	/* loop through each row */
	for(y = 0; y < 10; y++)
	{
		/* loop through each collum */
		for(x = 0; x < 50; x++)
		{
			if(textCounter < strlen(pauseText))
			{
				/* if its a newline, move to the next y */
				if(pauseText[textCounter] == '\n')
				{
					y++;
					
					/* set the x back to 0 */
					x = -1;
				}
				else
				{
					cha[y + pauseTextY][x + pauseTextX].Char.AsciiChar = pauseText[textCounter];
					cha[y + pauseTextY][x + pauseTextX].Attributes = pauseChar.Attributes;
				}
				textCounter++;
			}
		}
	}
}

char * loadFileToString(char *fileName)
{
	FILE *file;
	char *fileString;
	/* line buffer to hold in each line as we read it */
	char lineBuffer[81] = {0};

	/* this will hold the longest line we've read - to calculate level width */
	int longestLine = 0;

	/* this will count the number of lines in the file - to calculate the level
		height */
    int lineCount = 0;

	/* open the file */
	file = fopen(fileName, "rt");

	if(file == NULL)
	{
		return 0;
	}

	/* loop through the entire file and read information on its size and such */
	while(fgets(&lineBuffer[0], 81, file) != NULL)
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

	/* create space for the file string */
	fileString = malloc(sizeof(char) * longestLine * lineCount + sizeof(char));
	fileString[0] = 0;

	/* rewind the file to the beginning */
	rewind(file);

	/* loop through the entire file and read information and create our level */
	while(fgets(&lineBuffer[0], 81, file) != NULL)
	{
		strcat(&fileString[0], &lineBuffer[0]);
	}

	return fileString;
}
