/***************************************************************************
  Author: Michael Lobato
  Game:	  Toasted
  File:	  MenuSystem.h
  Date:	  Feb. 10, 2007

  Description:
          Nothing special here, just all of the defines and globals for 
          MenuSystem.c
***************************************************************************/
#ifndef MAIN_MENU
#define MAIN_MENU

#include "GraphicsSystem.h"


/*global counter for savefilename index*/
int saveIndex;

/*create all the defines for the menu colors so it is easier to set them*/
#define FLASH_BLUE (FORGROUND_BLUE | FOREGROUND_INTENSITY)

#define FLASH_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#define FLASH_GREEN (FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#define FLASH_TEAL (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)

#define MAX_SAVE_LENGTH 40
#define MAX_NAME_LENGTH 20

/*buffer for the desired name of a save file*/
char saveFileName [MAX_SAVE_LENGTH];

/*create an enum that will store what option the user is currently on* 
 *in the main menu                                                    */
enum E_MENU_POSITION {NEW_GAME = 1,LOAD_GAME, EXIT_GAME};

enum E_CURSOR_DIRECTION {MOVE_CURSOR_NULL = 0, MOVE_CURSOR_UP, MOVE_CURSOR_DOWN};

typedef enum E_MENU_POSITION MENU_POSITION;
typedef enum E_CURSOR_DIRECTION CURSOR_DIRECTION;

/*struct for storing cursor information*/
typedef struct
{
  MENU_POSITION selection;
  int position_x;
  int position_y;
  char cursor;
  CURSOR_DIRECTION direction;

} CURSOR;

CURSOR menu_cursor;

void InitializeCursor();
void MoveCursor(CURSOR_DIRECTION dir);
void SetCursorInBuff(CHAR_INFO cha[][CONSOLE_WIDTH]);

#endif
