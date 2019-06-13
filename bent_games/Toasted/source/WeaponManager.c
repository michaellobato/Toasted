/***************************************************************************
Author: Cullen McElroy
Game:	  Toasted
File:	  WeaponManager.c
Date:	  January 27th, 2007
(modifyed through: February 2nd, 2007)
Description:
This file contains all code dealing with weapons and the ability
to shoot. The functions for this file are prototyped in
'WeaponManager.h'
***************************************************************************/

#include "WeaponManager.h"
#include "player.h"
#include "LevelManager.h"
#include <stdlib.h>
#include "timer.h"
#include "EnemyManager.h"
#include "Sound.h"
#include "effects.h"
#include <stdio.h>
#include <math.h>

/*******************************************************************************
Function:    loadWeapons

Description: This is the main function in the file. It loads all the
weapons and creates the mainBullet for the linked list.

Inputs:      None

Outputs:     Return one to indicate success.
*******************************************************************************/
int loadWeapons(void)
{
	/* setting each element of the weapon array to
	its respected gun */
	weaponArray[GUN_PISTOL] = &weaponPistol;
	weaponArray[GUN_DUALPISTOL] = &weaponDualPistol;
	weaponArray[GUN_SPREADGUN] = &weaponSpreadGun;
	weaponArray[GUN_SCATTERGUN] = &weaponScatterGun;
	weaponArray[GUN_SUBMACHINEGUN] = &weaponSubMachineGun;
	weaponArray[GUN_HEAVYMACHINEGUN] = &weaponHeavyMachineGun;
	weaponArray[GUN_RIFLE] = &weaponRifle;
	weaponArray[GUN_SNIPER] = &weaponSniper;
	weaponArray[GUN_ROCKETLAUNCHER] = &weaponRocketLauncher;
	weaponArray[GUN_OMEGALASER] = &weaponOmegaLaser;

	/* setting the necessary pistol attributes */
	weaponPistol.range = 1000;
	weaponPistol.damage = 10;
	weaponPistol.graphic = '*';
	weaponPistol.lastFired = 0;
	weaponPistol.reloadTime = 300;
	weaponPistol.owner = OWNER_PLAYER;
	weaponPistol.ID = GUN_PISTOL;

	/* setting the necessary dual pistol attributes */
	weaponDualPistol.range = 1000;
	weaponDualPistol.damage = 10;
	weaponDualPistol.graphic = '*';
	weaponDualPistol.lastFired = 0;
	weaponDualPistol.reloadTime = 300;
	weaponDualPistol.owner = OWNER_PLAYER;
	weaponDualPistol.ID = GUN_DUALPISTOL;

	/* setting the necessary Spread gun attributes */
	weaponSpreadGun.range = 350;
	weaponSpreadGun.damage = 20;
	weaponSpreadGun.graphic = '*';
	weaponSpreadGun.lastFired = 0;
	weaponSpreadGun.reloadTime = 600;
	weaponSpreadGun.owner = OWNER_PLAYER;
	weaponSpreadGun.ID = GUN_SPREADGUN;

	/* setting the necessary Scatter Gun attributes */
	weaponScatterGun.range = 450;
	weaponScatterGun.damage = 25;
	weaponScatterGun.graphic = '*';
	weaponScatterGun.lastFired = 0;
	weaponScatterGun.reloadTime = 750;
	weaponScatterGun.owner = OWNER_PLAYER;
	weaponScatterGun.ID = GUN_SCATTERGUN;

	/* setting the necessary Sub Machine Gun attributes */
	weaponSubMachineGun.range = 1000;
	weaponSubMachineGun.damage = 15;
	weaponSubMachineGun.graphic = '*';
	weaponSubMachineGun.lastFired = 0;
	weaponSubMachineGun.reloadTime = 100;
	weaponSubMachineGun.owner = OWNER_PLAYER;
	weaponSubMachineGun.ID = GUN_SUBMACHINEGUN;

	/* setting the necessary pistol attributes */
	weaponHeavyMachineGun.range = 1500;
	weaponHeavyMachineGun.damage = 20;
	weaponHeavyMachineGun.graphic = '*';
	weaponHeavyMachineGun.lastFired = 0;
	weaponHeavyMachineGun.reloadTime = 150;
	weaponHeavyMachineGun.owner = OWNER_PLAYER;
	weaponHeavyMachineGun.ID = GUN_HEAVYMACHINEGUN;

	/* setting the necessary rifle attributes */
	weaponRifle.range = 2000;
	weaponRifle.damage = 50;
	weaponRifle.graphic = '*';
	weaponRifle.lastFired = 0;
	weaponRifle.reloadTime = 600;
	weaponRifle.owner = OWNER_PLAYER;
	weaponRifle.ID = GUN_RIFLE;

	/* setting the necessary sniper attributes */
	weaponSniper.range = 4000;
	weaponSniper.damage = 70;
	weaponSniper.graphic = '*';
	weaponSniper.lastFired = 0;
	weaponSniper.reloadTime = 800;
	weaponSniper.owner = OWNER_PLAYER;
	weaponSniper.ID = GUN_SNIPER;

	/* setting the necessary rocket launcher attributes */
	weaponRocketLauncher.range = 1500;
	weaponRocketLauncher.damage = 100;
	weaponRocketLauncher.graphic = '*';
	weaponRocketLauncher.lastFired = 0;
	weaponRocketLauncher.reloadTime = 200;
	weaponRocketLauncher.owner = OWNER_PLAYER;
	weaponRocketLauncher.ID = GUN_ROCKETLAUNCHER;

	/* setting the necessary omega laser attributes */
	weaponOmegaLaser.range = 4000;
	weaponOmegaLaser.damage = 100;
	weaponOmegaLaser.graphic = '*';
	weaponOmegaLaser.lastFired = 0;
	weaponOmegaLaser.reloadTime = 0;
	weaponOmegaLaser.owner = OWNER_PLAYER;
	weaponOmegaLaser.ID = GUN_OMEGALASER;

	/* setting the necessary gernade attributes */
	weaponGernade.range = 1000;
	weaponGernade.damage = 0;
	weaponGernade.graphic = '*';
	weaponGernade.lastFired = 0;
	weaponGernade.reloadTime = 1200;
	weaponGernade.owner = OWNER_PLAYER;
	weaponGernade.ID = GUN_GERNADE;


	/* creating the main bullet for the linked list */
	mainBullet = addBullet(NULL);
	return 1;
}
/*******************************************************************************
Function:    shoot

Description: This function is used to have the player shoot bullets.

Inputs:      GUN* weapon - a weapon struct
int xDirection - a value to tell which direction the bullet 
should travel (x-axis).
int yDirection - a value to tell which direction the bullet
should travel (y-axis).

Outputs:     None
*******************************************************************************/
void shoot(GUN* weapon, float xDirection, float yDirection, int xPos, int yPos)
{

	/* if the pistol is reloaded, do the following */
	if(reloadTime(weapon))
	{
		if(weapon->ID != GUN_OMEGALASER && weapon->ID != GUN_SNIPER && weapon->ID != GUN_RIFLE
			&& weapon->ID != GUN_GERNADE)
		{
			/* play the shooting sound */
			playSoundCopy(&soundBullet, 80);
		}
		else if(weapon->ID == GUN_SNIPER || weapon->ID == GUN_RIFLE)
		{
			/* play the rifle sound */
			playSoundCopy(&soundSniper, 100);
		}

		/* if the weapon eqiuped is the dual pistol */
		if(weapon->ID == GUN_DUALPISTOL)
		{
			/* creating values for position of the dual
			pistol bullets */
			float bullet1X;
			float bullet1Y;
			float bullet2X;
			float bullet2Y;

			/* creating a level position for the bullet being created */
			LEVEL_POSITION *bulletPosition1;
			LEVEL_POSITION *bulletPosition2;

			/* allocating memory for the bullet to be created */
			BULLET *bullet1 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet2 = (BULLET*)malloc(sizeof(BULLET));

			/* defining variables for the initial position of the bullets
			shot from the dual pistols
			*/
			if(xDirection == 0)
			{
				/* create a bullet on either side of the player
				with respect to y */
				bullet1X = (float)xPos + 1;
				bullet1Y = (float)yPos;

				bullet2X = (float)xPos - 1;
				bullet2Y = (float)yPos;
			}
			else
			{
				/* create a bullet on either side of the player
				with respect to x */
				bullet1X = (float)xPos;
				bullet1Y = (float)yPos + 1;

				bullet2X = (float)xPos;
				bullet2Y = (float)yPos - 1;
			}


			/* setting all the necessary attributes for the first bullet */
			bullet1->deleteFlag = 0;
			bullet1->range = weapon->range;
			bullet1->damage = weapon->damage;
			bullet1->graphic = '*';
			bullet1->X = bullet1X;
			bullet1->Y = bullet1Y;
			bullet1->speed = 30;
			bullet1->lastMoveTime = 0;
			bullet1->yDirection = yDirection;
			bullet1->xDirection = xDirection;
			bullet1->firedTime = (int)getCurrentTime();
			bullet1->owner = weapon->owner;
			bullet1->ID = weapon->ID;

			/* setting all the necessary attributes for the second bullet */
			bullet2->deleteFlag = 0;
			bullet2->range = weapon->range;
			bullet2->damage = weapon->damage;
			bullet2->graphic = '*';
			bullet2->X = bullet2X;
			bullet2->Y = bullet2Y;
			bullet2->speed = 30;
			bullet2->lastMoveTime = 0;
			bullet2->yDirection = yDirection;
			bullet2->xDirection = xDirection;
			bullet2->firedTime = (int)getCurrentTime();
			bullet2->owner = weapon->owner;
			bullet2->ID = weapon->ID;

			/* creating the bullet position and graphic */
			bulletPosition1 = createLevelPosition(bullet1, POSITION_WEAPON, '*');
			bulletPosition2 = createLevelPosition(bullet2, POSITION_WEAPON, '*');

			/* setting the position */
			bullet1->bullet_pos = bulletPosition1;
			bullet2->bullet_pos = bulletPosition2;

			/* calling the add bullet function to actually add the bullet... */

			addBullet(bullet1);
			addBullet(bullet2);

			/* set the bullets' level position */

			setLevelPosition(WEAPON_LAYER, (int)bullet1X, (int)bullet1Y, bulletPosition1);
			setLevelPosition(WEAPON_LAYER, (int)bullet2X, (int)bullet2Y, bulletPosition2);

		}
		else if(weapon->ID == GUN_SCATTERGUN)
		{
			/* declaring variables for the bullets' trajectory
			defined later*/
			float dirX1 = 0;
			float dirX2 = 0;
			float dirX3 = 0;
			float dirX4 = 0;
			float dirX5 = 0;
			float dirX6 = 0;
			float dirX7 = 0;

			float dirY1 = 0;
			float dirY2 = 0;
			float dirY3 = 0;
			float dirY4 = 0;
			float dirY5 = 0;
			float dirY6 = 0;
			float dirY7 = 0;

			/* creating a level position for the bullet being created */
			LEVEL_POSITION *bulletPosition1;
			LEVEL_POSITION *bulletPosition2;
			LEVEL_POSITION *bulletPosition3;
			LEVEL_POSITION *bulletPosition4;
			LEVEL_POSITION *bulletPosition5;
			LEVEL_POSITION *bulletPosition6;
			LEVEL_POSITION *bulletPosition7;

			/* allocating memory for the bullet to be created */
			BULLET *bullet1 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet2 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet3 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet4 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet5 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet6 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet7 = (BULLET*)malloc(sizeof(BULLET));

			/* this section will set the trajectory for each bullet
			of the scatter gun depending on which way the player is 
			facing */
			if(xDirection == 1)
			{
				dirX1 = 1;
				dirX2 = 1;
				dirX3 = 1;
				dirX4 = 1;
				dirX5 = 1;
				dirX6 = 1;
				dirX7 = 1;
				dirY6 = .05f;
				dirY7 = -.05f;
				dirY5 = .5f;
				dirY1 = .25f;
				dirY2 = 0;
				dirY3 = -.25f;
				dirY4 = -.5f;
			}
			if(xDirection == -1)
			{
				dirX1 = -1;
				dirX2 = -1;
				dirX3 = -1;
				dirX4 = -1;
				dirX5 = -1;
				dirX6 = -1;
				dirX7 = -1;
				dirY6 = .05f;
				dirY7 = -.05f;
				dirY5 = .5f;
				dirY1 = .25f;
				dirY2 = 0;
				dirY3 = -.25f;
				dirY4 = -.5f;
			}
			if(yDirection == 1)
			{
				dirX4 = -1;
				dirX1 = -.5f;
				dirX2 = 0;
				dirX3 = .5f;
				dirX5 = 1;
				dirX6 = -.1f;
				dirX7 = .1f;
				dirY1 = 1;
				dirY2 = 1;
				dirY3 = 1;
				dirY4 = 1;
				dirY5 = 1;
				dirY6 = 1;
				dirY7 = 1;
			}
			if(yDirection == -1)
			{
				dirX4 = -1;
				dirX1 = -.5f;
				dirX2 = 0;
				dirX3 = .5f;
				dirX5 = 1;
				dirX6 = .1f;
				dirX7 = -.1f;
				dirY4 = -1;
				dirY1 = -1;
				dirY2 = -1;
				dirY3 = -1;
				dirY5 = -1;
				dirY6 = -1;
				dirY7 = -1;
			}


			/* setting all the necessary attributes for the first bullet */
			bullet1->deleteFlag = 0;
			bullet1->range = weapon->range;
			bullet1->damage = weapon->damage;
			bullet1->graphic = '*';
			bullet1->X = (float)xPos;
			bullet1->Y = (float)yPos;
			bullet1->speed = 25;
			bullet1->lastMoveTime = 0;
			bullet1->yDirection = dirY1;
			bullet1->xDirection = dirX1;
			bullet1->firedTime = (int)getCurrentTime();
			bullet1->owner = weapon->owner;
			bullet1->ID = weapon->ID;

			/* setting all the necessary attributes for the second bullet */
			bullet2->deleteFlag = 0;
			bullet2->range = weapon->range;
			bullet2->damage = weapon->damage;
			bullet2->graphic = '*';
			bullet2->X = (float)xPos;
			bullet2->Y = (float)yPos;
			bullet2->speed = 30;
			bullet2->lastMoveTime = 0;
			bullet2->yDirection = dirY2;
			bullet2->xDirection = dirX2;
			bullet2->firedTime = (int)getCurrentTime();
			bullet2->owner = weapon->owner;
			bullet2->ID = weapon->ID;

			/* setting all the necessary attributes for the third bullet */
			bullet3->deleteFlag = 0;
			bullet3->range = weapon->range;
			bullet3->damage = weapon->damage;
			bullet3->graphic = '*';
			bullet3->X = (float)xPos;
			bullet3->Y = (float)yPos;
			bullet3->speed = 25;
			bullet3->lastMoveTime = 0;
			bullet3->yDirection = dirY3;
			bullet3->xDirection = dirX3;
			bullet3->firedTime = (int)getCurrentTime();
			bullet3->owner = weapon->owner;
			bullet3->ID = weapon->ID;

			/* setting all the necessary attributes for the fourth bullet */
			bullet4->deleteFlag = 0;
			bullet4->range = weapon->range;
			bullet4->damage = weapon->damage;
			bullet4->graphic = '*';
			bullet4->X = (float)xPos;
			bullet4->Y = (float)yPos;
			bullet4->speed = 20;
			bullet4->lastMoveTime = 0;
			bullet4->yDirection = dirY4;
			bullet4->xDirection = dirX4;
			bullet4->firedTime = (int)getCurrentTime();
			bullet4->owner = weapon->owner;
			bullet4->ID = weapon->ID;

			/* setting all the necessary attributes for the fifth bullet */
			bullet5->deleteFlag = 0;
			bullet5->range = weapon->range;
			bullet5->damage = weapon->damage;
			bullet5->graphic = '*';
			bullet5->X = (float)xPos;
			bullet5->Y = (float)yPos;
			bullet5->speed = 20;
			bullet5->lastMoveTime = 0;
			bullet5->yDirection = dirY5;
			bullet5->xDirection = dirX5;
			bullet5->firedTime = (int)getCurrentTime();
			bullet5->owner = weapon->owner;
			bullet5->ID = weapon->ID;

			/* setting all the necessary attributes for the sixth bullet */
			bullet6->deleteFlag = 0;
			bullet6->range = weapon->range;
			bullet6->damage = weapon->damage;
			bullet6->graphic = '*';
			bullet6->X = (float)xPos;
			bullet6->Y = (float)yPos;
			bullet6->speed = 45;
			bullet6->lastMoveTime = 0;
			bullet6->yDirection = dirY6;
			bullet6->xDirection = dirX6;
			bullet6->firedTime = (int)getCurrentTime();
			bullet6->owner = weapon->owner;
			bullet6->ID = weapon->ID;

			/* setting all the necessary attributes for the seventh bullet */
			bullet7->deleteFlag = 0;
			bullet7->range = weapon->range;
			bullet7->damage = weapon->damage;
			bullet7->graphic = '*';
			bullet7->X = (float)xPos;
			bullet7->Y = (float)yPos;
			bullet7->speed = 45;
			bullet7->lastMoveTime = 0;
			bullet7->yDirection = dirY7;
			bullet7->xDirection = dirX7;
			bullet7->firedTime = (int)getCurrentTime();
			bullet7->owner = weapon->owner;
			bullet7->ID = weapon->ID;


			/* creating the bullet position and graphic */
			bulletPosition1 = createLevelPosition(bullet1, POSITION_WEAPON, '*');
			bulletPosition2 = createLevelPosition(bullet2, POSITION_WEAPON, '*');
			bulletPosition3 = createLevelPosition(bullet3, POSITION_WEAPON, '*');
			bulletPosition4 = createLevelPosition(bullet4, POSITION_WEAPON, '*');
			bulletPosition5 = createLevelPosition(bullet5, POSITION_WEAPON, '*');
			bulletPosition6 = createLevelPosition(bullet6, POSITION_WEAPON, '*');
			bulletPosition7 = createLevelPosition(bullet7, POSITION_WEAPON, '*');

			/* setting the position */
			bullet1->bullet_pos = bulletPosition1;
			bullet2->bullet_pos = bulletPosition2;
			bullet3->bullet_pos = bulletPosition3;
			bullet4->bullet_pos = bulletPosition4;
			bullet5->bullet_pos = bulletPosition5;
			bullet6->bullet_pos = bulletPosition6;
			bullet7->bullet_pos = bulletPosition7;

			/* calling the add bullet function to actually add the bullet... */

			addBullet(bullet1);
			addBullet(bullet2);
			addBullet(bullet3);
			addBullet(bullet4);
			addBullet(bullet5);
			addBullet(bullet6);
			addBullet(bullet7);

			/* set the bullets' level position */
			setLevelPosition(WEAPON_LAYER, (int)bullet1->X, (int)bullet1->Y, bulletPosition1);
			setLevelPosition(WEAPON_LAYER, (int)bullet2->X, (int)bullet2->Y, bulletPosition2);
			setLevelPosition(WEAPON_LAYER, (int)bullet3->X, (int)bullet3->Y, bulletPosition3);
			setLevelPosition(WEAPON_LAYER, (int)bullet4->X, (int)bullet4->Y, bulletPosition4);
			setLevelPosition(WEAPON_LAYER, (int)bullet5->X, (int)bullet5->Y, bulletPosition5);
			setLevelPosition(WEAPON_LAYER, (int)bullet6->X, (int)bullet6->Y, bulletPosition6);
			setLevelPosition(WEAPON_LAYER, (int)bullet7->X, (int)bullet7->Y, bulletPosition7);

		}
		else if(weapon->ID == GUN_SPREADGUN)
		{
			/* declaring variables for the bullets' trajectory
			defined later */
			float dirX1 = 0;
			float dirX2 = 0;
			float dirX3 = 0;
			float dirX4 = 0;
			float dirX5 = 0;

			float dirY1 = 0;
			float dirY2 = 0;
			float dirY3 = 0;
			float dirY4 = 0;
			float dirY5 = 0;

			/* creating a level position for the bullet being created */
			LEVEL_POSITION *bulletPosition1;
			LEVEL_POSITION *bulletPosition2;
			LEVEL_POSITION *bulletPosition3;
			LEVEL_POSITION *bulletPosition4;
			LEVEL_POSITION *bulletPosition5;

			/* allocating memory for the bullet to be created */
			BULLET *bullet1 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet2 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet3 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet4 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet5 = (BULLET*)malloc(sizeof(BULLET));

			/* this section will set the trajectory for each bullet
			of the scatter gun depending on which way the player 
			is facing */
			if(xDirection == 1)
			{
				dirX1 = 1;
				dirX2 = 1;
				dirX3 = 1;
				dirX4 = 1;
				dirX5 = 1;
				dirY5 = .5f;
				dirY1 = .25f;
				dirY2 = 0;
				dirY3 = -.25f;
				dirY4 = -.5f;
			}
			if(xDirection == -1)
			{
				dirX1 = -1;
				dirX2 = -1;
				dirX3 = -1;
				dirX4 = -1;
				dirX5 = -1;
				dirY5 = .5f;
				dirY1 = .25f;
				dirY2 = 0;
				dirY3 = -.25f;
				dirY4 = -.5f;
			}
			if(yDirection == 1)
			{
				dirX4 = -1;
				dirX1 = -.5f;
				dirX2 = 0;
				dirX3 = .5f;
				dirX5 = 1;
				dirY1 = 1;
				dirY2 = 1;
				dirY3 = 1;
				dirY4 = 1;
				dirY5 = 1;
			}
			if(yDirection == -1)
			{
				dirX4 = -1;
				dirX1 = -.5f;
				dirX2 = 0;
				dirX3 = .5f;
				dirX5 = 1;
				dirY4 = -1;
				dirY1 = -1;
				dirY2 = -1;
				dirY3 = -1;
				dirY5 = -1;
			}


			/* setting all the necessary attributes for the first bullet */
			bullet1->deleteFlag = 0;
			bullet1->range = weapon->range;
			bullet1->damage = weapon->damage;
			bullet1->graphic = '*';
			bullet1->X = (float)xPos;
			bullet1->Y = (float)yPos;
			bullet1->speed = 30;
			bullet1->lastMoveTime = 0;
			bullet1->yDirection = dirY1;
			bullet1->xDirection = dirX1;
			bullet1->firedTime = (int)getCurrentTime();
			bullet1->owner = weapon->owner;
			bullet1->ID = weapon->ID;

			/* setting all the necessary attributes for the second bullet */
			bullet2->deleteFlag = 0;
			bullet2->range = weapon->range;
			bullet2->damage = weapon->damage;
			bullet2->graphic = '*';
			bullet2->X = (float)xPos;
			bullet2->Y = (float)yPos;
			bullet2->speed = 30;
			bullet2->lastMoveTime = 0;
			bullet2->yDirection = dirY2;
			bullet2->xDirection = dirX2;
			bullet2->firedTime = (int)getCurrentTime();
			bullet2->owner = weapon->owner;
			bullet2->ID = weapon->ID;

			/* setting all the necessary attributes for the third bullet */
			bullet3->deleteFlag = 0;
			bullet3->range = weapon->range;
			bullet3->damage = weapon->damage;
			bullet3->graphic = '*';
			bullet3->X = (float)xPos;
			bullet3->Y = (float)yPos;
			bullet3->speed = 30;
			bullet3->lastMoveTime = 0;
			bullet3->yDirection = dirY3;
			bullet3->xDirection = dirX3;
			bullet3->firedTime = (int)getCurrentTime();
			bullet3->owner = weapon->owner;
			bullet3->ID = weapon->ID;

			/* setting all the necessary attributes for the fourth bullet */
			bullet4->deleteFlag = 0;
			bullet4->range = weapon->range;
			bullet4->damage = weapon->damage;
			bullet4->graphic = '*';
			bullet4->X = (float)xPos;
			bullet4->Y = (float)yPos;
			bullet4->speed = 30;
			bullet4->lastMoveTime = 0;
			bullet4->yDirection = dirY4;
			bullet4->xDirection = dirX4;
			bullet4->firedTime = (int)getCurrentTime();
			bullet4->owner = weapon->owner;
			bullet4->ID = weapon->ID;

			/* setting all the necessary attributes for the fifth bullet */
			bullet5->deleteFlag = 0;
			bullet5->range = weapon->range;
			bullet5->damage = weapon->damage;
			bullet5->graphic = '*';
			bullet5->X = (float)xPos;
			bullet5->Y = (float)yPos;
			bullet5->speed = 30;
			bullet5->lastMoveTime = 0;
			bullet5->yDirection = dirY5;
			bullet5->xDirection = dirX5;
			bullet5->firedTime = (int)getCurrentTime();
			bullet5->owner = weapon->owner;
			bullet4->ID = weapon->ID;

			/* creating the bullets position and graphic */
			bulletPosition1 = createLevelPosition(bullet1, POSITION_WEAPON, '*');
			bulletPosition2 = createLevelPosition(bullet2, POSITION_WEAPON, '*');
			bulletPosition3 = createLevelPosition(bullet3, POSITION_WEAPON, '*');
			bulletPosition4 = createLevelPosition(bullet4, POSITION_WEAPON, '*');
			bulletPosition5 = createLevelPosition(bullet5, POSITION_WEAPON, '*');

			/* setting the position */
			bullet1->bullet_pos = bulletPosition1;
			bullet2->bullet_pos = bulletPosition2;
			bullet3->bullet_pos = bulletPosition3;
			bullet4->bullet_pos = bulletPosition4;
			bullet5->bullet_pos = bulletPosition5;

			/* calling the add bullet function to actually add the bullet... */

			addBullet(bullet1);
			addBullet(bullet2);
			addBullet(bullet3);
			addBullet(bullet4);
			addBullet(bullet5);

			/* set the bullets' level position */
			setLevelPosition(WEAPON_LAYER, (int)bullet1->X, (int)bullet1->Y, bulletPosition1);
			setLevelPosition(WEAPON_LAYER, (int)bullet2->X, (int)bullet2->Y, bulletPosition2);
			setLevelPosition(WEAPON_LAYER, (int)bullet3->X, (int)bullet3->Y, bulletPosition3);
			setLevelPosition(WEAPON_LAYER, (int)bullet4->X, (int)bullet4->Y, bulletPosition4);
			setLevelPosition(WEAPON_LAYER, (int)bullet5->X, (int)bullet5->Y, bulletPosition5);
		}
		else if(weapon->ID == GUN_OMEGALASER)
		{
			/* creating values to hold the bullets' coordinates */
			float bullet1X = 0;
			float bullet1Y = 0;
			float bullet2X = 0;
			float bullet2Y = 0;
			float bullet4X = 0;
			float bullet4Y = 0;

			/* creating a level position for the bullet being created */
			LEVEL_POSITION *bulletPosition1;
			LEVEL_POSITION *bulletPosition2;
			LEVEL_POSITION *bulletPosition3;
			LEVEL_POSITION *bulletPosition4;

			/* allocating memory for the bullet to be created */
			BULLET *bullet1 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet2 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet3 = (BULLET*)malloc(sizeof(BULLET));
			BULLET *bullet4 = (BULLET*)malloc(sizeof(BULLET));

			/* defining variables for the initial position of the bullets
			shot from the dual pistols*/

			if(xDirection == 1)
			{
				/* creating a bullet on either side of the player, and
				one space in front of the player with respect to y */
				bullet1X = (float)xPos + 1;
				bullet1Y = (float)yPos - 1;

				bullet2X = (float)xPos + 1;
				bullet2Y = (float)yPos + 1;

				bullet4X = (float)xPos + 1;
				bullet4Y = (float)yPos;

				/* giving the laser its graphic */
				bullet1->graphic = '\\';
				bullet2->graphic = '/';
			}
			if(xDirection == -1)
			{
				/* creating a bullet on either side of the player, and
				one space in front of the player with respect to y */
				bullet1X = (float)xPos - 1;
				bullet1Y = (float)yPos - 1;

				bullet2X = (float)xPos - 1;
				bullet2Y = (float)yPos + 1;

				bullet4X = (float)xPos -1;
				bullet4Y = (float)yPos;

				/* giving the laser its graphic */
				bullet1->graphic = '/';
				bullet2->graphic = '\\';
			}
			if(yDirection == 1)
			{
				/* creating a bullet on either side of the player, and
				one space in front of the player with respect to x */	
				bullet4Y = (float)yPos + 1;
				bullet4X = (float)xPos;

				bullet1X = (float)xPos + 1;
				bullet1Y = (float)yPos + 1;

				bullet2X = (float)xPos - 1;
				bullet2Y = (float)yPos + 1;

				/* giving the laser its graphic */
				bullet1->graphic = '/';
				bullet2->graphic = '\\';
			}
			if(yDirection == -1)
			{
				/* creating a bullet on either side of the player, and
				one space in front of the player with respect to x */
				bullet1X = (float)xPos + 1;
				bullet1Y = (float)yPos - 1;

				bullet2X = (float)xPos - 1;
				bullet2Y = (float)yPos - 1;

				bullet4Y = (float)yPos -1;
				bullet4X = (float)xPos;

				/* giving the laser its graphic */
				bullet1->graphic = '\\';
				bullet2->graphic = '/';
			}


			/* setting all the necessary attributes for the first bullet */
			bullet1->deleteFlag = 0;
			bullet1->range = weapon->range;
			bullet1->damage = weapon->damage;
			bullet1->X = bullet1X;
			bullet1->Y = bullet1Y;
			bullet1->speed = 60;
			bullet1->lastMoveTime = 0;
			bullet1->yDirection = yDirection;
			bullet1->xDirection = xDirection;
			bullet1->firedTime = (int)getCurrentTime();
			bullet1->owner = weapon->owner;
			bullet1->ID = weapon->ID;

			/* setting all the necessary attributes for the second bullet */
			bullet2->deleteFlag = 0;
			bullet2->range = weapon->range;
			bullet2->damage = weapon->damage;
			bullet2->X = bullet2X;
			bullet2->Y = bullet2Y;
			bullet2->speed = 60;
			bullet2->lastMoveTime = 0;
			bullet2->yDirection = yDirection;
			bullet2->xDirection = xDirection;
			bullet2->firedTime = (int)getCurrentTime();
			bullet2->owner = weapon->owner;
			bullet2->ID = weapon->ID;

			/* setting all the necessary attributes for the three bullet */
			bullet3->deleteFlag = 0;
			bullet3->range = weapon->range;
			bullet3->damage = weapon->damage;
			bullet3->graphic = '*';
			bullet3->X = (float)xPos;
			bullet3->Y = (float)yPos;
			bullet3->speed = 60;
			bullet3->lastMoveTime = 0;
			bullet3->yDirection = yDirection;
			bullet3->xDirection = xDirection;
			bullet3->firedTime = (int)getCurrentTime();
			bullet3->owner = weapon->owner;
			bullet3->ID = weapon->ID;

			/* setting all the necessary attributes for the fourth bullet */
			bullet4->deleteFlag = 0;
			bullet4->range = weapon->range;
			bullet4->damage = weapon->damage;
			bullet4->graphic = '*';
			bullet4->X = bullet4X;
			bullet4->Y = bullet4Y;
			bullet4->speed = 60;
			bullet4->lastMoveTime = 0;
			bullet4->yDirection = yDirection;
			bullet4->xDirection = xDirection;
			bullet4->firedTime = (int)getCurrentTime();
			bullet4->owner = weapon->owner;
			bullet4->ID = weapon->ID;

			/* creating the bullets position and graphic */
			bulletPosition1 = createLevelPosition(bullet1, POSITION_WEAPON, '*');
			bulletPosition2 = createLevelPosition(bullet2, POSITION_WEAPON, '*');
			bulletPosition3 = createLevelPosition(bullet3, POSITION_WEAPON, '*');
			bulletPosition4 = createLevelPosition(bullet4, POSITION_WEAPON, '*');

			/* setting the position */
			bullet1->bullet_pos = bulletPosition1;
			bullet2->bullet_pos = bulletPosition2;
			bullet3->bullet_pos = bulletPosition3;
			bullet4->bullet_pos = bulletPosition4;

			/* calling the add bullet function to actually add the bullet... */
			addBullet(bullet1);
			addBullet(bullet2);
			addBullet(bullet3);
			addBullet(bullet4);

			/* set the bullet's level position */
			setLevelPosition(WEAPON_LAYER, (int)bullet1X, (int)bullet1Y, bulletPosition1);
			setLevelPosition(WEAPON_LAYER, (int)bullet2X, (int)bullet2Y, bulletPosition2);
			setLevelPosition(WEAPON_LAYER, xPos, yPos, bulletPosition3);
			setLevelPosition(WEAPON_LAYER, (int)bullet4X, (int)bullet4Y, bulletPosition4);

		}
		else if(weapon->ID == GUN_SUBMACHINEGUN)
		{

			/* giving the sub machine gun scattered accuracy */
			float dirX = ((float)(rand() % 3) / 10);
			float dirY = ((float)(rand() % 2) / 10);

			/* making half of the scattered bullets in the negative
			direction */
			int neg = (rand() % 2);

			/* creating a level position for the bullet being created */
			LEVEL_POSITION *bulletPosition;

			/* allocating memory for the bullet to be created */
			BULLET *bullet = (BULLET*)malloc(sizeof(BULLET));

			if(neg == 1)
			{
				/* if the bullet is negative, send it in the negative
				direction */
				dirX = (dirX * -1);
				dirY = (dirY * -1);
			}

			if(xDirection == 1)
			{
				/* setting direction variable to passed player value */
				dirX = xDirection;
			}
			else if(xDirection == -1)
			{
				/* setting direction variable to passed player value */
				dirX = xDirection;
			}
			if(yDirection == 1)
			{
				/* setting direction variable to passed player value */
				dirY = yDirection;
			}
			else if(yDirection == -1)
			{
				/* setting direction variable to passed player value */
				dirY = yDirection;
			}

			/* setting all the necessary attributes for the bullet */
			bullet->deleteFlag = 0;
			bullet->range = weapon->range;
			bullet->damage = weapon->damage;
			bullet->graphic = '*';
			bullet->X = (float)xPos;
			bullet->Y = (float)yPos;
			bullet->speed = 30;
			bullet->lastMoveTime = 0;
			bullet->yDirection = dirY;
			bullet->xDirection = dirX;
			bullet->firedTime = (int)getCurrentTime();
			bullet->owner = weapon->owner;
			bullet->ID = weapon->ID;

			/* creating the bullet's position and graphic */
			bulletPosition = createLevelPosition(bullet, POSITION_WEAPON, '*');

			/* setting the position */
			bullet->bullet_pos = bulletPosition;

			/* calling the add bullet function to actually add the bullet... */
			addBullet(bullet);

			/* set the bullet's level position */
			setLevelPosition(WEAPON_LAYER, (int)bullet->X, (int)bullet->Y, bulletPosition);
		}
		else if(weapon->ID == GUN_ROCKETLAUNCHER)
		{

			/* creating a level position for the bullet being created */
			LEVEL_POSITION *bulletPosition;

			/* allocating memory for the bullet to be created */
			BULLET *bullet = (BULLET*)malloc(sizeof(BULLET));

			/* setting all the necessary attributes for the bullet */
			bullet->deleteFlag = 0;
			bullet->range = weapon->range;
			bullet->damage = weapon->damage;
			bullet->graphic = '*';
			bullet->X = (float)xPos;
			bullet->Y = (float)yPos;
			bullet->speed = 30;
			bullet->lastMoveTime = 0;
			bullet->yDirection = yDirection;
			bullet->xDirection = xDirection;
			bullet->firedTime = (int)getCurrentTime();
			bullet->owner = weapon->owner;
			bullet->ID = weapon->ID;

			/* creating the bullets position and graphic */
			bulletPosition = createLevelPosition(bullet, POSITION_WEAPON, '*');

			/* setting the position */
			bullet->bullet_pos = bulletPosition;

			/* calling the add bullet function to actually add the bullet... */
			addBullet(bullet);

			/* set the bullet's level position */
			setLevelPosition(WEAPON_LAYER, (int)bullet->X, (int)bullet->Y, bulletPosition);
		}
		else if(weapon->ID == GUN_GERNADE)
		{

			/* creating a level position for the bullet being created */
			LEVEL_POSITION *bulletPosition;

			/* allocating memory for the bullet to be created */
			BULLET *bullet = (BULLET*)malloc(sizeof(BULLET));

			/* setting all the necessary attributes for the bullet */
			bullet->deleteFlag = 0;
			bullet->range = weapon->range;
			bullet->damage = weapon->damage;
			bullet->X = (float)xPos;
			bullet->Y = (float)yPos;
			bullet->speed = 65;
			bullet->lastMoveTime = 0;
			bullet->yDirection = yDirection;
			bullet->xDirection = xDirection;
			bullet->firedTime = (int)getCurrentTime();
			bullet->owner = weapon->owner;
			bullet->ID = weapon->ID;

			/* creating the bullet's position and graphic */
			bulletPosition = createLevelPosition(bullet, POSITION_WEAPON, '*');

			/* setting the position */
			bullet->bullet_pos = bulletPosition;

			/* calling the add bullet function to actually add the bullet... */
			addBullet(bullet);

			/* set the bullet's level position */
			setLevelPosition(WEAPON_LAYER, (int)bullet->X, (int)bullet->Y, bulletPosition);
		}
		else
		{

			/* creating a level position for the bullet being created */
			LEVEL_POSITION *bulletPosition;

			/* allocating memory for the bullet to be created */
			BULLET *bullet = (BULLET*)malloc(sizeof(BULLET));

			/* setting all the necessary attributes for the bullet */
			bullet->deleteFlag = 0;
			bullet->range = weapon->range;
			bullet->damage = weapon->damage;
			bullet->graphic = '*';
			bullet->X = (float)xPos;
			bullet->Y = (float)yPos;

			/* if the weapon is either the rifle or the sniper,
			speed up the bullet, if not, keep it the same
			speed
			*/
			if(weapon->ID == GUN_RIFLE)
			{
				bullet->speed = 80;
			}
			else if(weapon->ID == GUN_SNIPER)
			{
				bullet->speed = 160;
			}
			else
			{
				bullet->speed = 30;
			}
			bullet->lastMoveTime = 0;
			bullet->yDirection = yDirection;
			bullet->xDirection = xDirection;
			bullet->firedTime = (int)getCurrentTime();
			bullet->owner = weapon->owner;
			bullet->ID = weapon->ID;

			/* creating the bullets position and graphic */
			bulletPosition = createLevelPosition(bullet, POSITION_WEAPON, '*');

			/* setting the position */
			bullet->bullet_pos = bulletPosition;

			/* calling the add bullet function to actually add the bullet... */
			addBullet(bullet);

			/* set the bullet's level position */
			setLevelPosition(WEAPON_LAYER, (int)bullet->X, (int)bullet->Y, bulletPosition);
		}
	}
}

/*******************************************************************************
Function:    clearBullet()

Description: This function clears all bullets that have been created,
this is needed for restarting

Inputs:      None

Outputs:     void
*******************************************************************************/
void clearBullets()
{
	/* setting the bullets clear flag so that they are deleted*/
	clearBulletFlag = 1;

	/* calling the updateBulletList function so that on the update the bullets
	are deleted
	*/
	updateBulletList();

	/* after the reset the clearBulletFlag must me set to 0 so that more
	bullets can be created
	*/
	clearBulletFlag = 0;
}
/*******************************************************************************
Function:    updateBulletList()

Description: This function is called every game loop. It makes sure
the bullet linked list has been updated correctly. It calls
the updateBullet function to update every bullet in the list

Inputs:      None

Outputs:     This function returns mainBullet, a pointer to the first bullet
in the list. 
*******************************************************************************/
BULLET_LINKLIST* updateBulletList()
{
	/* create two pointers to keep track of where we are in the list */
	BULLET_LINKLIST* prev = NULL;
	BULLET_LINKLIST* current = mainBullet;

	/* while we aren't at the last bullet */
	while (current != NULL)
	{
		/* check to make sure we aren't pointing at the dummy bullet */
		if (prev != NULL)
		{

			/* if the bullet's delete flag is 1 */
			if (current->data->deleteFlag == 1 || clearBulletFlag == 1)
			{
				/* move the previous bullet's pointer to point past the deleted bullet
				(this keeps the list connected) 
				*/
				prev->next = current->next;

				/* free level position */
				setLevelPosition(WEAPON_LAYER, (int)current->data->X, (int)current->data->Y, whiteSpacePosition);

				free(current->data->bullet_pos);

				/* free our copy of our bullet */
				free(current->data);

				/* free deleted bullet */
				free(current);

				current = prev;
			}

			/* if the bullet isn't deleted, update it */
			else
			{
				updateBullet(current->data);	
			}
		}

		/* keep track of previous bullet */
		prev = current;

		/* increment pointer */
		current = current->next;
	}

	return mainBullet;
}
/*******************************************************************************
Function:    updateBullet

Description: This function updates each bullet and deals with collision.

Inputs:      BULLET* bullet - pointer to the bullet to update.

Outputs:     None
*******************************************************************************/
void updateBullet(BULLET* bullet)
{
	outOfRange(bullet);
	/* see if its time to move again */
	if((getCurrentTime() - bullet->lastMoveTime) >= (1000 / bullet->speed))
	{
		/* create a new X and Y value for the bullet*/
		float newX = bullet->X + bullet->xDirection;
		float newY = bullet->Y + bullet->yDirection;

		/* if the level space is not a wall then do the following */
		if(!isLevelPositionWall(WALL_LAYER, roundF(newX), roundF(newY)))
		{
			/* if the bullet is from the grenade */
			if(bullet->ID == GUN_GERNADE)
			{
				/* slow the grenade down every update */
				if(bullet->speed > 5)
				{
					bullet->speed -= 5;
				}
				/* if the grenade was just thrown, make it a period */
				if((int)getCurrentTime() - bullet->firedTime < 50)
				{
					bullet->graphic = '.';
				}
				/* if the grenade has been up for a while, change it to an o */
				else if((int)getCurrentTime() - bullet->firedTime >= 50 && 
					(int)getCurrentTime() - bullet->firedTime < 200)
				{
					bullet->graphic = 'o';
				}
				/* after a while, change the graphic to an O */
				else if((int)getCurrentTime() - bullet->firedTime >= 200 && 
					(int)getCurrentTime() - bullet->firedTime <= 500)
				{
					bullet->graphic = 'O';
				}
				/* on its way down, change it back to an o */
				else if((int)getCurrentTime() - bullet->firedTime > 500 && 
					(int)getCurrentTime() - bullet->firedTime <= 700)
				{
					bullet->graphic = 'o';
				}
				/* before deletion, change it to a period again */
				else if((int)getCurrentTime() - bullet->firedTime > 700 && 
					(int)getCurrentTime() - bullet->firedTime <= 1000)
				{
					bullet->graphic = '.';
				}
			}



			/* write a whitespace in the location where the bullet last was */
			setLevelPosition(WEAPON_LAYER, roundF(bullet->X), roundF(bullet->Y), whiteSpacePosition);

			bullet->X = newX;
			bullet->Y = newY;

			/* write the new bullet (with a new position) */
			setLevelPosition(WEAPON_LAYER, roundF(bullet->X), roundF(bullet->Y), bullet->bullet_pos);

			/* store the last movement time */
			bullet->lastMoveTime = getCurrentTime();
		}

		/* otherwise, delete the bullet */
		else
		{	
			if(bullet->ID == GUN_GERNADE)
			{
				explosion((int)bullet->X, (int)bullet->Y, 500, bullet->owner);
			}
			/* if the bullet came from the rocket launcher, before
			deleting, make explosions depending upon which direction
			the rocket was fired, these stipulations handle explosions
			not happening in walls
			*/

			if(bullet->ID == GUN_ROCKETLAUNCHER)
			{
				/* creating values for the coordinates of the rocket
				launcher's explosions */
				float randX1 = bullet->X;
				float randX2 = bullet->X;
				float randY1 = bullet->Y;
				float randY2 = bullet->Y;

				/*if the coordinates of the rocket launcher aren't outside the map
				create them*/
				if(((int)randX1 >= 0) && (isLevelPositionSpace(WALL_LAYER, (int)randX1, (int)bullet->Y)))
				{
					explosion((int)randX1, (int)bullet->Y, 300, bullet->owner);
				}
				if((int)randX2 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)randX2, (int)bullet->Y)))
				{
					explosion((int)randX2, (int)bullet->Y, 300, bullet->owner);
				}
				if((int)randY1 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)bullet->X, (int)randY1)))
				{
					explosion((int)bullet->X, (int)randY1, 300, bullet->owner);
				}
				if((int)randY2 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)bullet->X, (int)randY2)))
				{
					explosion((int)bullet->X, (int)randY2, 300, bullet->owner);
				}
			}

			/* delete the bullet */
			bullet->deleteFlag = 1;
		}
	}

	/* if the level space is on the enemy layer and is not white space, do the following */
	if(!isLevelPositionSpace(ENEMY_LAYER, (int)bullet->X, (int)bullet->Y))
	{
		/* get the level position of the bullet */
		LEVEL_POSITION* lvlPos = getLevelPosition(ENEMY_LAYER, (int)bullet->X, (int)bullet->Y);

		/* if the type of data in that space is an enemy, do the following */
		if(lvlPos->type == POSITION_ENEMY && bullet->owner == OWNER_PLAYER)
		{
			/* call the enemy damage function and delete the bullet */
			damageEnemy((ENEMY*)(lvlPos->data), bullet->damage);

			/* if the bullet is a gernade, create and explosion on
			deletion */
			if(bullet->ID == GUN_GERNADE)
			{
				explosion((int)bullet->X, (int)bullet->Y, 500, bullet->owner);
			}

			/* if the bullet came from the...*/
			if(bullet->ID == GUN_ROCKETLAUNCHER)
			{
				/* creating values for the coordinates of the rocket
				launcher's explosions */
				float randX1 = (bullet->X - (rand() % 10));
				float randX2 = (bullet->X + (rand() % 10));
				float randY1 = (bullet->Y - (rand() % 10));
				float randY2 = (bullet->Y + (rand() % 10));

				/*if the coordinates of the rocket launcher aren't outside the map
				create them*/
				if(((int)randX1 >= 0) && (isLevelPositionSpace(WALL_LAYER, (int)randX1, (int)bullet->Y)))
				{
					explosion((int)randX1, (int)bullet->Y, 300, OWNER_PLAYER);
				}
				if((int)randX2 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)randX2, (int)bullet->Y)))
				{
					explosion((int)randX2, (int)bullet->Y, 300, OWNER_PLAYER);
				}
				if((int)randY1 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)bullet->X, (int)randY1)))
				{
					explosion((int)bullet->X, (int)randY1, 300, OWNER_PLAYER);
				}
				if((int)randY2 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)bullet->X, (int)randY2)))
				{
					explosion((int)bullet->X, (int)randY2, 300, OWNER_PLAYER);
				}
			}

			/* if the bullet is not from a sniper rifle, delete it */
			if(!(bullet->ID == GUN_SNIPER || bullet->ID == GUN_RIFLE || bullet->ID == GUN_OMEGALASER))
				bullet->deleteFlag = 1;
		}
		/* if the data is not an enemy (an enemy spawn), do the following */
		else if(lvlPos->type == POSITION_SPAWN && bullet->owner == OWNER_PLAYER)
		{
			/* call the damage spawn function and delete the bullet */
			damageSpawn((ENEMY_SPAWN*)(lvlPos->data), bullet->damage);


			/* if the bullet is a gernade, create and explosion on
			deletion */
			if(bullet->ID == GUN_GERNADE)
			{
				explosion((int)bullet->X, (int)bullet->Y, 500, bullet->owner);

				/* damage spawn on contact */ 
				damageSpawn((ENEMY_SPAWN*)(lvlPos->data), 10);
			}

			/* if the bullet came from the...*/
			if(bullet->ID == GUN_ROCKETLAUNCHER)
			{
				/* creating values for the coordinates of the rocket
				launcher's explosions */
				float randX1 = (bullet->X - (rand() % 10));
				float randX2 = (bullet->X + (rand() % 10));
				float randY1 = (bullet->Y - (rand() % 10));
				float randY2 = (bullet->Y + (rand() % 10));

				/*if the coordinates of the rocket launcher aren't outside the map
				create them*/
				if(((int)randX1 >= 0) && (isLevelPositionSpace(WALL_LAYER, (int)randX1, (int)bullet->Y)))
				{
					explosion((int)randX1, (int)bullet->Y, 300, OWNER_PLAYER);
				}
				if((int)randX2 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)randX2, (int)bullet->Y)))
				{
					explosion((int)randX2, (int)bullet->Y, 300, OWNER_PLAYER);
				}
				if((int)randY1 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)bullet->X, (int)randY1)))
				{
					explosion((int)bullet->X, (int)randY1, 300, OWNER_PLAYER);
				}
				if((int)randY2 >= 0 && (isLevelPositionSpace(WALL_LAYER, (int)bullet->X, (int)randY2)))
				{
					explosion((int)bullet->X, (int)randY2, 300, OWNER_PLAYER);
				}
			}

			/* if the bullet is not from a sniper rifle, delete it */
			if(!(bullet->ID == GUN_SNIPER || bullet->ID == GUN_RIFLE || bullet->ID == GUN_OMEGALASER))
				bullet->deleteFlag = 1;
		}


	}
	/* if the level space is on the player layer, and is not white space, do the following */ 
	if(!isLevelPositionSpace(PLAYER_LAYER, (int)bullet->X, (int)bullet->Y))
	{
		/* get the level position of the bullet */
		LEVEL_POSITION* lvlPos = getLevelPosition(PLAYER_LAYER, (int)bullet->X, (int)bullet->Y);

		/* if the type of data in that space is the player, do the following */
		if(lvlPos->type == POSITION_PLAYER && bullet->owner == OWNER_ENEMY)
		{
			/* damage the player by the bullet's damage */
			player.hp -= bullet->damage;

			/* delete the bullet */
			bullet->deleteFlag = 1;

			/* if the player has less than 0 health, set it back to 0 */
			if(player.hp < 0)
				player.hp = 0;
		}
	}

}
/*******************************************************************************
Function:    addBullet

Description: This function is called from shoot. It creates a new bullet
and gives it the necessary information

Inputs:      BULLET* input - a pointer to a bullet struct

Outputs:     a pointer to the bullet created
*******************************************************************************/
BULLET_LINKLIST* addBullet(BULLET* input)
{
	/* allocate memory for a new node in the list */
	BULLET_LINKLIST* newNode = (BULLET_LINKLIST*) malloc (sizeof(BULLET_LINKLIST));
	BULLET_LINKLIST* current = mainBullet; 

	/* if we are given the memory */
	if (newNode)
	{
		/* fill the node with the bullet's information */
		newNode->data = input;
		newNode->next = NULL;
	}

	/* if we aren't at the end of the list */
	if (current != NULL)
	{
		/* loop until we are at the end of the list */
		while (current->next != NULL)
			current = current->next;

		/* point to our new node */
		current->next = newNode;  

	}
	return newNode;
}
/*******************************************************************************
Function:    bulletCopy

Description: This function makes a copy of a bullet.

Inputs:      BULLET* bullet - pointer to an bullet struct.

Outputs:     Returns a pointer to the bullet created
*******************************************************************************/
BULLET* bulletCopy (BULLET* bullet)
{
	BULLET* newbullet = (BULLET* )malloc(sizeof(BULLET));

	memcpy(newbullet, bullet, sizeof(BULLET));

	return newbullet;
}
/*******************************************************************************
Function:    gunCopy

Description: This function makes a copy of the gun

Inputs:      GUN* gun - pointer to an gun struct.

Outputs:     Returns a pointer to the gun created
*******************************************************************************/
GUN* gunCopy (GUN* gun)
{
	GUN* newgun = (GUN* )malloc(sizeof(GUN));

	memcpy(newgun, gun, sizeof(GUN));

	return newgun;
}
/*******************************************************************************
Function:    reloadTime

Description: This function controls the rate of fire for weapons

Inputs:      GUN* weapon - a weapon struct

Outputs:     1 - True (able to fire) 0 - False (reloading)
*******************************************************************************/
int reloadTime(GUN* weapon)
{
	/* if the weapon has been reloaded return true */
	if((getCurrentTime() - weapon->lastFired) >= weapon->reloadTime)
	{
		weapon->lastFired = (int)getCurrentTime();
		return 1;
	}
	return 0;
}
/*******************************************************************************
Function:    range

Description: This function deletes bullets if they go out of range

Inputs:      GUN* weapon - a weapon struct

Outputs:     
*******************************************************************************/
void outOfRange(BULLET* bullet)
{
	/* if the bullet is out of range, delete it */
	if((getCurrentTime() - bullet->firedTime) >= bullet->range)
	{
		/* if the gernade gets outside its range, delete it */
		if(bullet->ID == GUN_GERNADE)
		{
			explosion((int)bullet->X, (int)bullet->Y, 500, bullet->owner);
		}
		bullet->deleteFlag = 1;
	}
}
/*******************************************************************************
Function:    getCurrentWeapon

Description: This function gets the current weapon from the weapon ID

Inputs:      WEAPON_ID weapon

Outputs:     GUN weaponPistol
		     GUN weaponDualPistol
			 GUN weaponSpread
			 GUN weaponScatterGun
			 GUN weaponSubMachineGun
			 GUN weaponHeavyMachineGun
			 GUN weaponRifle
			 GUN weaponSniper
			 GUN weaponRocketLauncher
			 GUN weaponOmegaLaser
*******************************************************************************/
GUN* getCurrentWeapon(WEAPON_ID weapon)
{
	/* switch statement for each weapon */
	switch(weapon)
	{
		/* if it's the pistol ID, return the pistol */
	case GUN_PISTOL:
		return &weaponPistol;
		break;

		/* if it's the dual pistol ID, return the dual pistol */
	case GUN_DUALPISTOL:
		return &weaponDualPistol;
		break;

		/* if it's the spread gun ID, return the spread gun */
	case GUN_SPREADGUN:
		return &weaponSpreadGun;
		break;

		/* if it's the scatter gun ID, return the scatter gun */
	case GUN_SCATTERGUN:
		return &weaponScatterGun;
		break;


		/* if it's the sub machine gun ID, return the sub machine gun */
	case GUN_SUBMACHINEGUN:
		return &weaponSubMachineGun;
		break;

		/* it it's the heavy machine gun ID, return the heavy machine gun */
	case GUN_HEAVYMACHINEGUN:
		return &weaponHeavyMachineGun;
		break;

		/* if it's the rifle ID, return the rifle */
	case GUN_RIFLE:
		return &weaponRifle;
		break;

		/* if it's the sniper ID, return the sniper rifle */
	case GUN_SNIPER:
		return &weaponSniper;
		break;

		/* if it's the rocket launcher ID, return the rocket launcher */
	case GUN_ROCKETLAUNCHER:
		return &weaponRocketLauncher;
		break;

		/* if it's the omega laser ID, return the omega laser */
	case GUN_OMEGALASER:
		return &weaponOmegaLaser;
		break;
		default:
			break;
	}
	return &weaponPistol;
}


int roundF(float value)
{
	return (int)value;
}
