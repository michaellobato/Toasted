/*! ************************************************************************
  \file mauler.c
  \author Chris Howard
  \par email: choward@digipen.edu
  \par Game: Toasted
  \date Feburuary 2nd, 2007
  \brief Makes a miniboss -  the mauler, a bull like character who lines up
         a shot then charges, when he stops hit is backside to kill him
***************************************************************************/
#include "mauler.h"
#include "timer.h"
#include "player.h"
#include "effects.h"
#include <math.h>
#include <windows.h>
#include <time.h>
/* holds all the timers, positions, and the vector of the mauler */
static struct mauler
{
    ENEMY stats;
    LEVEL_POSITION * weakpt;
    LEVEL_POSITION * teeth;
    LEVEL_POSITION * counterteeth;
    int orient;
    double vecX;
    double vecY;
    int targX;
    int targY;
    int dead;
    int state;
    int counter;
    float vel;
    float centX;
    float centY;
    TIMER animtm;
    TIMER mvtm;
    int lastMoveTime;
    COORD tr, tl, br, bl;

}mauler;

static void update();
void moveMauler();
static void setVector(double x, double y);
static void animate();
void moveMauler();
int checkCol(float vX, float vY);
static void setColData();
void destroy();

/*! \brief makes one mauler, sets the initial time, gives all the
           attributes, and sets its position

	\param int x - x position to put the mauler at
  \param int y - y position to put the mauler at
 
  \sa  void createMauler(int x, int y)
*/
void createMauler(int x, int y)
{
    mauler.counterteeth = createLevelPosition(&mauler, POSITION_WEAPON, '\\');
    mauler.teeth = createLevelPosition(&mauler, POSITION_WEAPON, '/');
    mauler.stats.enemy_pos = createLevelPosition(&mauler, POSITION_WALL, 'Q');
    mauler.weakpt = createLevelPosition(&mauler, POSITION_ENEMY, 'q');
   
    mauler.lastMoveTime = (int)getCurrentTime();
    mauler.orient = 1;
    mauler.state = 1;
    mauler.dead = 0;
    mauler.counter = 0;

    mauler.stats.type = 'Q';
    mauler.stats.health = 10000;
    mauler.stats.damage = 1;

    /* this is the "center" of the mauler, where we base the vector 
       calculations off of
     */
    mauler.centX = x*1.0f;
    mauler.centY = y*1.0f;
    mauler.vel = 0.00f;

    mauler.stats.lastTime = 0;
    mauler.stats.interval = 0;
    setColData();
    update();
}


/*! \brief switches from \ to / on the teeth of the mauler

  \sa  void createMauler(int x, int y)
*/
static void animate()
{
    LEVEL_POSITION *holder;
    holder = mauler.teeth;
    mauler.teeth = mauler.counterteeth;
    mauler.counterteeth = holder;
}

/*! \brief makes one mauler, sets the initial time, gives all the
           attributes, and sets its position

	\param double x - x position to put the mauler at
  \param double y - y position to put the mauler at
 
  \sa  static void setVector(double x, double y)
*/
static void setVector(double x, double y)
{   
    mauler.vecX = (x/sqrt(x*x + y*y));
    mauler.vecY = (y/sqrt(x*x + y*y));
}

/*! \brief based on the actual vector between the player and the
           mauler, figgures out which direction the mauler should be facing
 
  \sa  static void findOrient()
*/
static void findOrient()
{
    double half = sqrt(2)/2;/* vector math for 45 degrees */

    /* sees which fourth of the unit circle the vector is, then compresses to 90 degrees */
    if(((mauler.vecX <= half) && (mauler.vecX >= -1 * half)) && (mauler.vecY > half))
    {
        mauler.orient = 3;
    }
    else if(((mauler.vecY <= half) && (mauler.vecY >= (-1 * half))) && (mauler.vecX < -1 * half))
    {
        mauler.orient = 2;
    }
    else if(((mauler.vecX <= half) && (mauler.vecX >= -1 * half)) && (mauler.vecY < -1 * half))
    {
        mauler.orient = 1;
    }
    else if(((mauler.vecY <= half) && (mauler.vecY >= -1 * half)) && (mauler.vecX > half))
    {
        mauler.orient = 4;
    }
}


/*! \brief based on where the "center" of the mauler is, figgures out where
           the corners of the mauler should be
 
  \sa  void setColData()
*/
static void setColData()
{
    mauler.tl.X = (short)mauler.centX - 3;
    mauler.tl.Y = (short)mauler.centY - 2;

    mauler.tr.X = (short)mauler.centX + 1;
    mauler.tr.Y = (short)mauler.centY - 2;

    mauler.bl.X = (short)mauler.centX - 3;
    mauler.bl.Y = (short)mauler.centY + 1;

    mauler.br.X = (short)mauler.centX + 1;
    mauler.br.Y = (short)mauler.centY + 1;
}

/*! \brief makes the actions of the mauler come to life, the mauler
           is either in the finding position state, charging state
           , dead state, or in the stuck against a wall state
 
  \sa  void moveMauler()
*/
void moveMauler()
{      
  if(checkCol((float)mauler.vecX*mauler.vel, (float)mauler.vecY*mauler.vel) == 3)
  {
    mauler.vel = 0;
    mauler.state = 4;
  }
  if((getCurrentTime() - mauler.lastMoveTime) > 2)
  {
      int i,j; /* counters */
      /* const float maxvel = 0.3f; the fastest the mauler can go */
      const float accel = .004f; /* the acceleration of the mauler */

      /* if dead, dont do anything */
      if(mauler.dead)
      {
          if(mauler.counter < 100)
          {
              if((mauler.counter %5) == 0)
              {
                explosion((int)mauler.centX+(rand()%4-2), (int)mauler.centY+(rand()%4-2), 2000, OWNER_ENEMY);
              }
          }

          mauler.counter++;
      }
      /* if the mauler is at 0, destroy it */
      else if(mauler.stats.health < 0)
      {
          mauler.dead= 1;
          mauler.counter = 0;
          setColData();
          for(i = 0; i< 4; i++)
          {
            for(j = 0; j < 5; j++)
            {

              /* set where it is as blank */
              setLevelPosition(ENEMY_LAYER, mauler.tl.X+j, mauler.tl.Y+i,whiteSpacePosition);
              setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i,whiteSpacePosition);
              setLevelPosition(WALL_LAYER, mauler.tl.X+j, mauler.tl.Y+i,whiteSpacePosition);
            }
          }
      }
      /* otherwise do normal mauler actions */
      else 
      {
        /* rotate the teeth */
        animate();
        for(i = 0; i< 4; i++)
        {
            for(j = 0; j < 5; j++)
            {
                /* set where it is as blank */
                setLevelPosition(ENEMY_LAYER, mauler.tl.X+j, mauler.tl.Y+i,
                                 whiteSpacePosition);
                setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i,
                                 whiteSpacePosition);
                setLevelPosition(WALL_LAYER, mauler.tl.X+j, mauler.tl.Y+i,
                                 whiteSpacePosition);
            }
        }
        /* make sure the collison data is updated */
        setColData();
        /* what it does next depends on it's state */
        switch(mauler.state)
        {
            /* if state 1 dont move, but get the vector between it and the player */
            case 1:
                setVector(getPlayerX() - mauler.centX, getPlayerY() - mauler.centY);
                findOrient();
                if(mauler.counter >= 300)
                {
                    mauler.vel = 0;
                    mauler.state = 2;
                    mauler.counter = 0;
                }
                break;
            /* if state 2 use the vector from state 1 to accelerate toward that area */
            case 2:
                /* this makes the mauler pick up speed as it goes along */
                  mauler.vel += accel;
                /* if we have hit a wall, go to hit wall state */
                if(checkCol((float)mauler.vecX*mauler.vel, (float)mauler.vecY*mauler.vel) == 3)
                {
                  mauler.vel = 0;
                  mauler.state = 4;
                }
                if((checkCol((float)mauler.vecX*mauler.vel, (float)mauler.vecY*mauler.vel) == 1) && (mauler.state != 4))
                {
                  mauler.counter = 0;
                  mauler.vel = 0;
                  mauler.state = 3;
                }
                else if ((mauler.state != 4))
                {
                  mauler.centX = (float)(mauler.centX + mauler.vecX*mauler.vel);
                  mauler.centY = (float)(mauler.centY + mauler.vecY*mauler.vel);
                }
                break;
            /* in case 3 we wait for 6 seconds before getting the vector again */
            case 3:
                    if(mauler.counter > 300)
                    {
                        mauler.state = 1;
                        mauler.counter = 0;
                    }
                break;
            /* in state four we chop up the player, and spray particles everywhere */
            case 4:
                /* explodes the player for extra fun */
                  expl_create(getPlayerX(),getPlayerY(),((mauler.orient-3) %2) * 0.1f + (rand()%20-10)*.01f, 
                             ((mauler.orient-2) %2) * 0.1f + (rand()%20-10)*.01f,200, 0, FOREGROUND_BLUE, OWNER_ENEMY);
                  player.stopped = 1;
                if(((int)getCurrentTime() % 3) == 0)
                {
                  player.hp -= 1;
                }
                break;
        }
        update();
        mauler.counter++;
      }
      mauler.lastMoveTime = (int)getCurrentTime();
  }
  /* recalculate the new position of the mauler */
  
}

/*! \brief removes the mauler's presence from the screen
 
  \sa  void destroy()
*/
void destroy()
{
  int i, j; /* loops through all the coordinates of the mauler and makes them empty */
    for(i = 0; i< 4; i++)
    {
        for(j = 0; j < 5; j++)
        {
            fillspace(mauler.tl.X+j, mauler.tl.Y+i);
        }
    }
}

/*! \brief tracks the next space the mauler is moving into and looks
           at the corners of the mauler to see if anything has been hit

	\param float vY - velocity in the y direction of the mauler
	\param float vX - velocity in the x direction of the mauler
	\return collision status of the mauler
 
  \sa  int checkCol(float vX, float vY)
*/
int checkCol(float vX, float vY)
{

    int tempX = 0 , tempY = 0;/* holder values */
    /* rounding calculations because the position and vectors for the mauler
       must be floating point, for accuracy but ultimately the position must be an int
     */
    if((getLevelPosition(WEAPON_LAYER, player.X, player.Y) == mauler.counterteeth) ||
       (getLevelPosition(WEAPON_LAYER, player.X, player.Y) == mauler.teeth) || 
       (getLevelPosition(WALL_LAYER, player.X, player.Y) == mauler.stats.enemy_pos))
    {
      mauler.state = 4;
      return 3;
    }
    if(vX > 0)
    {
        tempX = (int)(vX + 1);
    }
    else if(vX < 0)
    {
        tempX = (int)(vX - 1);
    }
    if(vY > 0)
    {
        tempY = (int)(vY + 1);
    }
    else if(vY < 0)
    {
        tempY = (int)(vY - 1);
    }
    /* if any  */
    if((isLevelPositionCollision(getCurrentLevel(),WALL_LAYER,mauler.tl.X+tempX,mauler.tl.Y+tempY)||
        isLevelPositionCollision(getCurrentLevel(),WALL_LAYER,mauler.tr.X+tempX,mauler.tr.Y+tempY)||
        isLevelPositionCollision(getCurrentLevel(),WALL_LAYER,mauler.bl.X+tempX,mauler.bl.Y+tempY)||
       isLevelPositionCollision(getCurrentLevel(),WALL_LAYER,mauler.br.X+tempX,mauler.br.Y+tempY)))
    {
        return 1;
    }
    /* sees if the player ended up hitting the teeth */
    return 0;
}

/*! \brief figgures out where the teeth and weakpoints of the mauler
           should be, fills the middle in with wall spaces
  \sa  static void update()
*/
static void update()
{
    int i, j;/* for counting through the mauler's coordinates */

    for(i = 0; i< 4; i++)
    {
        for(j = 0; j < 5; j++)
        {
            /* sees which way the mauler should be facing */
            switch(mauler.orient)
            {
                case 1:
                    /* if north set the top row to be teeth */
                    if(i == 0)
                    {
                        /* make every other tooth offset from each other to get the /\ effect */
                        if(j%2 == 0)
                        {
                            setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.teeth);
                        }
                        else
                        {
                            setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.counterteeth);
                        }
                    }
                    /* if it isnt teeth, maybe its the backside, set them as weakpoints */
                    else if((i == 3) && (j>0) && (j<4))
                    {
                      setLevelPosition(ENEMY_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.weakpt);
                    }
                    /* if its neither, fill it in with a wall space */
                    else
                    {
                      setLevelPosition(WALL_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.stats.enemy_pos);
                    }
                    break;
                    /* cases 2-4 are more or less the same thing */
                case 2:
                    if(j == 0)
                    {
                        if(i%2 == 0)
                        {
                            setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.teeth);
                        }
                        else
                        {
                            setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.counterteeth);
                        }
                    }
                    else if((j == 4) && (i>0) && (i<3))
                    {
                      setLevelPosition(ENEMY_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.weakpt);
                    }
                    else
                    {
                      setLevelPosition(WALL_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.stats.enemy_pos);
                    }
                    break;
                case 3:
                    if(i == 3)
                    {
                        if(j%2 == 0)
                        {
                            setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.teeth);
                        }
                        else
                        {
                            setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.counterteeth);
                        }
                    }
                    else if(i == 0 && (j>0) && (j<4))
                    {
                      setLevelPosition(ENEMY_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.weakpt);
                    }
                    else
                    {
                      setLevelPosition(WALL_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.stats.enemy_pos);
                    }
                    break;
                case 4:
                    if(j == 4)
                    {
                        if(i%2 == 0)
                        {
                            setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.teeth);
                        }
                        else
                        {
                            setLevelPosition(WEAPON_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.counterteeth);
                        }
                    }
                    else if(j == 0 && (i>0) && (i<3))
                    {
                      setLevelPosition(ENEMY_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.weakpt);
                    }
                    else
                    {
                      setLevelPosition(WALL_LAYER, mauler.tl.X+j, mauler.tl.Y+i, mauler.stats.enemy_pos);
                    }
                    break;
            }

        }
    }
}
