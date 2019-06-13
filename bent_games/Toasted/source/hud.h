/*! ************************************************************************
  \file hud.h
  \author Michael Lobato
  \par email: mlobato@digipen.edu
  \par Game: Toasted
  \date January 29th, 2007
  \brief  the header file for the hud.c file
***************************************************************************/
#ifndef HEADS_UP_DISPLAY_H
#define HEADS_UP_DISPLAY_H

/*define the area the hud will take up along the x axis*/
#define HUDHEIGHT 6

/*define for the colors of the hud*/
#define EQUIPED_COLOR (FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

#define HUD_BACKGROUND_COLOR (BACKGROUND_BLUE | BACKGROUND_INTENSITY)

#define HUD_BACKGROUND_FLASH_COLOR (BACKGROUND_RED)

#define HUD_BORDER_COLOR (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

#define UN_EQUIPED_COLOR (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

/*Struct for hud info, mainly for flashing*/
typedef struct
{

  int startFlash;
  int previousBlinkCount;
  double beginFlashTime;
  double beginNormalTime;
  int isTextFlash;
  int isHealthFlash;
  int blinkCount;

}HUD_STATS;

/*global for hud stats*/
HUD_STATS hud_info;

void WriteHUD(CHAR_INFO cha[][CONSOLE_WIDTH]);
void FlashHUD();
void HUDInitializer();


/* text prompt - Sheridan */
int textPromptX;
int textPromptY;
char textPrompt[500];
void createTextPrompt();
void setTextPrompt(char *text);
void renderTextPrompt(CHAR_INFO cha[][CONSOLE_WIDTH]);


#endif 
