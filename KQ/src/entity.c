/*
   KQ is Copyright (C) 2002 - Josh Bolduc

   This file is part of KQ... a freeware RPG.

   KQ is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   KQ is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with KQ; see the file COPYING.  If not, write to
   the Free Software Foundation,
       675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*! \file
 * \brief Entity functions
 *
 * \author JB
 * \date ??????
*/

#include <stdio.h>
#include <string.h>

#include "kq.h"
#include "entity.h"
#include "setup.h"
#include "menu.h"
#include "combat.h"
#include "itemdefs.h"
#include "intrface.h"



/*! \name  global variables  */
/*\{*/
int emoved;                     /*!< non-zero if the entity moved this turn */
/*\}*/



/*  internal functions  */
static int entity_near (int, int, int);
static void speed_adjust (int);
static void process_entity (int);
static void lastm_check (int);
static void follow (void);
static void wander (int);
static void player_move (void);
static void moveup (int);
static void movedown (int);
static void moveright (int);
static void moveleft (int);
static int obstruction (int, int, int, int);
static void parsems (int);
static void getcommand (int);
static void entscript (int);



/*! \brief Main entity routine
 *
 * The main routine that loops through the entity list and processes each
 * one.
*/
void process_entities (void)
{
   int i;

   for (i = 0; i < MAX_ENT; i++)
      if (g_ent[i].active == 1)
         speed_adjust (i);
}



/*! \brief Adjust movement speed
 *
 * This has to adjust for each entity's speed.
 * 'Normal' speed appears to be 4.
 *
 * \param   i Index of entity
*/
static void speed_adjust (int i)
{
   if (g_ent[i].speed < 4) {
      switch (g_ent[i].speed) {
      case 1:
         if (g_ent[i].scount < 3) {
            g_ent[i].scount++;
            return;
         }
         break;
      case 2:
         if (g_ent[i].scount < 2) {
            g_ent[i].scount++;
            return;
         }
         break;
      case 3:
         if (g_ent[i].scount < 1) {
            g_ent[i].scount++;
            return;
         }
         break;
      }
   }
   if (g_ent[i].speed < 5)
      process_entity (i);
   switch (g_ent[i].speed) {
   case 5:
      process_entity (i);
      process_entity (i);
      break;
   case 6:
      process_entity (i);
      process_entity (i);
      process_entity (i);
      break;
   case 7:
      process_entity (i);
      process_entity (i);
      process_entity (i);
      process_entity (i);
      break;
   }
   if (key[kctrl] && i < PSIZE)
      process_entity (i);
}



/*! \brief Actions for one entity
 *
 * Process an individual active entity.  If the entity in question
 * is #0 (main character) and the party is not automated, then allow
 * for player input.
 *
 * \param   i Index of entity
*/
static void process_entity (int i)
{
   g_ent[i].scount = 0;
   if (g_ent[i].active) {
      if (g_ent[i].moving == 0) {
         if (i == 0 && !autoparty) {
            player_move ();
            if (g_ent[i].moving > 0 && display_desc == 1)
               display_desc = 0;
            return;
         }
         if (g_ent[i].movemode == 0)
            return;
         if (g_ent[i].movemode == 1)
            wander (i);
         if (g_ent[i].movemode == 2)
            entscript (i);
         if (g_ent[i].movemode == 3) {
            if (g_ent[i].chasing == 0) {
               if (entity_near (i, 0, 3) == 1
                   && rand () % 100 <= g_ent[i].extra) {
                  g_ent[i].chasing = 1;
                  if (g_ent[i].speed < 7)
                     g_ent[i].speed++;
                  g_ent[i].delay = 0;
                  /* PH FIXME check for emoved here? or not? */
                  if (g_ent[0].tilex > g_ent[i].tilex && emoved == 0)
                     moveright (i);
                  if (g_ent[0].tilex < g_ent[i].tilex && emoved == 0)
                     moveleft (i);
                  if (g_ent[0].tiley > g_ent[i].tiley && emoved == 0)
                     movedown (i);
                  if (g_ent[0].tiley < g_ent[i].tiley && emoved == 0)
                     moveup (i);
                  if (emoved == 0)
                     wander (i);
               } else
                  wander (i);
            } else {
               if (entity_near (i, 0, 4) == 1) {
                  /* PH FIXME check for emoved here? or not? */
                  if (g_ent[0].tilex > g_ent[i].tilex)
                     moveright (i);
                  if (g_ent[0].tilex < g_ent[i].tilex && emoved == 0)
                     moveleft (i);
                  if (g_ent[0].tiley > g_ent[i].tiley && emoved == 0)
                     movedown (i);
                  if (g_ent[0].tiley < g_ent[i].tiley && emoved == 0)
                     moveup (i);
                  if (emoved == 0)
                     wander (i);
               } else {
                  g_ent[i].chasing = 0;
                  if (g_ent[i].speed > 1)
                     g_ent[i].speed--;
                  g_ent[i].delay = 25 + rand () % 25;
                  wander (i);
               }
            }
         }
      } else {
         if (g_ent[i].moving == MOVE_DOWN) {
            g_ent[i].y++;
            g_ent[i].movcnt--;
            g_ent[i].framectr++;
         }
         if (g_ent[i].moving == MOVE_UP) {
            g_ent[i].y--;
            g_ent[i].movcnt--;
            g_ent[i].framectr++;
         }
         if (g_ent[i].moving == MOVE_LEFT) {
            g_ent[i].x--;
            g_ent[i].movcnt--;
            g_ent[i].framectr++;
         }
         if (g_ent[i].moving == MOVE_RIGHT) {
            g_ent[i].x++;
            g_ent[i].movcnt--;
            g_ent[i].framectr++;
         }
         if (g_ent[i].framectr > 20)
            g_ent[i].framectr = 0;
         if (g_ent[i].movcnt == 0 && g_ent[i].moving > 0) {
            g_ent[i].moving = MOVE_NOT;
            if (i < PSIZE) {
               steps++;
               if (steps >= STEPS_NEEDED)
                  steps = STEPS_NEEDED;
               if (party[pidx[i]].sts[S_POISON] > 0) {
                  party[pidx[i]].hp--;
                  if (party[pidx[i]].hp < 1)
                     party[pidx[i]].hp = 1;
                  play_effect (21, 128);
               }
               if (party[pidx[i]].eqp[5] == I_REGENERATOR) {
                  party[pidx[i]].hp++;
                  if (party[pidx[i]].hp > party[pidx[i]].mhp)
                     party[pidx[i]].hp = party[pidx[i]].mhp;
               }
            }
            if (i == 0)
               zone_check ();
         }
         if (i == 0 && vfollow == 1)
            calc_viewport (0);
      }
   }
}



/*! \brief Check proximity
 *
 * Check to see if the target is within rad squares.
 * Test area is a square box rather than a circle
 * target entity needs to be within the view area
 * to be visible
 * (PH) this implementation is really odd :?
 *
 * \param   eno Entity under consideration
 * \param   tgt Entity to test
 * \param   rad Radius to test within
 * \returns 1 if near, 0 otherwise
*/
static int entity_near (int eno, int tgt, int rad)
{
   int ax, ay, ex, ey, b;

   b = 0 - rad;
   ex = g_ent[eno].tilex;
   ey = g_ent[eno].tiley;
   for (ay = b; ay <= rad; ay++)
      for (ax = b; ax <= rad; ax++)
         if (ex + ax >= view_x1 && ax + ax <= view_x2 && ey + ay >= view_y1
             && ey + ay <= view_y2)
            if (ex + ax == g_ent[tgt].tilex && ey + ay == g_ent[tgt].tiley)
               return 1;
   return 0;
}



/*! \brief Handle party following
 *
 * This is used to co-ordinate the following of party
 * members after the first.
 * Basically it's an implementation of a queue
 *
 * \param   lm Last moved direction
*/
static void lastm_check (int lm)
{
   int i;

   for (i = numchrs - 1; i > 0; i--)
      lastm[i] = lastm[i - 1];
   lastm[0] = lm;
}



/*! \brief Party following leader
 *
 * This makes any characters (after the first) follow the leader.
*/
static void follow (void)
{
   int i;

   if (numchrs == 1)
      return;
   for (i = 1; i < numchrs; i++) {
      if (lastm[i] == 0)
         return;
      g_ent[i].facing = lastm[i] - 1;
      g_ent[i].moving = lastm[i];
      g_ent[i].movcnt = 15;
      switch (lastm[i]) {
      case MOVE_RIGHT:
         {
            g_ent[i].x++;
            g_ent[i].tilex++;
            break;
         }
      case MOVE_DOWN:
         {
            g_ent[i].y++;
            g_ent[i].tiley++;
            break;
         }
      case MOVE_LEFT:
         {
            g_ent[i].x--;
            g_ent[i].tilex--;
            break;
         }
      case MOVE_UP:
         {
            g_ent[i].y--;
            g_ent[i].tiley--;
            break;
         }
      }
   }
}



/*! \brief Move randomly
 *
 * Choose a random direction for the entity to walk in and set up the
 * vars to do so.
 *
 * \param   i Index of entity to move
*/
static void wander (int i)
{
   if (g_ent[i].delayctr < g_ent[i].delay) {
      g_ent[i].delayctr++;
      return;
   }
   g_ent[i].delayctr = 0;
   switch (rand () % 8) {
   case 0:
      moveup (i);
      break;
   case 1:
      movedown (i);
      break;
   case 2:
      moveleft (i);
      break;
   case 3:
      moveright (i);
      break;
   }
}



/*! \brief Process movement for player
 *
 * This is the replacement for process_controls that used to be in kq.c
 * I realized that all the work in process_controls was already being
 * done in process_entity... I just had to make this exception for the
 * player-controlled dude.
*/
static void player_move (void)
{
   readcontrols ();

   if (balt)
      activate ();
   if (benter)
      menu ();
#ifdef KQ_CHEATS
   if (key[KEY_F10]) {
      unpress ();
      do_luacheat ();
   }
#endif
   if (right) {
      moveright (0);
      if (g_ent[0].moving > 0) {
         lastm_check (MOVE_RIGHT);
         follow ();
      }
      return;
   }
   if (down) {
      movedown (0);
      if (g_ent[0].moving > 0) {
         lastm_check (MOVE_DOWN);
         follow ();
      }
      return;
   }
   if (left) {
      moveleft (0);
      if (g_ent[0].moving > 0) {
         lastm_check (MOVE_LEFT);
         follow ();
      }
      return;
   }
   if (up) {
      moveup (0);
      if (g_ent[0].moving > 0) {
         lastm_check (MOVE_UP);
         follow ();
      }
      return;
   }
}



/*! \brief Movement upwards
 *
 * Set up the entity vars to move upwards (if possible).
 *
 * \param   i Index of entity to move
*/
static void moveup (int i)
{
   int tx, ty;

   emoved = 0;
   tx = g_ent[i].x / 16;
   ty = g_ent[i].y / 16;
   g_ent[i].facing = FACE_UP;
   if (ty == 0)
      return;
   if (g_ent[i].obsmode == 1
       && (obstruction (tx, ty, 0, -1) || entityat (tx, ty - 1, i)))
      return;
   g_ent[i].tiley--;
   g_ent[i].moving = MOVE_UP;
   g_ent[i].movcnt = 15;
   g_ent[i].y--;
   emoved = 1;
}



/*! \brief Movement downwards
 *
 * Set up the entity vars to move down (if possible).
 *
 * \param   i Index of entity to move
*/
static void movedown (int i)
{
   int tx, ty;

   emoved = 0;
   tx = g_ent[i].x / 16;
   ty = g_ent[i].y / 16;
   g_ent[i].facing = FACE_DOWN;
   if (ty == g_map.ysize - 1)
      return;
   if (g_ent[i].obsmode == 1
       && (obstruction (tx, ty, 0, 1) || entityat (tx, ty + 1, i)))
      return;
   g_ent[i].tiley++;
   g_ent[i].moving = MOVE_DOWN;
   g_ent[i].movcnt = 15;
   g_ent[i].y++;
   emoved = 1;
}



/*! \brief Movement rightwards
 *
 * Set up the entity vars to move right (if possible).
 *
 * \param   i Index of entity to move
*/
static void moveright (int i)
{
   int tx, ty;

   emoved = 0;
   tx = g_ent[i].x / 16;
   ty = g_ent[i].y / 16;
   g_ent[i].facing = FACE_RIGHT;
   if (tx == g_map.xsize - 1)
      return;
   if (g_ent[i].obsmode == 1
       && (obstruction (tx, ty, 1, 0) || entityat (tx + 1, ty, i)))
      return;
   g_ent[i].tilex++;
   g_ent[i].moving = MOVE_RIGHT;
   g_ent[i].movcnt = 15;
   g_ent[i].x++;
   emoved = 1;
}



/*! \brief Movement leftwards
 *
 * Set up the entity vars to move left (if possible).
 *
 * \param   i Index of entity to move
*/
static void moveleft (int i)
{
   int tx, ty;

   emoved = 0;
   tx = g_ent[i].x / 16;
   ty = g_ent[i].y / 16;
   g_ent[i].facing = FACE_LEFT;
   if (tx == 0)
      return;
   if (g_ent[i].obsmode == 1
       && (obstruction (tx, ty, -1, 0) || entityat (tx - 1, ty, i)))
      return;
   g_ent[i].tilex--;
   g_ent[i].moving = MOVE_LEFT;
   g_ent[i].movcnt = 15;
   g_ent[i].x--;
   emoved = 1;
}

/*! \brief Check for obstruction
 *
 * Check for any map-based obstructions in the specified co-ordinates.
 *
 * \param   ox Original x-coord position
 * \param   oy Original y-coord position
 * \param   mx Amount to move
 * \param   my Amount to move
 * \returns 1 if path is obstructed, 0 otherwise
*/
static int obstruction (int ox, int oy, int mx, int my)
{
   int son, sto, tox, toy;

   tox = ox + mx;
   if (tox < 0 || tox > g_map.xsize - 1)
      return 1;
   toy = oy + my;
   if (toy < 0 || toy > g_map.ysize - 1)
      return 1;
   son = o_seg[(oy * g_map.xsize) + ox];
   sto = o_seg[(toy * g_map.xsize) + tox];
   if (sto == 1)
      return 1;
   if (mx == 0) {
      if (my == -1) {
         if (son == 2 || sto == 4)
            return 1;
      }
      if (my == 1) {
         if (son == 4 || sto == 2)
            return 1;
      }
   }
   if (my == 0) {
      if (mx == -1) {
         if (son == 5 || sto == 3)
            return 1;
      }
      if (mx == 1) {
         if (son == 3 || sto == 5)
            return 1;
      }
   }
   return 0;
}



/*! \brief Check entites at location
 *
 * Check for any entities in the specified co-ordinates.
 * Runs combat routines if a character and an enemy meet,
 * and de-activate the enemy if it was defeated.
 *
 * \sa combat_check()
 * \param   ox x-coord to check
 * \param   oy y-coord to check
 * \param   who Id of entity doing the checking
 * \returns index of entity found+1 or 0 if none found
*/
int entityat (int ox, int oy, int who)
{
   int i;

   for (i = 0; i < MAX_ENT; i++) {
      if (g_ent[i].active && ox == g_ent[i].tilex && oy == g_ent[i].tiley) {
         if (who >= PSIZE) {
            if (g_ent[who].eid == ID_ENEMY && i < PSIZE) {
               if (combat_check (ox, oy) == 1)
                  g_ent[who].active = 0;
               return 0;
            }
            return i + 1;
         } else {
            if (g_ent[i].eid == ID_ENEMY) {
               if (combat_check (ox, oy) == 1)
                  g_ent[i].active = 0;
               return 0;
            }
            if (i >= PSIZE)
               return i + 1;
         }
      }

   }
   return 0;
}



/*! \brief Read an int from a script
 *
 * This parses the movement script for a value that relates
 * to a command.  This is from Verge1.
 *
 * \param   n Entity to process
*/
static void parsems (int n)
{
   int p = 0;
   char tok[10];
   char s;

   s = g_ent[n].script[g_ent[n].sidx];
   while (s >= 48 && s <= 57) {
      tok[p] = s;
      g_ent[n].sidx++;
      s = g_ent[n].script[g_ent[n].sidx];
      p++;
   }
   tok[p] = 0;
   g_ent[n].cmdnum = atoi (tok);
}



/*! \brief Read a command and parameter from a script
 *
 * This processes entity commands from the movement script.
 * This is from Verge1.
 *
 * Script commands are:
 * - U,R,D,L + param:  move up, right, down, left by param spaces
 * - W+param: wait param frames
 * - B: start script again
 * - X+param: move to x-coord param
 * - Y+param: move to y-coord param
 * - F+param: face direction param
 *
 * \param   n Entity to process
*/
static void getcommand (int n)
{
   char s;
   /* PH FIXME: prevented from running off end of string */
   if (g_ent[n].sidx < sizeof (g_ent[n].script))
      s = g_ent[n].script[g_ent[n].sidx++];
   else
      s = '\0';
   switch (s) {
   case 'U':
      g_ent[n].cmd = 1;
      parsems (n);
      break;
   case 'D':
      g_ent[n].cmd = 2;
      parsems (n);
      break;
   case 'L':
      g_ent[n].cmd = 3;
      parsems (n);
      break;
   case 'R':
      g_ent[n].cmd = 4;
      parsems (n);
      break;
   case 'W':
      g_ent[n].cmd = 5;
      parsems (n);
      break;
   case '\0':
      g_ent[n].cmd = 6;
      g_ent[n].movemode = 0;
      g_ent[n].cmdnum = 0;
      g_ent[n].sidx = 0;
      break;
   case 'B':
      g_ent[n].cmd = 7;
      break;
   case 'X':
      g_ent[n].cmd = 8;
      parsems (n);
      break;
   case 'Y':
      g_ent[n].cmd = 9;
      parsems (n);
      break;
   case 'F':
      g_ent[n].cmd = 10;
      parsems (n);
      break;
      /* PH add: command K makes the ent disappear */
   case 'K':
      g_ent[n].active = 0;
      break;
   default:
      program_death ("Invalid entity command!");
   }
}



/*! \brief Run script
 *
 * This executes script commands.  This is from Verge1.
 *
 * \param   n Entity to process
*/
static void entscript (int n)
{
   if (g_ent[n].cmd == 0)
      getcommand (n);
   switch (g_ent[n].cmd) {
   case 1:
      moveup (n);
      if (emoved)
         g_ent[n].cmdnum--;
      break;
   case 2:
      movedown (n);
      if (emoved)
         g_ent[n].cmdnum--;
      break;
   case 3:
      moveleft (n);
      if (emoved)
         g_ent[n].cmdnum--;
      break;
   case 4:
      moveright (n);
      if (emoved)
         g_ent[n].cmdnum--;
      break;
   case 5:
      g_ent[n].cmdnum--;
      break;
   case 6:
      return;
   case 7:
      g_ent[n].sidx = 0;
      g_ent[n].cmdnum = 0;
      break;
   case 8:
      if (g_ent[n].tilex < g_ent[n].cmdnum)
         moveright (n);
      if (g_ent[n].tilex > g_ent[n].cmdnum)
         moveleft (n);
      if (g_ent[n].tilex == g_ent[n].cmdnum)
         g_ent[n].cmdnum = 0;
      break;
   case 9:
      if (g_ent[n].tiley < g_ent[n].cmdnum)
         movedown (n);
      if (g_ent[n].tiley > g_ent[n].cmdnum)
         moveup (n);
      if (g_ent[n].tiley == g_ent[n].cmdnum)
         g_ent[n].cmdnum = 0;
      break;
   case 10:
      g_ent[n].facing = g_ent[n].cmdnum;
      g_ent[n].cmdnum = 0;
      break;
   }
   if (g_ent[n].cmdnum == 0)
      g_ent[n].cmd = 0;
}



/*! \brief Initialise script
 *
 * This is used to set up an entity with a movement script so that
 * it can be automatically controlled.
 *
 * \param   n Entity to process
 * \param   movestring The script
*/
void set_script (int n, char *movestring)
{
   g_ent[n].active = 1;
   g_ent[n].moving = MOVE_NOT;
   g_ent[n].movcnt = 0;
   g_ent[n].cmd = 0;
   g_ent[n].sidx = 0;
   g_ent[n].cmdnum = 0;
   g_ent[n].movemode = 2;
   strncpy (g_ent[n].script, movestring, sizeof (g_ent[n].script));
}



/*! \brief Set position
 *
 * Position an entity manually.
 *
 * \param   en Entity to position
 * \param   ex x-coord
 * \param   ey y-coord
*/
void place_ent (int en, int ex, int ey)
{
   g_ent[en].tilex = ex;
   g_ent[en].tiley = ey;
   g_ent[en].x = g_ent[en].tilex * 16;
   g_ent[en].y = g_ent[en].tiley * 16;
}



/*! \brief Count active entities
 *
 * Force calculation of the 'noe' variable.
*/
void count_entities (void)
{
   int p;

   noe = 0;
   for (p = 0; p < MAX_ENT; p++)
      if (g_ent[p].active == 1)
         noe = p + 1;
}
