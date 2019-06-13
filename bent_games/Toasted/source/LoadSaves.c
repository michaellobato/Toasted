/***************************************************************************
  Author: Michael Lobato
  Game:	  Toasted
  File:	  LoadSaves.c
  Date:	  Feb. 22, 2007

  Description:
          This file contains the functions for the LoadSaves.h.

          FindAllSaves is called before the user goes to the menus
              so all of the saved game text files are found.
***************************************************************************/

#include "LoadSaves.h"

/***********************
 -takes no peramiters and returns no peramiters

 -reads the text file where all the names of saved games are and puts 
 -all of the information in the saved_files struct

 ***********************/
void FindAllSaves()
{
  int i;
  
  /*copy all of the saved file names into the save file array*/
  strcpy (&saved_files.file_array[0][0], "saves/SaveFile1.txt");
  strcpy (&saved_files.file_array[1][0], "saves/SaveFile2.txt");
  strcpy (&saved_files.file_array[2][0], "saves/SaveFile3.txt");
  strcpy (&saved_files.file_array[3][0], "saves/SaveFile4.txt");
  strcpy (&saved_files.file_array[4][0], "saves/SaveFile5.txt");

  for (i = 0; i < MAX_SAVES; i++)
  {
    loadPlayerData(saved_files.file_array[i], &saved_files.plyrarray[i]);
  }
}
/*function to save the newly created save file in the SavedNames.txt file*/
void AddNewSave()
{
}
