/*! ************************************************************************
  \file LevelManager.h
  \author Sheridan Beebe
  \par email: sbeebe@digipen.edu
  \par Game: Toasted
  \date January 16th, 2007
  \brief

	This is the header file for LevelManager.c. It is to include
	all prototypes and type defines, enums, structures, etc. This
	file should be included by any other file that needs to access
	the level management functions.
    
***************************************************************************/

#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

/* includes, malloc */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

/*! \brief This defines the longest line to read - makes the max x for the level 1000. */
#define LONGEST_LINE 1001

/*! \brief This is the number of total layers to create when loading levels. */
#define LAYER_COUNT 5
/* These are the layers to be used by certain items */
/*! \brief This is the layer number to be used by the player. */
#define PLAYER_LAYER 1
/*! \brief This is the layer number to be used by the walls. */
#define WALL_LAYER 0
/*! \brief This is the layer number to be used by the weapons. */
#define WEAPON_LAYER 2
/*! \brief This is the layer number to be used by the enemies. */
#define ENEMY_LAYER 1
/*! \brief This is the layer number to be used by the health packs. */
#define HEALTH_LAYER 3
/*! \brief This is the layer number to be used by the switches. */
#define SWITCH_LAYER 3

#define PICKUP_LAYER 3

#define UNDER_LAYER 4

/* Enums for the level manager */

/*! 
   \brief This enum is used to identify what type of object is contained in a LEVEL_POSITION.
   It is type defed as POSITION_TYPE. 
*/
enum E_POSITION_TYPE {POSITION_INVALID = -1, /*!< Defines this position as an invalid space. */
						POSITION_SPACE = 1, /*!< Defines this position as an empty space. */
						POSITION_ENEMY, /*!< Defines this position as an enemy. */
						POSITION_SPAWN, /*!< Defines this position as a spawn. */
						POSITION_PLAYER, /*!< Defines this position as the player. */
						POSITION_WALL, /*!< Defines this position as a wall. */
						POSITION_HEALTH, /*!< Defines this position as a health pack. */
						POSITION_SWITCH, /*!< Defines this position as a switch. */
						POSITION_WEAPON, /*!< Defines this position as a weapon. */
						POSITION_UNDER, /*!< Defines this position as under everything else. */
						POSITION_SUPER_TANK /*!< Defines this position as a super tank position. */
						};

/*! \brief This type is used for storing what type of object is currently in a specific position */
typedef enum E_POSITION_TYPE POSITION_TYPE;

/* structs to be used by the level manager */

/*!
   \brief The level position type is a struct used to store information on each position
   of a map. 
   
   For instance, if there is a wall in a given location, that position
   will have a pointer to a LEVEL_POSITION struct for all walls. The same goes for
   spaces, enemies, weapons, player, what ever. 
*/
typedef struct
{
	/*! \brief The data pointer is a pointer to the data that is in this spot. If there is
	   enemy at this location, it points to the particular enemy struct that is here. */
	void *data;

	/*! \brief Store what type of object is at this position */
	POSITION_TYPE type;

	/*! \brief Character that should be displayed in this position */
	char character;

} LEVEL_POSITION;


/*! 
   \brief This struct will be the main struct to hold a level. It will contain
   everything that partains to a certain level. This way we can ensure to keep
   everything together. Its typedef'ed as a LEVEL type. 
*/
typedef struct 
{
	/*! 
	   \brief This character array will store the file name of this particular map.

	   The file name should be relative to the application exe and less than 20
	   chatacters. 
	*/
	char fileName[20];

	/*! \brief This is a pointer to the very first level position in our map, ie. (0,0),
	   the top left position. 
	   
	   It is used to calcuate and store all level positions on all layers. 
	   The level positions will be layed out like so:

	   layer 1 starting with 0, 0 to n,n<br>
	   layer 2 starting with 0, 0 to n,n<br>
	   layer n starting with 0, 0 to n,n.<br>
	   This will allow for an expandable layering system if
	   the need should arise. */
	LEVEL_POSITION **firstPosition;

	/*! \brief This holds the width of the level */
	int width;

	/*! \brief This holds the height of the level */
	int height;

} LEVEL;

/* Function prototypes for the Level Manager 
   All the comments explaining the functions are in the source file. */

LEVEL *loadLevel(char *fileName, char *underFileName, int levelNumber);

void unloadLevel(LEVEL *level);

LEVEL_POSITION *getLevelPosition(int layerNumber, int x, int y);

void setLevelPosition(int layerNumber, int x, int y, LEVEL_POSITION *newLevelPosition);

int isLevelPositionCollision(LEVEL * level, int layerNumber, int x, int y);

int isLevelPositionSpace(int layerNumber, int x, int y);

int isLevelPositionWall(int layerNumber, int x, int y);

void clearLevelPositions(int layerNumber, char clearChar);

void switchCharacterLayers(int startingLayer, int endingLayer, char switchChar);

LEVEL *getCurrentLevel();

void loadLevelManager();

void unloadLevelManager();

LEVEL_POSITION *createLevelPosition(void *data, POSITION_TYPE type, char character);

void setCurrentLevel(LEVEL *level);

int getPositionIndex(int layerNumber, int x, int y);

void fillspace(int x, int y);

void loadchars(char *fileName, char *colorName,  CHAR_INFO cha[50][80]);

int isLevelPositionSpaceAll(int x, int y);

/* Global level variables */

/*! \brief A pointer to the current LEVEL in use by the game. */
LEVEL *currentLevel;

/*! \brief A pointer to a LEVEL_POSITION struct that will be used for all
	whitespace. */
LEVEL_POSITION *whiteSpacePosition;

/*! \brief A pointer to a LEVEL_POSITION struct that will be used for all
	black space. */
LEVEL_POSITION *blackSpacePosition;

/*! \brief A pointer to a LEVEL_POSITION struct that will be used for all
	walls. */
LEVEL_POSITION *wallPosition;

LEVEL_POSITION *invalidPosition;

LEVEL_POSITION *walkWayPositionOne;

LEVEL_POSITION *shadowPosition;



/* these are color storage for all the reused under positions */
WORD greenColorData;
WORD greenIntenseColorData;
WORD shadowColorData;
WORD brownIntenseColorData;

/*
	A pointer to a LEVEL_POSITION struct that is used to represent the player
	**edit Chris
*/
LEVEL_POSITION *playerPosition;

#endif
