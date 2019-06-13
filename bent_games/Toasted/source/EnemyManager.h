/*******************************************************************************
  Author: Paul Ewers
  Game:	  Toasted
  File:	  EnemyManager.h
  Date:	  January 23rd, 2007
          (modifyed through: April 5th, 2007)
  Description:
          This is the header file for MoveEnemy.c. It is to include
          all prototypes and type defines, enums, structures, etc. This
          file should be included by any other file that needs to access
          the move enemy functions.
*******************************************************************************/

#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "LevelManager.h"
#include "WeaponManager.h"

/* create our enemy struct that will hold all the information for each
   enemy */
typedef struct
{
  char type;
  int health;
  int damage;
  int posX;
  int posY;
  LEVEL_POSITION* enemy_pos;
  double lastTime;
  double interval;
  GUN* currentGun;
  int cullDistanceX;
  int cullDistanceY;
} ENEMY;

/* create our enemy spawn struct that will hold all the information for each
   enemy spawn */
typedef struct
{
  char type;
  int health;
  int hpholder;
  int flashing;
  double startflashing;
  int posX;
  int posY;
  LEVEL_POSITION* lvl_pos;
  double lastTime;
  double interval;
} ENEMY_SPAWN;

/* Create a linked list to hold all the enemies */
typedef struct enemy_linklist
{
  ENEMY* data;
  struct enemy_linklist* next;
} ENEMY_LINKLIST;

/* Create a linked list to hold all the enemy spawns */
typedef struct enemy_spawn_linklist
{
  ENEMY_SPAWN* data;
  struct enemy_spawn_linklist* next;
} ENEMY_SPAWN_LINKLIST;

/* create our global head variables to use when creating enemies and spawns */
ENEMY_LINKLIST* head;
ENEMY_SPAWN_LINKLIST* head_spawn;

/* create current enemy counters */
int currentClawbots;
int currentJumpers;
int currentShooters;
int currentSnipers;
int currentbigburlys;

/* keep track of the number of total number of enemies destroyed */

/* create current enemy spawn counters */
int currentClawbotSpawns;
int currentJumperSpawns;
int currentShooterSpawns;
int currentSniperSpawns;
int currentBigBurlySpawns;

/* the total number of each type of enemy on the map */
int totalClawbots;
int totalJumpers;
int totalShooters;
int totalSnipers;
int totalbigburlys;

/* keep track of the total clawbot spawns (used for the tutorial level) */
int totalClawbotSpawns;

/* create a global flag to check when to delete the enemies */
int clearEnemiesFlag;

/* define our enemy spawn types */
ENEMY_SPAWN claw_spawn;
ENEMY_SPAWN jumper_spawn;
ENEMY_SPAWN shooter_spawn;
ENEMY_SPAWN sniper_spawn;
ENEMY_SPAWN bigburly_spawn;

/* define our enemy types */
ENEMY claw;
ENEMY jumper;
ENEMY shooter;
ENEMY sniper;
ENEMY bigburly;

/* *** prototypes *** */

/* the main function in EnemyManager.c */
int loadEnemies(void);

/* this wrapper function clears the board of all spawns and enemies */
void clearEnemies();

/* this function damages the enemies (should be called by bullets) */
void damageEnemy(ENEMY* cur_enemy, int ammount);

/* this function damages the spawns (should be called by bullets) */
void damageSpawn(ENEMY_SPAWN* cur_spawn, int ammount);

/* this function gets called directly from the main game loop */
ENEMY_SPAWN_LINKLIST* updateSpawnList();

/* this function gets called directly from the main game loop */
ENEMY_LINKLIST* updateEnemyList();

/* this function is called by updateSpawnList */
void updateSpawn();

/* this function is called by updateEnemyList */
void updateEnemy(ENEMY* enemy);

/* this function is called from the LevelManager */
ENEMY_SPAWN_LINKLIST* createSpawn(ENEMY_SPAWN* inputData, LEVEL_POSITION* lvl_pos, int x, int y);

/* this function is called from updateSpawn */
ENEMY_LINKLIST* createEnemy(ENEMY* inputData, LEVEL_POSITION* lvl_pos, int x, int y);

/* spawn memory copy (used by LevelManager) */
ENEMY_SPAWN* spawnCopy (ENEMY_SPAWN* spawn);

/* enemy memory copy (used by updateSpawn) */
ENEMY* enemyCopy (ENEMY* enemy);

/* used for the level 5 miniboss */
void createMauler(int x, int y);

#endif
