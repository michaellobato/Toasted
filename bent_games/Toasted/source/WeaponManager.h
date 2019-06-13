/*******************************************************************************
  Author: Cullen McElroy
  Game:	  Toasted
  File:	  weapon.h
  Date:	  January 29th
  Description:
          This is a file for weaponManager.c, it prototypes all the weapons
		  and their attributes. It also prototypes all the functions involving
		  the weapons.
*******************************************************************************/

#ifndef WEAPON_H
#define WEAPON_H

#include "LevelManager.h"

#define WEAPON_COUNT 10

/* this enum is used to tell from whom a bullet came from */
typedef enum {OWNER_PLAYER = 1, OWNER_ENEMY, OWNER_NEITHER, OWNER_BOTH} E_OWNER;

/* an enum for giving each weapon an ID */
typedef enum
{
	GUN_INVALID = -1,
	GUN_PISTOL = 0,
	GUN_DUALPISTOL,
	GUN_SPREADGUN,
	GUN_SCATTERGUN,
	GUN_SUBMACHINEGUN,
	GUN_HEAVYMACHINEGUN,
	GUN_RIFLE,
	GUN_SNIPER,
	GUN_ROCKETLAUNCHER,
	GUN_OMEGALASER,
	GUN_GERNADE
} WEAPON_ID;


/* this struct is used as a template for each gun to set its attributes */
typedef struct
{
	int X;
	int Y;
	int lastFired;
	int reloadTime;
	int range;
	int damage;
	char graphic;
	E_OWNER owner;
	WEAPON_ID ID;
	LEVEL_POSITION* weapon_pos;
} GUN;

/* this struct is used as a template for bullets to set their attributes */
typedef struct
{
	float X;
	float Y;
	int range;
	int damage;
	int firedTime;
	int deleteFlag;
	char graphic;
	LEVEL_POSITION* bullet_pos;
	double lastMoveTime;
	int speed;
	float xDirection;
	float yDirection;
	E_OWNER owner;
	WEAPON_ID ID;
} BULLET;

GUN weaponPistol;
GUN weaponDualPistol;
GUN weaponSpreadGun;
GUN weaponScatterGun;
GUN weaponSubMachineGun;
GUN weaponHeavyMachineGun;
GUN weaponRifle;
GUN weaponSniper;
GUN weaponRocketLauncher;
GUN weaponOmegaLaser;
GUN weaponGernade;

GUN* weaponArray[WEAPON_COUNT];

/* this creates a linked list for bullets */
typedef struct bullet_linklist
{
	BULLET* data;
	struct bullet_linklist* next;
} BULLET_LINKLIST;

/* clears all bullets */
int clearBulletFlag;

void clearBullets();

/* called by main every loop, this function updates all bullets */
void loadBullet();

/* this creates a bullet */
BULLET_LINKLIST* addBullet(BULLET* input);

/* this updates the bullet linked list */
BULLET_LINKLIST* updateBulletList();

/* this creates the first node of the linked list */
BULLET_LINKLIST* mainBullet;

/* this updates all bullets attributes */
void updateBullet(BULLET* bullet);

/* this creates copies of bullets */
BULLET* bulletCopy (BULLET* bullet);

/* this creates a copy of a gun */
GUN* gunCopy(GUN *bullet);

/* the main function for WeaponManager.c, it loads all weapon info */
int loadWeapons(void);

/* checks to see if the weapon passed is reloaded */
int reloadTime(GUN* weapon);

/* this function deletes bullets if they get out of range */
void outOfRange(BULLET* bullet);

/* allows the player to shoot */
void shoot(GUN* Weapon, float xDirection, float yDirection, int xPos, int yPos);

/* gets the current weapon from the weapon ID */
GUN* getCurrentWeapon(WEAPON_ID weapon);

/* rounding function */
int roundF(float value);

#endif
