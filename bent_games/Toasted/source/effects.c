/*! ************************************************************************
\file effects.c
\author Chris Howard
\par email: choward@digipen.edu
\par Game: Toasted
\date Feburuary 10th, 2007
\brief A rudementary particle system to make explosions and other 
effects
***************************************************************************/

#include "effects.h"
#include "EnemyManager.h"
#include "player.h"
#include <math.h>
#include <stdio.h>
void expl_init()
{
	exp_head = expl_create(0, 0, 0, 0, -1,1, FOREGROUND_GREEN, OWNER_ENEMY);
}


/*! \brief makes one piece of an explosion, and adds it to a linked list
of explosion particles to be updated continually

\param int x - x position to put the explosion at
\param int y - y position to put the explosion at
\param int tx - x trajectory to set the explosion on
\param int ty - y trajectory to set the explosion on

\return The new element of the linked list that was created

\sa  EXPLOSION_LINKLIST* expl_create (int x, int y, float tx, float ty, int l)
*/
EXPLOSION_LINKLIST* expl_create (int x, int y, float tx, float ty, int l, int flag, WORD color, E_OWNER owner)
{
	EXPLOSION_LINKLIST *newexp = malloc(sizeof(EXPLOSION_LINKLIST)); /* the new particle to make */
	EXPLOSION_LINKLIST *counter = exp_head; /* the first particle in the list */

	/* if we can find the memory */
	if(newexp)
	{
		/* set all of the data for the new particl */
		newexp->next = NULL;
		newexp->data = malloc(sizeof(EXPLOSION));
		newexp->data->rand = 1;
		newexp->data->currchar = 1;
		newexp->data->count = 6;
		newexp->data->lastMove = (float)getCurrentTime();
		newexp->data->switcher = -1;
		newexp->data->life = l;
		newexp->data->trajX = tx;
		newexp->data->trajY = ty;
		newexp->data->Xpos = x*1.0f;
		newexp->data->Ypos = y*1.0f;
		newexp->data->flag = flag;
		newexp->data->time = getCurrentTime();
    newexp->data->tmr = getCurrentTime();
		newexp->data->appear = createLevelPosition(newexp->data,POSITION_WEAPON,'.');
		newexp->data->charinfo.Attributes = color;
		newexp->data->owner = owner;
		newexp->data->pwr = (float)sqrt(tx*tx + ty*ty);


		/* if its not the first element, then draw the particle on the screen */
		if (y != 0)
		{
			setLevelPosition(WEAPON_LAYER, x, y, newexp->data->appear);
		}
		/* go to the end of the linked list */
		if (counter != NULL)
		{
			while (counter->next != NULL)
			{
				counter = counter->next;
			}
			/* and link the last node in the list to the new one, making
			the new node the last one in the list*/
			counter->next = newexp;  
		}
	}
	return newexp;
}

void expl_destroy()
{
	EXPLOSION_LINKLIST *counter = exp_head; /* counts through the linked list */

	while (counter->next != NULL)
	{
		counter = counter->next;
		counter = expl_delete(counter);


	}
}
/*! \brief unlinks a given particle from the list, and then frees its
memory

\param EXPLOSION_LINKLIST *targ - the target to be deleted from the list


\return The element before the deleted one

\sa  EXPLOSION_LINKLIST* expl_delete(EXPLOSION_LINKLIST *targ)
*/
EXPLOSION_LINKLIST* expl_delete(EXPLOSION_LINKLIST *targ)
{
	EXPLOSION_LINKLIST *lookahead = exp_head; /* to get the element behind the element
											  to be deleted*/
	/* loops through the list till it finds the element to be deleted */

	if(targ->data->flag == 10)
	{
		explosion((int)(targ->data->Xpos - targ->data->trajX), (int)(targ->data->Ypos - targ->data->trajY), 500, OWNER_ENEMY);
	}
	if (lookahead != NULL)
	{
		while (lookahead->next != NULL && (lookahead->next != targ))
		{
			lookahead = lookahead->next;
		}
	}

	/* clears the position on the map of the target */
	setLevelPosition(WEAPON_LAYER, (int)targ->data->Xpos, (int)targ->data->Ypos, whiteSpacePosition);
	/* links the node behind the target with the node in front of the target */
	lookahead->next = targ->next;

	/* frees the memory assosiated with the target */
	free(targ->data->appear);
	free(targ->data);
	free(targ);

	return lookahead;

}

/*! \brief goes through all of the nodes in the linked list, and updates them so that
they are moved toward their trajectory or deleted
\sa  void expl_update()
*/
void expl_update()
{
	EXPLOSION_LINKLIST *counter = exp_head; /* counts through the linked list */
	if(counter->next != NULL)
	{
		counter = counter->next;
	}
	while ((counter != NULL) && counter != (exp_head))
	{
   if(getCurrentTime() > counter->data->tmr)
   {
		/* move the particle */
		expl_move(counter);
    counter->data->tmr = (int)getCurrentTime() + 1;
   }
		/* delete if past its life or the particle hits a wall */
		if((counter->data->life != -1) &&(((int)(getCurrentTime() - counter->data->time) >= counter->data->life) ||
			(isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, 
			(int)counter->data->Xpos + (int)counter->data->trajX, (int)counter->data->Ypos + (int)counter->data->trajX))))
		{
			counter = expl_delete(counter);
		}
		if(!isLevelPositionSpace(ENEMY_LAYER, (int)counter->data->Xpos, (int)counter->data->Ypos))
		{
			/* get the level position of the bullet */
			LEVEL_POSITION* lvlPos = getLevelPosition(ENEMY_LAYER, 
				(int)counter->data->Xpos, (int)counter->data->Ypos);

			/* if the type of data in that space is an enemy, do the following */
			if((counter->data->owner == OWNER_PLAYER || counter->data->owner == OWNER_NEITHER)  && 
         (lvlPos->type == POSITION_ENEMY))
			{
          /* makes sure particles cant hurt turrets */
        if(lvlPos->character == '!')
        {
        }
        else
        {
				    /* call the enemy damage function and delete the bullet */
			    	damageEnemy((ENEMY*)(lvlPos->data), 10);
				    counter = expl_delete(counter);
        }
			}
			else if((counter->data->owner == OWNER_PLAYER || counter->data->owner == OWNER_NEITHER)&& 
          (lvlPos->type == POSITION_SPAWN))
			{
				/* call the damage spawn function and delete the bullet */
				damageSpawn((ENEMY_SPAWN*)(lvlPos->data), 1);
				counter = expl_delete(counter);
			}
			else if((counter->data->owner == OWNER_ENEMY || counter->data->owner == OWNER_NEITHER)&& 
          (lvlPos->type == POSITION_PLAYER))
			{
				/* call the damage player function and delete the bullet */
				player.hp-=1;
				counter = expl_delete(counter);
			}
		}
		/* then move on */
		if(counter->next != NULL)
		{
			counter = counter->next;
		}

		else
		{
			break;
		}
	}
}

/*! \brief just deletes the old position of the particle, and updates its coordinates
then places it at the new coordinates

\param EXPLOSION_LINKLIST *mover - the particle to be moved

\sa  void expl_move(EXPLOSION_LINKLIST *mover)
*/
void expl_move(EXPLOSION_LINKLIST *mover)
{
	setLevelPosition(WEAPON_LAYER, (int)mover->data->Xpos, (int)mover->data->Ypos, whiteSpacePosition);
	mover->data->Xpos = mover->data->Xpos + mover->data->trajX;
	mover->data->Ypos = mover->data->Ypos + mover->data->trajY;
	setLevelPosition(WEAPON_LAYER, (int)mover->data->Xpos, (int)mover->data->Ypos,mover->data->appear);
}

/*! \brief converts a vector into degrees in the unit circle (should be a unit vector)

\param float tX - the x part of the vector
\param float tY - the y part of the vector

\return degrees (from 0 to 360) that the vector is at

\sa  float vecToDeg(float tX, float tY)
*/
float vecToDeg(float tX, float tY)
{
	if((tX>0) && (tY<0))
	{
		return (float)atan(tX/-tY)*(180/3.14f) + 270;
	}
	else if((tX<0) && (tY<0))
	{
		return (float)atan(tY/tX)*(180/3.14f) + 180;
	}
	else if((tX<0) && (tY>0))
	{
		return (float)atan(-tX/tY)*(180/3.14f) + 90;
	}
	else if((tX>0) && (tY>0))
	{
		return (float)atan(tY/tX)*(180/3.14f);
	}
	else 
		return 0;
}

/*! \brief makes it look as though the particles were rotating balls of wreckage
by setting the display character to \ then - then / and so on

\param LEVEL_POSITION* pos - the position of the particle to be moved

\return the character which should be displayed on the screen

\sa  char expl_anim(LEVEL_POSITION* pos)
*/
CHAR_INFO expl_anim(LEVEL_POSITION* pos)
{
	EXPLOSION *exp = pos->data;/* finds the right particle to rotate */
	/* sees where the particle is, then sets it to its respective
	correct place sequentially from the last*/
  /* rocket particle */
	if(exp->flag == 10)
	{
    float tX, tY, ang, realAng;
    /*converts vector to unit vector (length is 1)*/
    tX = exp->trajX;
    tY = exp->trajY;
		exp->trajX /= (float)sqrt(tX*tX + tY*tY);
		exp->trajY /= (float)sqrt(tX*tX + tY*tY);
    exp->trajX *= exp->pwr;
    exp->trajY *= exp->pwr;
		if((getCurrentTime() - exp->time) > 500)
		{
			float rotspeed = 10.0f/180;
      /* gives acceleration to rockets */
			if(exp->pwr <= .3f)
			{
				exp->pwr += .002f;
			}
      /* creates the rocket's "tail" */
			expl_create((int)exp->Xpos, (int)exp->Ypos, -exp->trajX*2, -exp->trajY*2, 100, 4, 0, OWNER_ENEMY);
      /* gets vector between player and the rocket */
			tX = player.X - exp->Xpos;
			tY = player.Y - exp->Ypos;
      /* degrees are easier for me to work with */
			ang = vecToDeg(tX, tY);
			realAng = vecToDeg(exp->trajX, exp->trajY); 
			realAng -= ang;
      /* compares the vector between the rocket and player to the vector the rocket is traveling on*/
			if (realAng > 180)
			{
				realAng -= 360;
			}
      /* rotates the rocket to the apropriate direction to be angled at the player */
			if(realAng < 0)
			{
              /*converts vector to unit vector (length is 1)*/
        tX = exp->trajX;
        tY = exp->trajY;
          /* the rocket can only turn so much, so dodging it is easy */
				exp->trajX = tX*(float)cos(rotspeed) - 
					tY*(float)sin(rotspeed);
				exp->trajY = tX*(float)sin(rotspeed) + 
					tY*(float)cos(rotspeed);
			}
			else
			{
        tX = exp->trajX;
        tY = exp->trajY;
				exp->trajX = tX*(float)cos(-rotspeed) - 
					tY*(float)sin(-rotspeed);
				exp->trajY = tX*(float)sin(-rotspeed) + 
					tY*(float)cos(-rotspeed);
			}
		}
	}
  /* "follower" particles */
	if(exp->flag == 9)
	{
		float tX, tY;
		tX = player.X - exp->Xpos;
		tY = player.Y - exp->Ypos;
    exp->trajX = tX;
    exp->trajY = tY;
    exp->trajX /= (float)sqrt(tX*tX + tY*tY);
    exp->trajY /= (float)sqrt(tX*tX + tY*tY);
    exp->trajX *= exp->pwr;
    exp->trajY *= exp->pwr;
	}
	if(exp->flag == 3)
	{
      /* makes the particle start as yellow, and after it is a certain distance turn red */
		if((int)(getCurrentTime() - exp->time) <= exp->life/3)
		{

			exp->charinfo.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
		else
		{

			exp->charinfo.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;

		}
	}
  /* makes the spinning animation */
	if(exp->flag < 4)
	{
		switch(exp->currchar)
		{
        /* goes to -, then /, then |, then \ and so forth */
		case 1:
			exp->currchar = 2;
			exp->charinfo.Char.AsciiChar = '-';
			break;
		case 2:
			exp->currchar = 3;
			exp->charinfo.Char.AsciiChar =  '/';
			break;
		case 3:
			exp->currchar = 4;
			exp->charinfo.Char.AsciiChar =  '|';
			break;
		case 4:
			exp->currchar = 1;
			exp->charinfo.Char.AsciiChar =  '\\';
			break;
		default:
        /* this tells me I have a problem */
			exp->charinfo.Char.AsciiChar =  '<';
			break;
		}
	}
	else
  {
      /* unitize the vector */
		float tX = exp->trajX;
		float tY = exp->trajY;
    float ang;
    exp->trajX /=(float)sqrt(tX*tX + tY*tY);
    exp->trajY /=(float)sqrt(tX*tX + tY*tY);

    /* sets the animation of the particle to correspond with its
       vector pos*/
    ang = vecToDeg(exp->trajX, exp->trajY);
		if(ang < 30)
		{
			exp->charinfo.Char.AsciiChar =  '-';
		}
    else if(ang < 60)
    {
        exp->charinfo.Char.AsciiChar =  '\\';
    }
		else if(ang < 120)
		{
			exp->charinfo.Char.AsciiChar =  '|';
		}
		else if(ang < 150)
		{
			exp->charinfo.Char.AsciiChar =  '/';
		}
		else if(ang < 210)
		{
			exp->charinfo.Char.AsciiChar =  '-';
		}
    else if(ang < 240)
		{
			exp->charinfo.Char.AsciiChar =  '\\';
		}
    else if(ang < 300)
		{
			exp->charinfo.Char.AsciiChar =  '|';
		}
    else if(ang < 330)
		{
			exp->charinfo.Char.AsciiChar =  '/';
		}
    else
    {
      exp->charinfo.Char.AsciiChar =  '-';
    }
    /* fire */
		if(exp->flag == 4)
		{
			float rotspeed = 3.14f/180;
			int random_number = (rand() % 5 - 2) * 10;
      tX = exp->trajX;
      tY = exp->trajY;
			exp->trajX = tX*(float)cos(rotspeed*random_number) - tY*(float)sin(rotspeed*random_number);
			exp->trajY = tX*(float)sin(rotspeed*random_number) + tY*(float)cos(rotspeed*random_number);
			if((int)(getCurrentTime() - exp->time) <= (exp->life/4))
			{

				exp->charinfo.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			}
			else
			{
				exp->charinfo.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
			}
		}
    /* blizzard */
		if(exp->flag == 6)
		{
			if(exp->count >= 10)
			{
				exp->switcher *= -1;
				exp->count = 0;
			}
      tX = exp->trajX;
      tY = exp->trajY;
			exp->trajX = -(float)sin((31.4/180)*exp->switcher+((3.14/180)*rand()))*tY + 
				(float)cos((31.4/180)*exp->switcher+((3.14/180)*0))*tX;
			exp->trajY =  (float)cos((31.4/180)*exp->switcher+((3.14/180)*0))*tY + 
				(float)sin((31.4/180)*exp->switcher+((3.14/180)*0))*tX;
			exp->count++;
		}
    /* lightning */
		if(exp->flag == 7)
		{
			if(exp->count >= 10)
			{
				exp->switcher *= -1;
				exp->count = 0;
			}
      tX = exp->trajX;
      tY = exp->trajY;
			exp->trajX = -(float)sin((31.4/180)*exp->switcher+((3.14/180)*exp->rand))*tY + 
				(float)cos((31.4/180)*exp->switcher+((3.14/180)*exp->rand))*tX;
			exp->trajY =  (float)cos((31.4/180)*exp->switcher+((3.14/180)*exp->rand))*tY + 
				(float)sin((31.4/180)*exp->switcher+((3.14/180)*exp->rand))*tX;
			exp->count++;
		}
    /* flamethrower */
		if(exp->flag == 8)
		{
			if(exp->count >= 10)
			{ 
				exp->switcher *= -1;
				exp->count = 0;
			}
			exp->trajX = -(float)sin((31.4/180)*exp->switcher*(rand()%5))*exp->trajY;
			exp->count++;
    }
     /* flamethrower */
		if(exp->flag == 11)
		{
			if(exp->count >= 10)
			{ 
				exp->switcher *= -1;
				exp->count = 0;
			}
			exp->trajY = (float)sin((31.4/180)*exp->switcher*(rand()%5))*exp->trajX;
			exp->count++;
    }
    exp->trajX *= exp->pwr;
    exp->trajY *= exp->pwr;
	}
	return exp->charinfo;
}
void setRand(EXPLOSION_LINKLIST * exp, float r)
{
	exp->data->rand = r;
}

/*! \brief A convenience function to make a bunch of explosion particles come out
in a circle from the coordinates

\param int x - the x origin of the explosion
int y - the y origin of the explosion
int l - the life of the explosion


\sa  void explosion (int x, int y, int l)
*/
void explosion (int x, int y, int l, E_OWNER owner)
{
	const float velocity1 = .4f, velocity2 = (float)sqrt(2)/2 *.4f;
	const WORD color = FOREGROUND_RED | FOREGROUND_INTENSITY;
	/* uses the unit circle to set an explosion particle off at every 45 degrees */

	expl_create(x, y, velocity1*.8f, 0.0f, l*2, 4, color, owner);
	expl_create(x, y, -velocity1*.8f, 0.0f, l*2, 4, color, owner);
	expl_create(x, y, 0.0f, -velocity1*.8f, l*2, 4, color, owner);
	expl_create(x, y, 0.0f, velocity1*.8f, l*2, 4, color, owner);

	expl_create(x, y, velocity2*.8f, velocity2*.8f, l*2, 4, color, owner);
	expl_create(x, y, -velocity2*.8f, velocity2*.8f, l*2, 4, color, owner);
	expl_create(x, y, velocity2*.8f, -velocity2*.8f, l*2, 4, color, owner);
	expl_create(x, y, -velocity2*.8f, -velocity2*.8f, l*2, 4, color, owner);

	expl_create(x, y, velocity1*.5f, 0.0f, l*2, 3, color, owner);
	expl_create(x, y, -velocity1*.5f, 0.0f, l*2, 3, color, owner);
	expl_create(x, y, 0.0f, -velocity1*.5f, l*2, 3, color, owner);
	expl_create(x, y, 0.0f, velocity1*.5f, l*2, 3, color, owner);

	expl_create(x, y, velocity2*.5f, velocity2*.5f, l*2, 3, color, owner);
	expl_create(x, y, -velocity2*.5f, velocity2*.5f, l*2, 3, color, owner);
	expl_create(x, y, velocity2*.5f, -velocity2*.5f, l*2, 3, color, owner);
	expl_create(x, y, -velocity2*.5f, -velocity2*.5f, l*2, 3, color, owner);


}
