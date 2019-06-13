/***************************************************************************
  Author: Michael Lobato
  Game:	  Toasted
  File:	  MenuSystem.c
  Date:	  Feb. 10, 2007

  Description:
          This file contains the functions for the main menu. 
***************************************************************************/

#include <windows.h>
#include "MenuSystem.h"
#include "Sound.h"
#include "timer.h"

/**************************************************
////Mike/////
function    InitializeCursor
Discription simply initialize the cursor struct
input       nothing
return      void
***************************************************/
void InitializeCursor()
{
  /*initialize all the menu cursor stuff*/
	menu_cursor.selection = NEW_GAME;
	menu_cursor.position_x = 32;
	menu_cursor.position_y = 29;
	menu_cursor.cursor = '>';
}

/**************************************************
////Mike/////
function    MoveCursor
Discription move the cursor to the correct spot
input       dir - the direction the cursor will be moving
return      void
***************************************************/
void MoveCursor(CURSOR_DIRECTION dir)
{
	/* play a sound */
	playSoundCopy(&soundMenuChange, 100);

  menu_cursor.direction = dir;
  if (dir == MOVE_CURSOR_DOWN)
  {
    menu_cursor.position_y++;
    menu_cursor.selection++;
  }
  if (dir == MOVE_CURSOR_UP)
  {
    menu_cursor.position_y--;
    menu_cursor.selection--;
  }
}

/**************************************************
////Mike/////
function    SetCursorInBuff
Discription changes where the cursor is in the screen buffer
input       cha - the buffer we use for the screen
return      void
***************************************************/
void SetCursorInBuff(CHAR_INFO cha[][CONSOLE_WIDTH])
{
  /*if the cursor has not been moved*/
  if (menu_cursor.direction == MOVE_CURSOR_NULL)
  {
    cha[menu_cursor.position_y][menu_cursor.position_x].Char.AsciiChar = menu_cursor.cursor;
    return;
  }
  /*if the cursor has been moved down*/
  if (menu_cursor.direction == MOVE_CURSOR_DOWN)
  {
    cha[menu_cursor.position_y - 1][menu_cursor.position_x].Char.AsciiChar = ' ';
    cha[menu_cursor.position_y][menu_cursor.position_x].Char.AsciiChar = menu_cursor.cursor;
  }
  /*if the cursor has been moved up*/
  if (menu_cursor.direction == MOVE_CURSOR_UP)
  {
    cha[menu_cursor.position_y + 1][menu_cursor.position_x].Char.AsciiChar = ' ';
    cha[menu_cursor.position_y][menu_cursor.position_x].Char.AsciiChar = menu_cursor.cursor;
  }
  /*set the cursor back to not being moved*/
  menu_cursor.direction = MOVE_CURSOR_NULL;
}
