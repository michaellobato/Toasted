/*! ************************************************************************
  \file overworld.c
  \author Sheridan Beebe
  \par email: sbeebe@digipen.edu
  \par Game: Toasted
  \date Feburuary 6th, 2007
  \brief  does all the actions in the overworld
***************************************************************************/
#include "Overworld.h"
#include "player.h"
#include "LevelManager.h"

void startupOverworld()
{
	/* remove the walls depending on what levels are completed */
	if(player.levelOneComplete)
	{
		/* remove the wall to the second level */
		clearLevelPositions(WALL_LAYER, 'R');
	}
	if(player.levelTwoComplete)
	{
		/* remove the wall to the third level */
		clearLevelPositions(WALL_LAYER, 'V');
	}
	if(player.levelThreeComplete)
	{
		/* remove the wall to the fourth level */
		clearLevelPositions(WALL_LAYER, 'S');
	}
	if(player.levelFourComplete)
	{
		/* remove the wall to the fifth level */
		clearLevelPositions(WALL_LAYER, 'U');
	}
}

void updateOverworld()
{

}
