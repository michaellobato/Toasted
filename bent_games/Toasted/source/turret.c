/*! ************************************************************************
  \file turret.c
  \author Chris Howard
  \par email: choward@digipen.edu
  \par Game: Toasted
  \date Feburuary 6th, 2007
  \brief  creates a linked list of turrets, a static enemy that shoots at
          the player, can be placed directly on the map (using a "!") or
          can be spawned by a boss (like the lvl 3 boss)
***************************************************************************/
#include "turret.h"
#include "player.h"
#include "effects.h"
#include <math.h>
void turret_init()
{
  turret_head = turret_create(0, 0, 1, 1, 1);
}


TURRET_LINKLIST* turret_create (int x, int y, int hp, int rng, int flag)
{
  TURRET_LINKLIST *newturr = malloc(sizeof(TURRET_LINKLIST)); /* the new particle to make */
  TURRET_LINKLIST *counter = turret_head; /* the first particle in the list */

  if(newturr)
  {
    newturr->next = NULL;
    newturr->data = malloc(sizeof(TURRET));
    newturr->data->stats = malloc(sizeof(ENEMY));
    newturr->data->Xpos = x;
    newturr->data->Ypos = y;
    newturr->data->stats->lastTime = getCurrentTime();
    newturr->data->stats->interval = 0;
    newturr->data->stats->health = hp;
    newturr->data->range = rng;
    newturr->data->flag = flag;

    newturr->data->stats->currentGun = gunCopy(&weaponSubMachineGun);
	  newturr->data->stats->currentGun->owner = OWNER_ENEMY;
	  newturr->data->stats->currentGun->range = 1400;
	  newturr->data->stats->currentGun->damage = 5;

    newturr->data->stats->enemy_pos = createLevelPosition(newturr->data->stats,POSITION_ENEMY,'!');
    if (counter != NULL)
    {
      while (counter->next != NULL)
      {
        counter = counter->next;
      }
      counter->next = newturr;  
    }
  }


  return newturr;
}
TURRET_LINKLIST* turret_delete(TURRET_LINKLIST *targ)
{
  TURRET_LINKLIST *lookahead = turret_head; /* to get the element behind the element
                                               to be deleted*/
  /* loops through the list till it finds the element to be deleted */
  if (lookahead != NULL)
  {
      while (lookahead->next != NULL && (lookahead->next != targ))
      {
        lookahead = lookahead->next;
      }
  }

  /* clears the position on the map of the target */
	setLevelPosition(ENEMY_LAYER, (int)targ->data->Xpos, (int)targ->data->Ypos, whiteSpacePosition);
  /* links the node behind the target with the node in front of the target */
  lookahead->next = targ->next;

  /* frees the memory assosiated with the target */
  free(targ->data->stats->enemy_pos);
  free(targ->data->stats);
  free(targ->data);
  free(targ);

  return lookahead;
  
}
void turret_fire()
{


  int time = (int)getCurrentTime();
  int difftime;


  TURRET_LINKLIST *counter = turret_head; /* the first particle in the list */
  if (counter != NULL)
  {
    while (counter->next != NULL)
    {
      counter = counter->next;
      if(counter->data->stats->health <=0)
      {
          counter = turret_delete(counter);
      }
      /* increment the counter to the next turret */


	    setLevelPosition(ENEMY_LAYER, (int)counter->data->Xpos, 
                                    (int)counter->data->Ypos, counter->data->stats->enemy_pos);

      /* find how much time has passed since the last time the turret shot */
      difftime = time - (int)counter->data->stats->lastTime;



        /* now offset by a small ammount depending on the time
           (creates that spread look)
        */
        if (counter->data->flag == 1)
        {  
          if(counter->data->stats->interval > 300)
          {
            if(((int)counter->data->stats->interval % 10) == 0)
            {
              float x;
              float y;
              x = counter->data->Xvec = ((float)getPlayerX() - (float)counter->data->Xpos);
              y = counter->data->Yvec = ((float)getPlayerY() - (float)counter->data->Ypos);
              
              counter->data->Xvec /= (float)sqrt(x*x + y*y);
              counter->data->Yvec /= (float)sqrt(x*x + y*y);
              /* creates that "spread effect by rotating the vector which the particles are fired */
              x = counter->data->Xvec;
              y = counter->data->Yvec;
              counter->data->Xvec = x*(float)cos((320-counter->data->stats->interval)*(3.14f/180)) - 
                                    y*(float)sin((320-counter->data->stats->interval)*(3.14f/180));
              counter->data->Yvec = x*(float)sin((320-counter->data->stats->interval)*(3.14f/180)) + 
                                    y*(float)cos((320-counter->data->stats->interval)*(3.14f/180));
              
              expl_create(counter->data->Xpos, counter->data->Ypos, counter->data->Xvec*.1f,
                     counter->data->Yvec*.1f,counter->data->range, 1, FOREGROUND_BLUE | FOREGROUND_GREEN |
				          FOREGROUND_INTENSITY, OWNER_ENEMY);
            }

            if(counter->data->stats->interval >= 340)
            {
                counter->data->stats->lastTime = time;
                counter->data->stats->interval = 0;
            }
          }
        }
        /* up fire turrets */
        else if (counter->data->flag == 2)
        {
            expl_create(counter->data->Xpos, counter->data->Ypos+1, 0,
                   .1f,counter->data->range, 8, FOREGROUND_GREEN | FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
            expl_create(counter->data->Xpos, counter->data->Ypos+1, 0,
                   .04f,counter->data->range, 8, FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
        }
        /* down fire turrets */
        else if(counter->data->flag == 3)
        {
          if(counter->data->stats->interval > 200)
          {
            expl_create(counter->data->Xpos, counter->data->Ypos+1, 0,
                   -.1f,counter->data->range, 8, FOREGROUND_GREEN | FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
            expl_create(counter->data->Xpos, counter->data->Ypos+1, 0,
                   -.04f,counter->data->range, 8, FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
          }
          if(counter->data->stats->interval > 500)
          {
            counter->data->stats->interval = 0;
            counter->data->stats->lastTime = time;
          }
        }
        /*left fire turrets*/
        else if(counter->data->flag == 4)
        {
          if(counter->data->stats->interval > 400)
          {
            expl_create(counter->data->Xpos+1, counter->data->Ypos, .4f,
                   0,counter->data->range, 11, FOREGROUND_GREEN | FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
            expl_create(counter->data->Xpos+1, counter->data->Ypos, .3f,
                   0,counter->data->range, 11, FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
          }
          if(counter->data->stats->interval > 900)
          {
            counter->data->stats->interval = 0;
            counter->data->stats->lastTime = time;
          }
        }
        /* right fire turrets */
        else if(counter->data->flag == 5)
        {
          if(counter->data->stats->interval > 600)
          {
            expl_create(counter->data->Xpos-1, counter->data->Ypos, -.4f,
                   0,counter->data->range, 11, FOREGROUND_GREEN | FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
            expl_create(counter->data->Xpos-1, counter->data->Ypos, -.3f,
                   0,counter->data->range, 11, FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
          }
          if(counter->data->stats->interval > 1200)
          {
            counter->data->stats->interval = 0;
            counter->data->stats->lastTime = time;
          }
        }
        /*  */
        else if(counter->data->flag == 6)
        {
          float x = player.X*1.0f - counter->data->Xpos;
          float y = player.Y*1.0f - counter->data->Ypos;
          float hx = x;
          x = x/(float)sqrt(hx*hx + y*y);
          y = y/(float)sqrt(hx*hx + y*y);
          if(counter->data->stats->interval > 800)
          {
            expl_create(counter->data->Xpos, counter->data->Ypos+1, x*.1f,
                   y*.1f,8000, 10, FOREGROUND_GREEN | FOREGROUND_RED |
				    FOREGROUND_INTENSITY, OWNER_NEITHER);
          }
          if(counter->data->stats->interval > 801)
          {
            counter->data->stats->interval = 0;
            counter->data->stats->lastTime = time;
          }
        }
        else if(counter->data->flag == 7)
        {
          if(((int)counter->data->stats->interval % 10)  == 0)
          {
            float x = (float)player.X - counter->data->Xpos;
            float y = (float)player.Y - counter->data->Ypos;
            expl_create(counter->data->Xpos+(int)(x/sqrt(x*x + y*y)), counter->data->Ypos+(int)(y/sqrt(x*x + y*y)), 
                (float)(x/sqrt(x*x + y*y))*.1f,(float)(y/sqrt(x*x + y*y))*.1f,counter->data->range, 9, 
                FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, OWNER_NEITHER);
          }
        }
       counter->data->stats->interval++;
      }
    }
}


void turret_delete_list()
{
  TURRET_LINKLIST *counter = turret_head; /* the first particle in the list */
  if (counter != NULL)
  {
    while (counter->next != NULL)
    {
        counter = counter->next;
        counter = turret_delete(counter);
    }
  }
}
