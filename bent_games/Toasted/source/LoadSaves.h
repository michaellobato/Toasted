/***************************************************************************
  Author: Michael Lobato
  Game:	  Toasted
  File:	  LoadSaves.h
  Date:	  Feb. 22, 2007

  Description:
          Nothing special here, just all of the defines and globals for 
          LoadSaves.c. The MAX NUMBER OF SAVES is set here along with the
          struct for the storing the saves.
***************************************************************************/
#ifndef LOAD_SAVES
#define LOAD_SAVES

#include <windows.h>
#include "MenuSystem.h"
#include "player.h"
#include <stdio.h>
#include <string.h>

/*define for the total number of saves we will allow the users to have*/
#define MAX_SAVES 5 

/*struct for the saved files found*/
typedef struct
{
  char file_array [MAX_SAVES][MAX_SAVE_LENGTH];
  int selected_save;
  struct S_PLAYER plyrarray [MAX_SAVES];

} SAVEDFILES;

SAVEDFILES saved_files;

/*function headers*/
void FindAllSaves();
void AddNewSave();

#endif
