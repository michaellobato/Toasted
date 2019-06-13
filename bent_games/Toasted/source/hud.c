/***************************************************************************
  Author: Michael Lobato
  Game:	  Toasted
  File:	  hud.c
  Date:	  January 30, 2007

  Description:
          This file contains the function that controls the heads up display
		  for the game. This hud displays the players current health, 
		  weapons as well as the currently equiped weapon, and a text box 
		  that gives the player updates during the game.
***************************************************************************/
#include <windows.h>
#include "LevelManager.h"
#include "player.h"
#include "GraphicsSystem.h"
#include <stdio.h>
#include "hud.h"
#include "WeaponManager.h"
#include "timer.h"
#include "Sound.h"


/*function that writes the hud to the screen*/
void WriteHUD(CHAR_INFO cha[][CONSOLE_WIDTH])
{
	/*Initialize counters for the loop that controls what area of the screen*
	 *I am writing to                                                       */
	int i;
	int j;
  int current_weapon;

  current_weapon = player.currentWeapon;
	
  /*call the function that flashes the hud if necesary*/
  FlashHUD();

	/*loups through the rows of the 2d array*/
	for(i = 0; i < HUDHEIGHT; i++)
	{
		/*loups through the columns of the 2d array*/
		for(j = 0; j < CONSOLE_WIDTH; j++)
		{
		  /*Colors the intire hud blue*/
		  if (i < HUDHEIGHT)
			{
				cha[i][j].Attributes = (HUD_BACKGROUND_COLOR);
        cha[i][j].Char.AsciiChar = ' ';
			}

      /*draw the top */
      if ( i == 0)
      {
        cha[i][j].Char.AsciiChar = (CHAR)205;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
      /*draw the bottom border*/
      if (i == HUDHEIGHT - 1)
      {
        cha[i][j].Char.AsciiChar = (CHAR)205;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
      /*draw the left side of the border*/
      if ((i == 0) && (j == 0))
      {
        cha[i][j].Char.AsciiChar = (CHAR)201;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
      /*draw the bottom left corner of the border*/
      if ((i == HUDHEIGHT - 1) && (j == 0))
      {
        cha[i][j].Char.AsciiChar = (CHAR)200;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
      /*draw the top right of the border*/
      if ((i == 0) && (j == CONSOLE_WIDTH - 1))
      {
        cha[i][j].Char.AsciiChar = (CHAR)187;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
      /*draw the bottom right of the border*/
      if ((i == HUDHEIGHT - 1) && (j == CONSOLE_WIDTH - 1))
      {
        cha[i][j].Char.AsciiChar = (CHAR)188;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
      /*draw the left and right side of the border*/
      if ((i > 0) && (i < HUDHEIGHT - 1) && ((j == 0) || (j == CONSOLE_WIDTH - 1)))
      {
        cha[i][j].Char.AsciiChar = (CHAR)186;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
      /*draw the top of the border that seperates the text area*/
      if ((i == 0) && (j == 35) )
      {
        cha[i][j].Char.AsciiChar = (CHAR)203;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
      /*draw the bottom of the border that seperates the text area*/
      if ((i == HUDHEIGHT - 1) && (j == 35) )
      {
        cha[i][j].Char.AsciiChar = (CHAR)202;
        cha[i][j].Attributes = HUD_BORDER_COLOR;
      }
			/*When the loup is in row 2 it uses the following if's to draw the health*/
			if (i == 1)		
			{
				/*10% health*/
				if ( ((j >= 2) && (j <= 3)) && ((player.hp) > 0) )
				{
					cha[i][j].Attributes = (BACKGROUND_RED);
				}
	
				/*20% health*/
				if ( ((j >= 2) && (j <= 5)) && ((player.hp) > 10) )
				{
					cha[i][j].Attributes = (BACKGROUND_RED);
				}

				/*30% health*/
				if ( ((j >= 2) && (j <= 7)) && ((player.hp) > 20) )
				{
					cha[i][j].Attributes = (BACKGROUND_RED) | 
										   (BACKGROUND_INTENSITY);
				}

				/*40% health*/
				if ( ((j >= 2) && (j <= 9)) && ((player.hp) > 30) )
				{
					cha[i][j].Attributes = (BACKGROUND_RED) | 
										   (BACKGROUND_INTENSITY);
				}

				/*50% health*/
				if ( ((j >= 2) && (j <= 11)) && ((player.hp) > 40) )
				{
					cha[i][j].Attributes = (BACKGROUND_RED) | 
										   (BACKGROUND_GREEN) | 
										   (BACKGROUND_INTENSITY);
				}

				/*60% health*/
				if ( ((j >= 2) && (j <= 13)) && ((player.hp) > 50) )
				{
					cha[i][j].Attributes = (BACKGROUND_RED) | 
										   (BACKGROUND_GREEN) | 
										   (BACKGROUND_INTENSITY);
				}

				/*70% health*/
				if ( ((j >= 2) && (j <= 15)) && ((player.hp) > 60) )
				{
					cha[i][j].Attributes = (BACKGROUND_GREEN | 
                                  BACKGROUND_BLUE | 
                                  BACKGROUND_INTENSITY);
				}

				/*80% health*/
				if ( ((j >= 2) && (j <= 17)) && ((player.hp) > 70) )
				{
					cha[i][j].Attributes = (BACKGROUND_GREEN | 
                                  BACKGROUND_BLUE | 
                                  BACKGROUND_INTENSITY);
				}

				/*90% health*/
				if ( ((j >= 2) && (j <= 19)) && ((player.hp) > 80) )
				{
					cha[i][j].Attributes = (BACKGROUND_GREEN) | 
										   (BACKGROUND_INTENSITY);
				}

				/*full health*/
				if ( ((j >= 2) && (j <= 21)) && ((player.hp) > 90) )
				{
					cha[i][j].Attributes = (BACKGROUND_GREEN) | 
										   (BACKGROUND_INTENSITY);
				}
			}

      /***************
       *****PISTOL****
       ***************/
			/*draw the top and bottom of the pistol equipment slots*/
			if ( ((i == 2) || (i == 4)) && ((j == 3) || (j == 4)) )
			{
				cha[i][j].Char.AsciiChar = '-';

        if ((current_weapon == GUN_PISTOL) || (current_weapon == GUN_DUALPISTOL))
				  cha[i][j].Attributes = EQUIPED_COLOR;
        else
          cha[i][j].Attributes = UN_EQUIPED_COLOR;
			}
       
      /*sets the p in the first slot for the shotgun*/
      if ((i == 3) && (j == 3))
      {
        cha[i][j].Char.AsciiChar = 'P';

        if ((current_weapon == GUN_PISTOL) || (current_weapon == GUN_DUALPISTOL))
          cha[i][j].Attributes = EQUIPED_COLOR;
        else
          cha[i][j].Attributes = UN_EQUIPED_COLOR;
      }

      /*this if displays the number of the weapon equiped*/
      if (i == 3)
      {
        if ((j == 4))
        {
          if ((current_weapon == GUN_DUALPISTOL))
          {
            cha[i][j].Char.AsciiChar = '2';
          }
          else if ((player.weaponsEnabled[GUN_PISTOL]))
          {
            cha[i][j].Char.AsciiChar = '1';
          }

          if ((current_weapon == GUN_PISTOL) || (current_weapon == GUN_DUALPISTOL))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }

        /*draw the sides of the pistol equipment slots*/
			  if ((j == 2) || (j == 5) )
			  {
				  cha[i][j].Char.AsciiChar = '|';
          if ((current_weapon == GUN_PISTOL) || (current_weapon == GUN_DUALPISTOL))
          {
            cha[i][j].Attributes = EQUIPED_COLOR;
          }
          else
				    cha[i][j].Attributes = UN_EQUIPED_COLOR;
			  }
      }

      /***************
       ****SHOTGUN****
       ***************/
      /*draw the top and bottom of the shotgun equipment slots*/
			if ( ((i == 2) || (i == 4)) && ((j == 7) || (j == 8)) )
			{
				cha[i][j].Char.AsciiChar = '-';

        if ((current_weapon == GUN_SPREADGUN) || (current_weapon == GUN_SCATTERGUN))
				  cha[i][j].Attributes = EQUIPED_COLOR;
        else
          cha[i][j].Attributes = UN_EQUIPED_COLOR;
			}
       
      /*draw the sides of the shotgun equipment slots*/
      if (i == 3)
      {
			  if ( (j == 6) || (j == 9) )
			  {
				  cha[i][j].Char.AsciiChar = '|';
          if ((current_weapon == GUN_SPREADGUN) || (current_weapon == GUN_SCATTERGUN))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
				    cha[i][j].Attributes = UN_EQUIPED_COLOR;
			  }

        /*sets the s in the first slot for the shotgun*/
        if ((j == 7))
        {
          cha[i][j].Char.AsciiChar = 'S';

          if ((current_weapon == GUN_SPREADGUN) || (current_weapon == GUN_SCATTERGUN))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }

        /*this if displays the number of the weapon equiped*/
        if ((j == 8))
        {
          if ((current_weapon == GUN_SCATTERGUN))
            {
              cha[i][j].Char.AsciiChar = '2';
            }
            else if ((player.weaponsEnabled[GUN_SPREADGUN]))
            {
              cha[i][j].Char.AsciiChar = '1';
            }

          if ((current_weapon == GUN_SPREADGUN) || (current_weapon == GUN_SCATTERGUN))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }
      }

      /***************
       ***AUTOMATICS**
       ***************/
      /*draw the top and bottom of the shotgun equipment slots*/
			if ( ((i == 2) || (i == 4)) && ((j == 11) || (j == 12)) )
			{
				cha[i][j].Char.AsciiChar = '-';

        if ((current_weapon == GUN_SUBMACHINEGUN) || (current_weapon == GUN_HEAVYMACHINEGUN))
				  cha[i][j].Attributes = EQUIPED_COLOR;
        else
          cha[i][j].Attributes = UN_EQUIPED_COLOR;
			}
       
      /*draw the sides of the shotgun equipment slots*/
      if (i == 3)
      {
			  if ( (j == 10) || (j == 13) )
			  {
				  cha[i][j].Char.AsciiChar = '|';
          if ((current_weapon == GUN_SUBMACHINEGUN) || (current_weapon == GUN_HEAVYMACHINEGUN))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
				    cha[i][j].Attributes = UN_EQUIPED_COLOR;
			  }

        /*sets the R in the first slot for the shotgun*/
        if ((j == 11))
        {
          cha[i][j].Char.AsciiChar = 'A';

          if ((current_weapon == GUN_SUBMACHINEGUN) || (current_weapon == GUN_HEAVYMACHINEGUN))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }

        /*this if displays the number of the weapon equiped*/
        if ((j == 12))
        {
          if ((current_weapon == GUN_HEAVYMACHINEGUN))
            {
              cha[i][j].Char.AsciiChar = '2';
            }
            else if ((player.weaponsEnabled[GUN_SUBMACHINEGUN]))
            {
              cha[i][j].Char.AsciiChar = '1';
            }

          if ((current_weapon == GUN_SUBMACHINEGUN) || (current_weapon == GUN_HEAVYMACHINEGUN))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }
      }

      /***************
       *****RIFLES****
       ***************/
      /*draw the top and bottom of the shotgun equipment slots*/
			if ( ((i == 2) || (i == 4)) && ((j == 15) || (j == 16)) )
			{
				cha[i][j].Char.AsciiChar = '-';

        if ((current_weapon == GUN_RIFLE) || (current_weapon == GUN_SNIPER))
				  cha[i][j].Attributes = EQUIPED_COLOR;
        else
          cha[i][j].Attributes = UN_EQUIPED_COLOR;
			}
       
      /*draw the sides of the shotgun equipment slots*/
      if (i == 3)
      {
			  if ( (j == 14) || (j == 17) )
			  {
				  cha[i][j].Char.AsciiChar = '|';
          if ((current_weapon == GUN_RIFLE) || (current_weapon == GUN_SNIPER))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
				    cha[i][j].Attributes = UN_EQUIPED_COLOR;
			  }

        /*sets the R in the first slot for the shotgun*/
        if ((j == 15))
        {
          cha[i][j].Char.AsciiChar = 'R';

          if ((current_weapon == GUN_RIFLE) || (current_weapon == GUN_SNIPER))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }

        /*this if displays the number of the weapon equiped*/
        if ((j == 16))
        {
          if ((current_weapon == GUN_SNIPER))
            {
              cha[i][j].Char.AsciiChar = '2';
            }
            else if ((player.weaponsEnabled[GUN_RIFLE]))
            {
              cha[i][j].Char.AsciiChar = '1';
            }

          if ((current_weapon == GUN_RIFLE) || (current_weapon == GUN_SNIPER))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }
      }
      

      /***************
       *****UBERS*****
       ***************/
      /*draw the top and bottom of the shotgun equipment slots*/
			if ( ((i == 2) || (i == 4)) && ((j == 19) || (j == 20)) )
			{
				cha[i][j].Char.AsciiChar = '-';

        if ((current_weapon == GUN_ROCKETLAUNCHER) || (current_weapon == GUN_OMEGALASER))
				  cha[i][j].Attributes = EQUIPED_COLOR;
        else
          cha[i][j].Attributes = UN_EQUIPED_COLOR;
			}
       
      /*draw the sides of the shotgun equipment slots*/
      if (i == 3)
      {
			  if ( (j == 18) || (j == 21) )
			  {
				  cha[i][j].Char.AsciiChar = '|';
          if ((current_weapon == GUN_ROCKETLAUNCHER) || (current_weapon == GUN_OMEGALASER))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
				    cha[i][j].Attributes = UN_EQUIPED_COLOR;
			  }

        /*sets the R in the first slot for the shotgun*/
        if ((j == 19))
        {
          cha[i][j].Char.AsciiChar = 'U';

          if ((current_weapon == GUN_ROCKETLAUNCHER) || (current_weapon == GUN_OMEGALASER))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }

        /*this if displays the number of the weapon equiped*/
        if ((j == 20))
        {
          if ((current_weapon == GUN_OMEGALASER))
          {
            cha[i][j].Char.AsciiChar = '2';
          }
          else if ((player.weaponsEnabled[GUN_ROCKETLAUNCHER]))
          {
            cha[i][j].Char.AsciiChar = '1';
          }

          if ((current_weapon == GUN_ROCKETLAUNCHER) || (current_weapon == GUN_OMEGALASER))
            cha[i][j].Attributes = EQUIPED_COLOR;
          else
            cha[i][j].Attributes = UN_EQUIPED_COLOR;
        }
      }

			/*outline the area where the text updates will appear*/
			if ((i != 0) && (i != HUDHEIGHT - 1) &&(j == 35)) 
			{
				cha[i][j].Char.AsciiChar = (CHAR)186;
				cha[i][j].Attributes = FOREGROUND_RED | 
                               FOREGROUND_GREEN |
                               FOREGROUND_BLUE | 
                               FOREGROUND_INTENSITY |
                               BACKGROUND_BLUE |
                               BACKGROUND_INTENSITY;
			}

      /*if the flashing bool is set to flash and the loop is in the *
       *correct spots to flash                                      */
      if (hud_info.isTextFlash && ((((i == 0) || (i == HUDHEIGHT - 1)) && (j >= 35)) || (j == 35) || (j == CONSOLE_WIDTH - 1) ) )
      {
        /*set this location to the flashing color*/
        cha[i][j].Attributes = HUD_BACKGROUND_FLASH_COLOR;
      }
      
    }

		/*Reset the column counter before the row counter is incrimented*/
		j = 0;
	}

	renderTextPrompt(cha);
	return;
}


void createTextPrompt()
{
	int i;

	/* initialize the text to be blank */
	for(i = 0; i < 500; i++)
	{
		textPrompt[i] = 0;
	}

	/* set the position */
	textPromptX = 37;
	textPromptY = 1;
}

void setTextPrompt(char *text)
{
	/* copy the text into the text prompt array */
	strcpy(&textPrompt[0], text);
	hud_info.startFlash = 1;
	playSoundCopy(&soundMenuChange, 100);
}

void renderTextPrompt(CHAR_INFO cha[][CONSOLE_WIDTH])
{
	/* loop through and set the positions of cha */
	int y;
	int x;
	int textCounter = 0;
	int textLength = strlen(&textPrompt[0]);

	/* loop through each row */
	for(y = 0; y < 3; y++)
	{
		/* loop through each collum */
		for(x = 0; x < 42; x++)
		{
			if(textCounter < textLength)
			{
				/* if its a newline, move to the next y */
				if(textPrompt[textCounter] == '\n')
				{
					y++;
					
					/* set the x back to 0 */
					x = -1;
				}
				else
				{
					cha[y + textPromptY][x + textPromptX].Char.AsciiChar = textPrompt[textCounter];
					cha[y + textPromptY][x + textPromptX].Attributes = (BACKGROUND_BLUE | BACKGROUND_INTENSITY |
						FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				}
				textCounter++;
			}
		}
	}
}

/**************************************************
////Mike/////
function    HUDInitializer

Discription simply initialize the hud struct

input       nothing

return      void
***************************************************/
void HUDInitializer()
{
  hud_info.beginNormalTime = 0;
  hud_info.previousBlinkCount = 0;
  hud_info.startFlash = 0;
  hud_info.beginFlashTime = 0;
  hud_info.isTextFlash = 0;
  hud_info.isHealthFlash = 0;
  hud_info.blinkCount = 0;
}

/**************************************************
////Mike/////
function    FlashHUD

Discription processes all of the data to flash the hud

input       nothing

return      void
***************************************************/
void FlashHUD()
{
  /*timer for the current time*/
  double current = getCurrentTime();

  /*if start flash is true, someone wants the hud to start flashing*/
  if (hud_info.startFlash)
  {
    /*set the time when the flashing started*/
    hud_info.beginFlashTime = current;
    /*change the start flash variable back to false*/
    hud_info.startFlash = 0;
    /*set the bool for making the actual flash*/
    hud_info.isTextFlash = 1;
    /*initialize the blinking count to 0*/
    hud_info.blinkCount = 0;
  }

  /*if the hud has blinked the max number of times*/
  if (hud_info.blinkCount >= 3)
  {
    hud_info.isTextFlash = 0; /*change flashing bool back to false*/
  }
  else
  {
    /*if we are set to flash and the flash timer is not up*/
    if ((hud_info.isTextFlash == 1) && ((hud_info.beginFlashTime + 200) < current) )
    {
      hud_info.beginNormalTime = current; /*start the timer for non flashing*/
      hud_info.beginFlashTime = 0;  /*change flashing clock back to 0*/
      hud_info.blinkCount++;  /*increase the blinking counter*/
      hud_info.isTextFlash = 0; /*change the flashing bool back to false*/
    }
    /*if we are set to not flash and the not flashing timer is not up*/
    else if ((hud_info.isTextFlash == 0) && (hud_info.beginNormalTime + 200 < current))
    {
      hud_info.beginFlashTime = current;  /*start the timer for flashing*/
      hud_info.beginNormalTime = 0; /*change not flashing clock back to 0*/
      hud_info.isTextFlash = 1; /*change the flashing bool to true*/
    }
  }
 }
