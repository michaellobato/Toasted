/*! ************************************************************************
  \file GraphicsSystem.h
  \author Sheridan Beebe, Chris Howard
  \par email: sbeebe@digipen.edu, choward@digipen.edu
  \par Game: Toasted
  \date January 16th, 2007
  \brief

	
    
***************************************************************************/

#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "LevelManager.h"

#define CONSOLE_HEIGHT 50
#define CONSOLE_WIDTH 80
#define LEFT_KEY_CODE 0x41
#define RIGHT_KEY_CODE 0x44
#define UP_KEY_CODE 0x57
#define DOWN_KEY_CODE 0x53

enum ENUM_STATE {
	STATE_END_GAME=0,
  STATE_COPYRIGHT_MENU,
  STATE_CREDITS_MENU,
	STATE_MAIN_MENU,
	STATE_LOAD_MENU,
	STATE_NEW_MENU,
	STATE_OPTIONS_MENU,
	STATE_SAVE_MENU,
	STATE_GAME_OVER,
	STATE_STORY,
  STATE_VICTORY,
	STATE_OVER_WORLD=50,
	STATE_LEVEL_ONE,
	STATE_LEVEL_TWO,
	STATE_LEVEL_THREE,
	STATE_LEVEL_FOUR,
	STATE_LEVEL_FIVE
};



struct keyboard
{
    int up;
    int down;
    int left;
    int right;

    int Sup;
    int Sright;
    int Sleft;
    int Sdown;
	int Gernade;
}keyboard;

typedef enum ENUM_STATE STATE;

/* function prototypes */

int plyr_movement(INPUT_RECORD InRec, int state);
/*Mike:*/
int mainMenuControls(INPUT_RECORD InRec, int state);
/*Mike:*/
int gameOverControls(INPUT_RECORD InRec, int state);
/*Mike:*/
int NewGameControls(INPUT_RECORD InRec, int state);

int processKeys(HANDLE hOut, int state);

int translate(HANDLE hOut, LEVEL *lvl, COORD *viewCoord);

int paint(HANDLE hOut, CHAR_INFO cha[CONSOLE_HEIGHT][CONSOLE_WIDTH]);

char * loadFileToString(char *fileName);

/* pause stuff */
char pauseText[1000];

void setPauseText(char *newText);

void drawPauseScreen(CHAR_INFO cha[][CONSOLE_WIDTH]);

#endif
