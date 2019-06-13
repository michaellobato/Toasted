/*! ************************************************************************
\file LevelFive.C
\author Chris Howard
\par email: choward@digipen.edu
\par Game: Toasted
\date Febuary 14th, 2007
\brief

Contains all the actions for level five, specifically the main boss

***************************************************************************/

#include "LevelFive.h"
#include "timer.h"
#include "player.h"
#include "effects.h"
#include "turret.h"
#include <stdlib.h>
#include <time.h>

typedef struct bossarms
{
	int aim;
	LEVEL_POSITION *drawW;
	LEVEL_POSITION *drawE;
	ENEMY *stats;
	CHAR_INFO frame[BOSS_HEIGHT][BOSS_WIDTH];
	int alive;
	int lastFrameTime;
	int counter;
}ARM;

typedef struct hand
{
	int centerX;
	int centerY;
	float offsetX;
	float offsetY;
	int counter;
	int width;
	int height;
	LEVEL_POSITION *drawW;
	LEVEL_POSITION *drawE;
	ENEMY *stats;
	CHAR_INFO frame[BOSS_HEIGHT][BOSS_WIDTH];
	int alive;
}HAND;

struct mainBoss
{
	int center;
	ARM left;
	int hpHolder;
	HAND leftH;
	ARM right;
	HAND rightH;
	LEVEL_POSITION *drawW;
	LEVEL_POSITION *drawE;
	ENEMY *stats;
	CHAR_INFO frame[BOSS_HEIGHT][BOSS_WIDTH];
	int alive;
	int state;
	int counter;
	int lastMoveTime;
}mainboss;

void loadMainBoss(int xPos, int yPos)
{
    /* sets base stats for the left arm */
  mainboss.left.alive = 1;
  mainboss.left.stats = malloc(sizeof(ENEMY));
  mainboss.left.stats->posX = xPos - 30;
  mainboss.left.stats->posY = yPos - 13;
  mainboss.left.stats->health = 1000000;
  mainboss.left.counter = 0;
  LarmFindPlyr();

  /* sets base stats for the right arm */
  mainboss.right.alive = 1;
  mainboss.right.stats = malloc(sizeof(ENEMY));
  mainboss.right.stats->posX = xPos + 30;
  mainboss.right.stats->posY = yPos - 13;
  mainboss.right.stats->health = 1000000;
  mainboss.right.counter = 0;
  RarmFindPlyr();


  mainboss.stats = malloc(sizeof(ENEMY));
  mainboss.stats->posX = xPos;
  mainboss.stats->posY = yPos;
  mainboss.center = xPos + 30;
  mainboss.counter = 0;
  mainboss.stats->health = 14000000;
  mainboss.hpHolder = 14000000;
  mainboss.alive = 1;
  mainboss.state = 1;


	mainboss.lastMoveTime = 0;

  /* sets base stats for the right arm */
	mainboss.leftH.stats = malloc(sizeof(ENEMY));
	mainboss.leftH.stats->interval = 6;
	mainboss.leftH.stats->health = 9000000;
	mainboss.leftH.width = 20;
	mainboss.leftH.height = 19;
	mainboss.leftH.alive = 1;
	mainboss.leftH.offsetX = 0;
	mainboss.leftH.offsetY = 0;
	LhandGetPos();

  /* sets base stats for the right arm */
	mainboss.rightH.stats = malloc(sizeof(ENEMY));
	mainboss.rightH.stats->interval = 6;
	mainboss.rightH.stats->health = 9000000;
	mainboss.rightH.width = 20;
	mainboss.rightH.height = 19;
	mainboss.rightH.alive = 1;
	mainboss.rightH.offsetX = 0;
	mainboss.rightH.offsetY = 0;
	RhandGetPos();

  /* sets unique identifiers for all of the pieces of the main boss */
	mainboss.drawW = createLevelPosition(NULL, POSITION_WALL, '$');
	mainboss.left.drawW = createLevelPosition(mainboss.left.stats, POSITION_WALL, 'L');
	mainboss.right.drawW = createLevelPosition(NULL, POSITION_WALL, '~');
	mainboss.leftH.drawW = createLevelPosition(NULL, POSITION_WALL, 'l');
	mainboss.rightH.drawW = createLevelPosition(NULL, POSITION_WALL, '`');
	mainboss.drawE = createLevelPosition(mainboss.stats, POSITION_ENEMY, '$');
	mainboss.left.drawE = createLevelPosition(mainboss.left.stats, POSITION_ENEMY, 'L');
	mainboss.right.drawE = createLevelPosition(mainboss.right.stats, POSITION_ENEMY, '~');
	mainboss.leftH.drawE = createLevelPosition(mainboss.leftH.stats, POSITION_ENEMY, 'l');
	mainboss.rightH.drawE = createLevelPosition(mainboss.rightH.stats, POSITION_ENEMY, '`');
	loadMainBossChars("bosses/armR.txt","bosses/armRC.txt",mainboss.right.frame);
	loadMainBossChars("bosses/mainboss.txt","bosses/mainbossC.txt",mainboss.frame);
	loadMainBossChars("bosses/armL.txt","bosses/armLC.txt",mainboss.left.frame);
	loadMainBossChars("bosses/hand.txt","bosses/handC.txt",mainboss.rightH.frame);
	loadMainBossChars("bosses/hand.txt","bosses/handC.txt",mainboss.leftH.frame);
}


int mainbossX()
{
    return mainboss.stats->posX;
}
int mainbossY()
{
    return mainboss.stats->posY;
}

int LarmX()
{
    return mainboss.left.stats->posX;
}
int LarmY()
{
    return mainboss.left.stats->posY;
}
int RarmX()
{
    return mainboss.right.stats->posX;
}
int RarmY()
{
    return mainboss.right.stats->posY;
}
int LhandX()
{
    return mainboss.leftH.stats->posX;
}
int LhandY()
{
    return mainboss.leftH.stats->posY;
}
int RhandX()
{
    return mainboss.rightH.stats->posX;
}
int RhandY()
{
    return mainboss.rightH.stats->posY;
}

void LhandGetPos()
{
  /* calculates the position of the left hand based on where the control point for the arm is
     at first the control point is bound to the arm the hand is attached to, then the control point
     is directly controlling the position of the hand */
	mainboss.leftH.centerX = mainboss.left.aim;
	mainboss.leftH.stats->posX = mainboss.leftH.centerX - 10;
	mainboss.leftH.stats->posY = (int)(mainboss.left.stats->posY - mainboss.leftH.stats->interval);
}

void RhandGetPos()
{
  /* calculates the position of the left hand based on where the control point for the arm is
     at first the control point is bound to the arm the hand is attached to, then the control point
     is directly controlling the position of the hand */
	mainboss.rightH.centerX = mainboss.right.aim;
	mainboss.rightH.stats->posX = mainboss.rightH.centerX - 10;
	mainboss.rightH.stats->posY = (int)(mainboss.right.stats->posY - mainboss.rightH.stats->interval);
}

void RarmFindPlyr()
{
  /* positions the right arm directly under the player if the player is within the 
     correct region for the boss*/
	mainboss.right.aim = mainboss.right.stats->posX + 64;
	if(getPlayerX() < mainboss.right.aim)
	{
		if((mainboss.center - mainboss.right.aim) < -40)
		{
			mainboss.right.stats->posX --;
		}

	}
	else if(getPlayerX() > mainboss.right.aim)
	{
		if(!isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, mainboss.rightH.stats->posX+mainboss.rightH.width+1, mainboss.rightH.stats->posY-1))
		{
			mainboss.right.stats->posX ++;
		}
	}

}

void LarmFindPlyr()
{
      /* positions the left arm directly under the player if the player is within the 
     correct region for the boss*/
	mainboss.left.aim = mainboss.left.stats->posX + 6;
	if(getPlayerX() > mainboss.left.aim)
	{
		if((mainboss.center - mainboss.left.aim) > 40)
		{
			mainboss.left.stats->posX ++;
		}
	}
	else if(getPlayerX() < mainboss.left.aim)
	{
		if(!isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, mainboss.leftH.stats->posX-1, mainboss.leftH.stats->posY-1))
		{
			mainboss.left.stats->posX --;
		}
	}
}

void clearLeftArm()
{
    /* clears the left arm */
	int i,j;
	for(i = 0; i < BOSS_HEIGHT; i++)
	{
		/* read in all the characters in the line and store their characer values in the array */
		for(j = 0; j < BOSS_WIDTH; j++)
		{

			mainboss.left.frame[i][j].Char.AsciiChar = 'X';
			mainboss.left.frame[i][j].Attributes = (BACKGROUND_GREEN);
		}
	}
	for(i = 0; i < BOSS_HEIGHT; i++)
	{
		for(j = 0; j < BOSS_WIDTH; j++)
		{
			if(mainboss.left.frame[i][j].Char.AsciiChar != ' ')
			{
				setLevelPosition(ENEMY_LAYER, mainboss.left.stats->posX + j, mainboss.left.stats->posY + i, 
					whiteSpacePosition);
				setLevelPosition(WALL_LAYER, mainboss.left.stats->posX + j, mainboss.left.stats->posY + i, 
					whiteSpacePosition);
			}
		}
	}
}

/* I have to use right and left versions of a lot of functions because their animations
   are technically unique (inverted from each other, but due to certain nuances in the
   animations, they cannot be directly put into the same function and flipped)*/
void clearRightArm()
{
    /* clears the right arm */
	int i,j;
	for(i = 0; i < BOSS_HEIGHT; i++)
	{
		/* read in all the characters in the line and store their characer values in the array */
		for(j = 0; j < BOSS_WIDTH; j++)
		{

			mainboss.right.frame[i][j].Char.AsciiChar = 'X';
			mainboss.right.frame[i][j].Attributes = (BACKGROUND_GREEN);
		}
	}
	for(i = 0; i < BOSS_HEIGHT; i++)
	{
		for(j = 0; j < BOSS_WIDTH; j++)
		{
			if(mainboss.right.frame[i][j].Char.AsciiChar != ' ')
			{
				setLevelPosition(ENEMY_LAYER, mainboss.right.stats->posX + j, mainboss.right.stats->posY + i, 
					whiteSpacePosition);
				setLevelPosition(WALL_LAYER, mainboss.right.stats->posX + j, mainboss.right.stats->posY + i, 
					whiteSpacePosition);
			}
		}
	}
}

int LVL_5(int state)
{
    /* does not spawn the boss until the player gets to a certain point */
	if(getPlayerY() > 280)
	{
		updateMainBoss();
    player.atLevelFiveBoss = 1;
	}
  /* goes to the victory screen when the boss dies */
	if (!mainboss.alive)
  {
    player.atLevelFiveBoss = 0;
    player.levelFiveComplete = 1;
		return STATE_VICTORY;
  }
	else
  {
		return state;
  }
}


/* this decides what the arm does in a given point in time */
void armActions(ARM *arm, HAND *hand, int osX, int osY, int dir)
{
  int i;/* counter variable */
  if(arm->alive != 0)
  {
    /* clear the arms and hands so we can redraw them in the animation loop */
    clearMainBoss(arm->stats, arm->frame);
    clearMainBoss(hand->stats, hand->frame);
    /* this is the acquiring player phase */
    if((arm->counter < 150) && (arm->counter % 5 == 0))
    {
      /* figgure out what the arms are supposed to do */
      if(arm->frame == mainboss.left.frame)
      {
        LarmFindPlyr();
      }
      else
      {
        RarmFindPlyr();
      }
    }
    /* this is the winding for the punch phase */
    else if(arm->counter < 460)
    {
      /* stops motion of the hand and makes it move backwards */
      if (arm->counter < 190)
      {
        if(arm->counter % 10 == 0)
        {
          hand->stats->interval -= 1;
        }
      }
      /* punching phase */
      else
      {
        /* punches forward until the hand hits something */
        if(!(isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+2, hand->stats->posY-1) || 
             isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+18, hand->stats->posY-1)))
        {
          hand->stats->interval += .8f;
        }
      }
    }
    /* reset phase */
    else
    {
       /* the hand comes back and readies for another punch */
      if(arm->counter % 5 == 0)
      {
        if(hand->stats->interval > 6)
        {
          hand->stats->interval -= 1;
        }
        else
        {
          hand->stats->interval = 6;
          arm->counter = 0;
        }
      }
    }
    /* now the hands can figgure out what to do based on data from previous parts */
    if(mainboss.left.frame == arm->frame)
    {
      LhandGetPos();
    }
    else
    {
      RhandGetPos();
    }
    arm->counter++;
    /* draw the new positions of the arms and hands */
		drawMainBoss(arm->stats, arm->frame, arm->drawW, arm->drawE);
    drawMainBoss(hand->stats, hand->frame, hand->drawW, hand->drawE);
  }
  else
  {
    /* this is the phase where the arm is gone, but the hand remains */
    if(hand->alive)
    {
      /* we have to draw a new horizontal hand so we clear the verticle hand and explode the arm */
      clearMainBoss(hand->stats, hand->frame);
      if(arm->counter < 100)
      {
        if((arm->counter % 10) == 0)
        {
          hand->stats->posY++;
        }
        if(arm->counter % ((141-arm->counter)/20) == 0)
        {
          if(arm->stats->posX == mainboss.left.stats->posX)
          {
            explosion(arm->stats->posX+rand()%30-3, arm->stats->posY+10+rand()%20, 1000, OWNER_PLAYER);
          }
          else 
          {
            explosion(arm->stats->posX+rand()%30+40, arm->stats->posY+10+rand()%20, 1000, OWNER_PLAYER);
          }
        }
        arm->counter ++;
        /* load the sideways hand */
        if(arm->counter == 100)
        {
          hand->width = 33;
          hand->height = 13;
          if(arm->frame == mainboss.left.frame)
          {
            clearLeftArm();
            loadMainBossChars("bosses/handsideways.txt","bosses/handsidewaysC.txt",hand->frame);
          }
          else
          {
            clearRightArm();
            loadMainBossChars("bosses/handsidewaysR.txt","bosses/handsidewaysRC.txt",hand->frame);
          }
          for(i = 0; i < 20; i++)
          {          
            if(arm->stats->posX == mainboss.left.stats->posX)
            {
              explosion(arm->stats->posX+rand()%30-3, arm->stats->posY+10+rand()%20, 1000, OWNER_PLAYER);
            }
            else 
            {
              explosion(arm->stats->posX+rand()%30+40, arm->stats->posY+10+rand()%20, 1000, OWNER_PLAYER);
            }
          }
          clearMainBoss(arm->stats, arm->frame);
        }
      }
      /* turn non destructable parts of the hand into destructable parts, so that the hand can
         now be destroyed*/
      else if(arm->counter < 133)
      {
        for (i = 0; i < 14; i++)
        {
          if(hand->frame[i][arm->counter-100].Char.AsciiChar == 'A')
          {
            hand->frame[i][arm->counter-100].Char.AsciiChar = 'X';
          }
          if(hand->frame[i][arm->counter-100].Attributes == (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN))
          {
            hand->frame[i][arm->counter-100].Attributes = (FOREGROUND_RED | FOREGROUND_INTENSITY);
          }
          arm->stats->posX = hand->stats->posX;
          arm->stats->posY = hand->stats->posY;
        }
        arm->counter++;
        hand->counter = 0;
        hand->stats->interval = 0;
      }
      else
      {
          /* this is the acquiring player phase */
        if(hand->counter < 170)
        {
          hand->centerX = (int)hand->stats->posX+osX;
          hand->centerY = hand->stats->posY+osY;
          if((hand->centerX > getPlayerX()) && ((hand->centerX +47- getPlayerX()) > 4) &&
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX-1, hand->stats->posY)&& 
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX-1, hand->stats->posY+13))
          {
              /* the hand moves quicker in the x direction than in the y direction, making it easier for it 
                 to position itself correctly, with the fist facing the player, directly to the left or right
                 of the player*/
            hand->offsetX -= .4f;
          }
          else if(hand->centerX < getPlayerX() && ((hand->centerX+47 - getPlayerX()) > 4) &&
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+34, hand->stats->posY)&& 
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+34,hand->stats->posY+13))
          {
              /* the hands cannot possibly hit any wall layer */
            hand->offsetX += .4f;
          }
          if(hand->centerY > getPlayerY()&& 
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX, hand->stats->posY-1)&& 
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+33, hand->stats->posY-1))
          {
            hand->offsetY -= .1f;
          }
          else if(hand->centerY < getPlayerY()&& 
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX, hand->stats->posY+14)&& 
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+33,hand->stats->posY+14))
          {
            hand->offsetY += .1f;
          }
          hand->stats->posX = (int)(arm->stats->posX + hand->offsetX);
          hand->stats->posY = (int)(arm->stats->posY + hand->offsetY);
          hand->stats->interval = 0;
        }
        /* this is the powering up phase of the punch */
        else if(hand->counter < 200)
        {
            /* shoots slightly differing in speed flame particles out the back, to warn the player */
          int holddir = (dir-1) % 3 *dir;
          for(i = 0; i <10; i++)
          {
            expl_create((hand->stats->posX+20*holddir), (hand->centerY+i/2), -.1f*dir,
				       0.0f, 1000, 4, (FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN), OWNER_ENEMY);
            expl_create((hand->stats->posX+20*holddir), (hand->centerY+-1*(i/2)), -.1f*dir, 
				       0.0f, 1000, 4, (FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN), OWNER_ENEMY);
          }
        }
        /* the hand accelerates here */
        else if(hand->counter < 220)
        {
          int holddir = (dir-1) % 3 *dir;
          hand->stats->interval += (hand->counter-200)*.02f*dir;
          /* there must be extensive calculations to make sure that no part of the hand can hit
             any wall, even when the hand is at full acceleration*/
          if(!isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, 
                  hand->stats->posX+34+(int)hand->stats->interval, hand->stats->posY)&& 
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, 
                  hand->stats->posX+34+(int)hand->stats->interval, hand->stats->posY+13))
          {
            hand->stats->posX += (int)hand->stats->interval;
          }
          for(i = 0; i <10; i++)
          {
            expl_create((hand->stats->posX+20*holddir), (hand->centerY+i/2), 
                 -.1f*dir,0.0f, 1000, 4, 
                 (FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN), OWNER_ENEMY);
            expl_create((hand->stats->posX+20*holddir), (hand->centerY+-1*(i/2)),   
                 -.1f*dir,0.0f, 1000, 4, 
                 (FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_GREEN), OWNER_ENEMY);
          }
        }
        /* slowdown phase */
        else if(hand->counter < 350)
        {
          if(hand->stats->interval > 0)
          {
              /* here the hand levels off its acceleration back down to 0 */
            hand->stats->interval -= (hand->counter-220)*.04f*dir;
            if(!isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, 
                    hand->stats->posX+34+(int)hand->stats->interval, 
                    hand->stats->posY)&& 
                !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, 
                    hand->stats->posX+34+(int)hand->stats->interval, 
                    hand->stats->posY+13))
            {
                hand->stats->posX += (int)hand->stats->interval;
            }
          }
        }
        /* makes sure the acceleration is 0 and waits */
        else if(hand->counter == 350)
        {
          hand->stats->interval = 0;
        }
        /* manually backs out a little bit */
        else if(hand->counter < 371)
        {
          hand->stats->posX -= (int)(dir*.5);
        }
        /* begins to accelerate back toward its previous position */
        else if(hand->counter < 371)
        {
            /* collision checking prior to actually moving the hand */
          hand->stats->interval -= (hand->counter-350)*.02f*dir;
          if(!isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+
             (int)hand->stats->interval, hand->stats->posY)&& 
             !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+
             (int)hand->stats->interval, hand->stats->posY+13))
          {
            hand->stats->posX += (int)hand->stats->interval;
          }
        }
        /* decelerates the hand to a stop */
        else if(hand->counter < 450)
        {
          if(hand->stats->interval < 0)
          {
              /* collision checking prior to actually moving the hand */
            hand->stats->interval += (hand->counter-370)*.02f*dir;
            if(!isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+
                (int)hand->stats->interval, hand->stats->posY)&& 
               !isLevelPositionCollision(getCurrentLevel(),WALL_LAYER, hand->stats->posX+
                (int)hand->stats->interval, hand->stats->posY+13))
            {
              hand->stats->posX += (int)hand->stats->interval;
            }
          }
        }
        else 
        {
            /* makes sure at the end everything is reset for another punch */
          hand->counter = 0;
          hand->offsetX = 0;
          hand->offsetY = 0;
          arm->stats->posX = hand->stats->posX;
          arm->stats->posY = hand->stats->posY;
          
        }
        hand->counter++;
      }
      /* draws the new hand now that it has been moved */
      drawMainBoss(hand->stats, hand->frame, hand->drawW, hand->drawE);
    }
    else
    {
        /* this is the explosion sequence for the hand when its health reaches 0 */
      if(hand->counter <= 100)
      {
        if(hand->counter % 2 == 0)
        {
          for(i = 0; i < 1; i++)
          {
            explosion(hand->stats->posX+rand()%28+2, hand->stats->posY+rand()%15, 1000, OWNER_NEITHER);
          }
        }
        hand->counter ++;
        if(hand->counter == 100)
        {
          clearMainBoss(hand->stats, hand->frame);
          for(i = 0; i < 20; i++)
          {
            explosion(hand->stats->posX+rand()%28+2, hand->stats->posY+rand()%15, 1000, OWNER_NEITHER);
          }
        }
      }
    }
  }
}
void updateMainBoss()
{
	int i, j;
	if((getCurrentTime() - mainboss.lastMoveTime) >= MOVE_DELAY_MAINBOSS)
  {
	    /* make sure the boss is alive before we do anything else */
      /* takes care of the actions of the hands and arms first */
      armActions(&mainboss.left, &mainboss.leftH, 47, 8, 1);
      armActions(&mainboss.right, &mainboss.rightH, -10, 8, -1);
		    if(mainboss.stats->health > 0)
        {
          static float seed = 0; /* a seed for the random particle systems, to keep 
                                    all the particles in that particular turn with the same seed, rather
                                    than each particle be completely random and therefore doing their own
                                    thing*/
			      /* clears the boss for a redraw */
            clearMainBoss(mainboss.stats, mainboss.frame);
            /* sees which state the boss is in */
            switch (mainboss.state)
            {
            case 1:
                /* the base state */
                mainboss.counter++;
                if(mainboss.counter > 100)
                {
                    /* depending on which phase the boss is in, choose which particle
                       obstacle to use randomly*/
                    mainboss.counter = 0;
                    if(mainboss.rightH.alive || mainboss.leftH.alive)
                    {
                      mainboss.state = rand() % 4 + 1;
                    }
                    /* once both arms have been blown off, there are more particle systems */
                    else
                    {
		                  mainboss.state = rand() % 6 + 1;
                    }
                    /* sets up the random seed for the particular animation to be seen */
                    seed = (rand() % 40 - 20)/10.0f;
                }
                break;
            case 2:
					   if(mainboss.counter < 120)
					   {
               for(i = -2; i <= 2; i++ )
               {
                  /* these are the fire turret effects, there are five of them and they make an echelon formation */
				          setRand(expl_create(mainboss.center+1+i*20, mainboss.stats->posY-1 + abs(i)*5, 
				          .7f*1.5f, -.2f*1.5f, 5000,8, FOREGROUND_RED | BACKGROUND_GREEN| FOREGROUND_INTENSITY, OWNER_ENEMY), seed);
                  setRand(expl_create(mainboss.center-1+i*20, mainboss.stats->posY-1+abs(i)*5, 
				          -.7f*1.5f, -.2f*1.5f, 5000,8, FOREGROUND_RED | BACKGROUND_GREEN| FOREGROUND_INTENSITY, OWNER_ENEMY), seed);
                  setRand(expl_create(mainboss.center+i*20, mainboss.stats->posY-1+abs(i)*5, 
				          0, -.3f, 5000,8, 
                  FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_GREEN| FOREGROUND_INTENSITY, OWNER_ENEMY), seed);
                  setRand(expl_create(mainboss.center-2+i*20, mainboss.stats->posY-1+abs(i)*5, 
				          -.7f*1.5f, -.2f*1.5f, 5000,8, 
                  FOREGROUND_RED | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
                  setRand(expl_create(mainboss.center+2+i*20, mainboss.stats->posY-1+abs(i)*5, 
				          .7f*1.5f, -.2f*1.5f, 5000,8, 
                  FOREGROUND_RED | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
                }
              }
					    else
					    {
                /* always remember to reset and return to the base state */
						    mainboss.counter = 0;
						    mainboss.state = 1;
					    }
              mainboss.counter ++;
					    break;
		    case 5:
            /* these are the lasers which come in toward the center and back out */
					    if(mainboss.counter < 200)
					    {
							    for(i = 0; i <4; i++)
							    {
                      /* creates 3 particle streams 4 units wide */
								    expl_create(i-2 + mainboss.center, mainboss.stats->posY, 
									    ((rand() % 40) - 20) * .01f, -.8f, 2000,5, FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY);
                    /* these two have their vector dependent on the counter, meaning that they spray in a forward pattern */
								    expl_create(i-2 + mainboss.center + 23, mainboss.stats->posY+6, 
									    -1*(mainboss.counter - 170)*.01f,-.8f, 2000,5, FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY);
								    expl_create(i-2 + mainboss.center - 23, mainboss.stats->posY+6, 
									    (mainboss.counter - 170) * .01f, -.8f, 2000,5, FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY);
							    }
						    mainboss.counter ++;
					    }
              /* this is the same thing, but going backwards, so it gets that sprinkler effect */
					    else if(mainboss.counter < 400)
					    {
							    for(i = 0; i <4; i++)
							    {
								    expl_create(i-2 + mainboss.center, mainboss.stats->posY, 
									    ((rand() % 40) - 20) * .01f, -.8f, 2000,5, FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY);
								    expl_create(i-2 + mainboss.center + 23, mainboss.stats->posY+6, 
									    (mainboss.counter - 230) * .01f, -.8f, 2000,5, FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY);
								    expl_create(i-2 + mainboss.center - 23, mainboss.stats->posY+6, 
									    -1*(mainboss.counter - 230)*.01f,-.8f, 2000,5, FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY);
							    }
						    mainboss.counter ++;
					    }
					    else
					    {
               /* resets and goes to the base state */
						    mainboss.counter = 0;
						    mainboss.state = 1;
					    }
					    break;
            case 4:
                /* the fire state */
              if(mainboss.counter < 4)
              {
                  /* gives a slight warning that this will happen by making the top red part of the
                     boss chance character and color*/
                for (i = 0; i < 60; i++)
                {
                  if(mainboss.frame[mainboss.counter][i].Char.AsciiChar == 'G')
                  {
                    mainboss.frame[mainboss.counter][i].Char.AsciiChar = 'X';
                  }
                  if(mainboss.frame[mainboss.counter][i].Attributes == 
                      (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN))
                  {
                    mainboss.frame[mainboss.counter][i].Attributes = 
                      (FOREGROUND_RED | FOREGROUND_INTENSITY);
                  }
                }
              }
              else if(mainboss.counter < 80)
              {
                if(mainboss.counter% 5 == 0)
                {
                  for(i = 0; i <40; i++)
                  {
                      /* uses an extremely random algorithm to dictate the path, length, and position
                         of the fire particles*/
                      expl_create((i/5 - 4) + mainboss.center, mainboss.stats->posY, 
                                 (i/5-4)*.1f, -.3f, (rand() % 500) + 1000,4, 
                                 FOREGROUND_RED | BACKGROUND_GREEN, OWNER_ENEMY);
                      expl_create((i/5 - 4) + mainboss.center+23, mainboss.stats->posY+6, 
                                 (i/5-4)*.1f, -.3f, (rand() % 500) + 1000,4, 
                                 FOREGROUND_RED | BACKGROUND_GREEN, OWNER_ENEMY);
                      expl_create((i/5 - 4) + mainboss.center-23, mainboss.stats->posY+6, 
                                 (i/5-4)*.1f, -.3f, (rand() % 500) + 1000,4, 
                                 FOREGROUND_RED | BACKGROUND_GREEN, OWNER_ENEMY);
                  }
                }
              }
              else if(mainboss.counter < 84)
              {
                for (i = 0; i < 60; i++)
                {
                    /* closes back up the front part of the main boss */
                  if(mainboss.frame[mainboss.counter-80][i].Char.AsciiChar == 'X')
                  {
                    mainboss.frame[mainboss.counter-80][i].Char.AsciiChar = 'G';
                  }
                  if(mainboss.frame[mainboss.counter-80][i].Attributes == 
                      (FOREGROUND_RED | FOREGROUND_INTENSITY))
                  {
                    mainboss.frame[mainboss.counter-80][i].Attributes = 
                        (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN);
                  }
                }
              }
              else
              {
                  /* resets and returns to base state */
                  mainboss.counter = 0;
                  mainboss.state = 1;
              }
              mainboss.counter ++;
              break;
            case 3:
					    if(mainboss.counter < 60)
					    {
                  /* this makes the meandering lightning particle systems
                     one particle stream is needed for each color and each individual stream*/
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .7f, -.2f, 5000,7, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .7f, -.2f, 5000,7, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.7f, -.2f, 5000,7, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.7f, -.2f, 5000,7, FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .6f, -.3f, 5000,7, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .6f, -.3f, 5000,7, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.6f, -.3f, 5000,7, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.6f, -.3f, 5000,7, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .5f, -.5f, 5000,7, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .5f, -.5f, 5000,7, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.5f, -.5f, 5000,7, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.5f, -.5f, 5000,7, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .1f, -.9f, 5000,7, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .1f, -.9f, 5000,7, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.1f, -.9f, 5000,7, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.1f, -.9f, 5000,7, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
              }
					    else
					    {
                  /* reset and return to base state */
						    mainboss.counter = 0;
						    mainboss.state = 1;
					    }
              mainboss.counter ++;
					    break;
            case 6:
                /* creates the blizzard effects */
					    if(mainboss.counter < 30)
					    {
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .7f, -.2f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .7f, -.2f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);

                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.7f, -.2f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.7f, -.2f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
              }
					    else if(mainboss.counter < 60)
					    {
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .6f, -.3f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .6f, -.3f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);

                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.6f, -.3f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.6f, -.3f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
              }
              else if(mainboss.counter < 90)
					    {
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .5f, -.5f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .5f, -.5f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);

                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.5f, -.5f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.5f, -.5f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
              }

              else if(mainboss.counter < 120)
					    {
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .3f, -.7f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .3f, -.7f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);

                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.3f, -.7f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.3f, -.7f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
              }
              else if(mainboss.counter < 150)
					    {
                setRand(expl_create(mainboss.center+2, mainboss.stats->posY-1, 
				        .1f, -.9f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center+1, mainboss.stats->posY-1, 
				        .1f, -.9f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);

                setRand(expl_create(mainboss.center-2, mainboss.stats->posY-1, 
				        -.1f, -.9f, 5000,6, 
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |  BACKGROUND_GREEN, OWNER_ENEMY), seed);
				        setRand(expl_create(mainboss.center-1, mainboss.stats->posY-1, 
				        -.1f, -.9f, 5000,6, 
                FOREGROUND_BLUE | BACKGROUND_GREEN, OWNER_ENEMY), seed);
              }
					    else
					    {
                  /* reset and return to base state */
						    mainboss.counter = 0;
						    mainboss.state = 1;
					    }
              mainboss.counter ++;
					    break;
				    case 7:
                /* this takes off the mainboss's shell when both arms have been destroyed */
					    if(mainboss.counter < 30)
              {
						    for (i = 0; i < 80; i++)
						    {
							    if(mainboss.frame[mainboss.counter-1][i].Char.AsciiChar == 'H')
							    {
								    mainboss.frame[mainboss.counter-1][i].Char.AsciiChar = 
                        mainboss.frame[mainboss.counter-1][i].Char.AsciiChar = ' ';
							    }
							    if(mainboss.frame[mainboss.counter-1][i].Char.AsciiChar == 'O')
							    {
								    mainboss.frame[mainboss.counter-1][i].Char.AsciiChar = 
                        mainboss.frame[mainboss.counter-1][i].Char.AsciiChar = 'X';
							    }
							    if(mainboss.frame[mainboss.counter-1][i].Attributes == 
                      (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN))
							    {
								    mainboss.frame[mainboss.counter-1][i].Attributes = 
                        (FOREGROUND_RED | FOREGROUND_INTENSITY);
							    }
						    }
					    }
					    else
					    {
                  /* reset and return to base state */
						    mainboss.state = 1;
						    mainboss.counter = 0;
                /* make a ton of rocket turrets to discourage going to the sides of the boss room */
                turret_create(162, 312,6000, 9999999, 6);
                turret_create(162, 332,6000, 9999999, 6);
                turret_create(329, 312,6000, 9999999, 6);
                turret_create(329, 332,6000, 9999999, 6);
                turret_create(162, 315,6000, 9999999, 6);
                turret_create(162, 330,6000, 9999999, 6);
                turret_create(329, 315,6000, 9999999, 6);
                turret_create(329, 330,6000, 9999999, 6);
					    }
					    mainboss.counter++;
					    break;
				default:
					break;
				}
        /* this if else statement makes the boss blink when it is hit */
				if((mainboss.stats->health != mainboss.hpHolder) && (mainboss.counter%2 == 0))
				{
					for (i = 0; i < 30; i++)
					{
						for(j = 0; j < 61; j++)
						{
							if(mainboss.frame[i][j].Attributes == 
                  (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED))
							{
								mainboss.frame[i][j].Attributes = 
                    (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
							}
						}
					}
					mainboss.hpHolder = mainboss.stats->health;
				}
				else
				{
					for (i = 0; i < 30; i++)
					{
						for(j = 0; j < 61; j++)
						{
							if(mainboss.frame[i][j].Attributes == 
                  (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY))
							{
								mainboss.frame[i][j].Attributes = 
                    (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
							}
						}
					}
				}
        /* if the health for the left arm is 0 set its attributes so that it is dead */
        if(mainboss.left.stats->health <= 0)
        { 
          mainboss.left.stats->health = 99999999;
          mainboss.state = 1;
          mainboss.counter = 0;
          mainboss.left.counter = 0;
          mainboss.left.alive = 0;
        }
         /* if the health for the left hand is 0 set its attributes so that it is dead */
        if(mainboss.leftH.stats->health <= 0)
        { 
          mainboss.leftH.stats->health = 99999999;
          mainboss.state = 1;
          mainboss.counter = 0;
          mainboss.leftH.counter = 0;
          mainboss.leftH.alive = 0;
          /* make sure both hands are dead before going to the last phase */
          if(!mainboss.rightH.alive)
          {
            mainboss.state = 7;
          }
        }
         /* if the health for the right arm is 0 set its attributes so that it is dead */
        if(mainboss.right.stats->health <= 0)
        { 
          mainboss.right.stats->health = 99999999;
          mainboss.state = 1;
          mainboss.counter = 0;
          mainboss.right.counter = 0;
          mainboss.right.alive = 0;
        }
         /* if the health for the right hand is 0 set its attributes so that it is dead */
        if(mainboss.rightH.stats->health <= 0)
        { 
          mainboss.rightH.stats->health = 99999999;
          mainboss.state = 1;
          mainboss.counter = 0;
          mainboss.rightH.counter = 0;
          mainboss.rightH.alive = 0;
          /* makes sure both hands are dead before going to the last phase */
          if(!mainboss.leftH.alive)
          {
            mainboss.state = 7;
          }
        }
        drawMainBoss(mainboss.stats, mainboss.frame, mainboss.drawW, mainboss.drawE);
        /* kills the player if the player gets run over by any part of the main boss */
        if((getPlyrCollidedWith() == 'l') || (getPlyrCollidedWith() == '`')|| 
           (getPlyrCollidedWith() == 'L') || (getPlyrCollidedWith() == '$') || (getPlyrCollidedWith() == '~'))
        {
          player.hp = 0;
        }
      }
        
      /* makes sure to kill the main boss once the minimum health is reached */
      else if (mainboss.stats->health > -100000000)
      {
        /* all this does is make sure that the boss counter is set to 0 before the boss dies */
        mainboss.counter = 0;
        mainboss.stats->health = -1000000000;
      }
     /* death animation for the main boss */
		  else
      {
        if(mainboss.counter <= 200)
        {
          if(mainboss.counter % 2 == 0)
          {
            for(i = 0; i < 5; i++)
            {
              explosion(mainboss.stats->posX+rand()%60,mainboss.stats->posY+rand()%15, 2000, OWNER_PLAYER);
            }
          }
          if(mainboss.counter == 100)
          {
            clearMainBoss(mainboss.stats, mainboss.frame);
            for(i = 0; i < 20; i++)
            {
              explosion(mainboss.stats->posX+rand()%100-50, mainboss.stats->posY+rand()%60-30, 2000, OWNER_PLAYER);
            }
          }
        }
        else
        {
			    mainboss.alive = 0;
        }
        mainboss.counter++;
		  }
      mainboss.lastMoveTime = (int)getCurrentTime();
  }
}


void drawMainBoss(ENEMY *coords, CHAR_INFO inf[BOSS_HEIGHT][BOSS_WIDTH], LEVEL_POSITION * draw, LEVEL_POSITION * drawE)
{
	int x, y;
	/* put the level positions in the right spot */
	for(y = 0; y < BOSS_HEIGHT; y++)
	{
		for(x = 0; x < BOSS_WIDTH; x++)
		{
			if(inf[y][x].Char.AsciiChar != ' ')
			{
				if(inf[y][x].Char.AsciiChar == 'X')
				{
            /* makes sure to set the correct layer to the correct parts of the boss */
					setLevelPosition(ENEMY_LAYER, coords->posX + x,coords->posY + y, drawE);
				}
				else
				{
					setLevelPosition(WALL_LAYER, coords->posX + x,coords->posY + y, draw);
				}
			}
		}
	}
}

/* deletes the part of the boss so it can be moved */
void clearMainBoss(ENEMY *coords, CHAR_INFO inf[BOSS_HEIGHT][BOSS_WIDTH])
{
	int x, y;
	for(y = 0; y < BOSS_HEIGHT; y++)
	{
		for(x = 0; x < BOSS_WIDTH; x++)
		{
			if(inf[y][x].Char.AsciiChar != ' ')
			{
				setLevelPosition(WALL_LAYER, coords->posX + x, coords->posY + y, 
					whiteSpacePosition);
				setLevelPosition(ENEMY_LAYER, coords->posX + x, coords->posY + y, 
					whiteSpacePosition);
			}
		}
	}
}



/* loads up the characters needed to view the last boss */
void loadMainBossChars(char *fileName, char *colorName,  CHAR_INFO cha[BOSS_HEIGHT][BOSS_WIDTH])
{
	/* load the files */
	FILE *levelFile;
	FILE *colorFile;

	/* holds all the lines we read in so we can store them individually */
	char lineBuffer[LONGEST_LINE] = {0};

	/* counters to get across the screen */
	int i = 0, j = 0;

	/* open the file in read-text mode */
	levelFile = fopen(fileName, "rt");

	/* read through the file once to find out the levels dimensions. */
	if(levelFile == NULL)
	{
		/* printf("Error: could not load level file "); */
	}
	else
	{
		/* fill in the new array with trivial characters, so there is always a default */
		/* go through each line */
		for(i = 0; i < BOSS_HEIGHT; i++)
		{
			/* read in all the characters in the line and store their characer values in the array */
			for(j = 0; j < BOSS_WIDTH; j++)
			{

				cha[i][j].Char.AsciiChar = ' ';
				cha[i][j].Attributes = (BACKGROUND_GREEN);
			}
		}

		i = 0;

		/* loop through the entire file and read information and create our level */
		while((fgets(&lineBuffer[0], LONGEST_LINE, levelFile )!= NULL) && i < BOSS_HEIGHT)
		{
			for(j = 0; j < BOSS_WIDTH; j++)
			{
				if(lineBuffer[j] > 30)
				{
					cha[i][j].Char.AsciiChar = lineBuffer[j];   
				}
			}
			i++;
		}
	}
	/* close the file now that we are finished with it */
	fclose(levelFile);


	/* we are repeating most of the process with the color file */
	colorFile = fopen(colorName, "rt");
	if(colorFile == NULL)
	{
		/* printf("Error: could not load level file "); */
	}
	else
	{
		/* we dont need to re-clear the screen, so that first loop is not present */
		i = 0;
		/* loop through the entire file and read information and create our level */
		while((fgets(&lineBuffer[0], LONGEST_LINE, colorFile )!= NULL) && (i < BOSS_HEIGHT))
		{
			for(j = 0; j < BOSS_WIDTH; j++)
			{
				switch (lineBuffer[j])
				{
				case 'b':
					cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_GREEN);
					break;
				case 'B':
					cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
					break;
				case 'V':
					cha[i][j].Attributes = (FOREGROUND_BLUE | BACKGROUND_BLUE);
					break;
				case 'r':
					cha[i][j].Attributes = (FOREGROUND_RED);
					break;
				case 'R':
					cha[i][j].Attributes = (FOREGROUND_RED | FOREGROUND_INTENSITY);
					break;
				case 'g':
					cha[i][j].Attributes = (FOREGROUND_GREEN);
					break;
				case 'G':
					cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					break;
				case 'o':
					cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_RED);
					break;
				case 'O':
					cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
					break;
				case 'p':
					cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_RED);
					break;
				case 'P':
					cha[i][j].Attributes = (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
					break;
				case 'y':
					cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE);
					break;
				case 'Y':
					cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					break;
				case 'w':
					cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
					break;
				case 'W':
					cha[i][j].Attributes = (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED| FOREGROUND_INTENSITY);
					break;
				}
			}
			i++;
		}
		fclose(colorFile);
	}
}


CHAR_INFO *getMainBossChar(int x, int y)
{
	return &(mainboss.frame[y][x]);
}

CHAR_INFO *getLarmChar(int x, int y)
{
	return &(mainboss.left.frame[y][x]);
}

CHAR_INFO *getLhandChar(int x, int y)
{
	return &(mainboss.leftH.frame[y][x]);
}

CHAR_INFO *getRhandChar(int x, int y)
{
	return &(mainboss.rightH.frame[y][x]);
}
CHAR_INFO *getRarmChar(int x, int y)
{
	return &(mainboss.right.frame[y][x]);
}
