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

#include <stdio.h>
#include <string.h>

#include "kq.h"
#include "entity.h"
#include "setup.h"
#include "menu.h"
#include "combat.h"
#include "itemdefs.h"
#include "intrface.h"

/*
   globals
*/
int emoved;

/*
   The main routine that loops through the entity list and processes each
   one.
*/
void process_entities(void)
{
  int i;

  for (i=0;i<MAX_ENT;i++)
    if (g_ent[i].active == 1)
      speed_adjust(i);
}

/*
   This has to adjust for each entity's speed.
*/
void speed_adjust(int i)
{
  if (g_ent[i].speed < 4)
  {
    switch (g_ent[i].speed)
    {
      case 1:
        if (g_ent[i].scount < 3)
        {
          g_ent[i].scount++;
          return;
        }
        break;
      case 2:
        if (g_ent[i].scount < 2)
        {
          g_ent[i].scount++;
          return;
        }
        break;
      case 3:
        if (g_ent[i].scount < 1)
        {
          g_ent[i].scount++;
          return;
        }
        break;
    }
  }
  if (g_ent[i].speed < 5)
    process_entity(i);
  switch (g_ent[i].speed)
  {
    case 5:
      process_entity(i);
      process_entity(i);
      break;
    case 6:
      process_entity(i);
      process_entity(i);
      process_entity(i);
      break;
    case 7:
      process_entity(i);
      process_entity(i);
      process_entity(i);
      process_entity(i);
      break;
  }
  if (key[kctrl] && i < PSIZE)
    process_entity(i);
}

/*
   Process an individual active entity.  If the entity in question
   is #0 (main character) and the party is not automated, then allow
   for player input.
*/
void process_entity(int i)
{
  g_ent[i].scount = 0;
  if (g_ent[i].active)
  {
    if (g_ent[i].moving == 0)
    {
      if (i == 0 && !autoparty)
      {
        player_move();
        if (g_ent[i].moving > 0 && display_desc == 1)
          display_desc = 0;
        return;
      }
      if (g_ent[i].movemode == 0)
        return;
      if (g_ent[i].movemode == 1)
        wander(i);
      if (g_ent[i].movemode == 2)
        entscript(i);
      if (g_ent[i].movemode == 3)
      {
        if (g_ent[i].chasing == 0)
        {
          if (entity_near(i,0,3) == 1 && rand()%100 <= g_ent[i].extra)
          {
            g_ent[i].chasing = 1;
            if (g_ent[i].speed < 7)
              g_ent[i].speed++;
            g_ent[i].delay = 0;
            if (g_ent[0].tilex > g_ent[i].tilex)
              moveright(i);
            if (g_ent[0].tilex < g_ent[i].tilex && emoved == 0)
              moveleft(i);
            if (g_ent[0].tiley > g_ent[i].tiley && emoved == 0)
              movedown(i);
            if (g_ent[0].tiley < g_ent[i].tiley && emoved == 0)
              moveup(i);
            if (emoved == 0)
              wander(i);
          }
          else
            wander(i);
        }
        else
        {
          if (entity_near(i,0,4) == 1)
          {
            if (g_ent[0].tilex > g_ent[i].tilex)
              moveright(i);
            if (g_ent[0].tilex < g_ent[i].tilex && emoved == 0)
              moveleft(i);
            if (g_ent[0].tiley > g_ent[i].tiley && emoved == 0)
              movedown(i);
            if (g_ent[0].tiley < g_ent[i].tiley && emoved == 0)
              moveup(i);
            if (emoved == 0)
              wander(i);
          }
          else
          {
            g_ent[i].chasing = 0;
            if (g_ent[i].speed > 1)
              g_ent[i].speed--;
            g_ent[i].delay = 25+rand()%25;
            wander(i);
          }
        }
      }
    }
    else
    {
      if (g_ent[i].moving == MOVE_DOWN)
      {
        g_ent[i].y++;
        g_ent[i].movcnt--;
        g_ent[i].framectr++;
      }
      if (g_ent[i].moving == MOVE_UP)
      {
        g_ent[i].y--;
        g_ent[i].movcnt--;
        g_ent[i].framectr++;
      }
      if (g_ent[i].moving == MOVE_LEFT)
      {
        g_ent[i].x--;
        g_ent[i].movcnt--;
        g_ent[i].framectr++;
      }
      if (g_ent[i].moving == MOVE_RIGHT)
      {
        g_ent[i].x++;
        g_ent[i].movcnt--;
        g_ent[i].framectr++;
      }
      if (g_ent[i].framectr > 20)
        g_ent[i].framectr = 0;
      if (g_ent[i].movcnt == 0 && g_ent[i].moving > 0)
      {
        g_ent[i].moving = 0;
        if (i < PSIZE)
        {
          steps++;
          if (steps >= STEPS_NEEDED)
            steps = STEPS_NEEDED;
          if (party[pidx[i]].sts[S_POISON] > 0)
          {
            party[pidx[i]].hp--;
            if (party[pidx[i]].hp < 1)
              party[pidx[i]].hp = 1;
            play_effect(21,128);
          }
          if (party[pidx[i]].eqp[5] == I_REGENERATOR)
          {
            party[pidx[i]].hp++;
            if (party[pidx[i]].hp > party[pidx[i]].mhp)
              party[pidx[i]].hp = party[pidx[i]].mhp;
          }
        }
        if (i == 0)
          zone_check();
      }
      if (i == 0 && vfollow == 1)
        calc_viewport(0);
    }
  }
}

/*
   Check to see if the target is within rad squares.
*/
int entity_near(int eno, int tgt, int rad)
{
  int ax,ay,ex,ey,b;

  b = 0-rad;
  ex = g_ent[eno].tilex;
  ey = g_ent[eno].tiley;
  for (ay=b;ay<=rad;ay++)
    for (ax=b;ax<=rad;ax++)
      if (ex+ax >= view_x1 && ax+ax <= view_x2 && ey+ay >= view_y1 && ey+ay <= view_y2)
        if (ex+ax == g_ent[tgt].tilex && ey+ay == g_ent[tgt].tiley)
          return 1;
  return 0;
}

/*
   This is used to co-ordinate the following of party
   members after the first.
*/
void lastm_check(int lm)
{
  int i;

  for (i=numchrs-1;i>0;i--)
    lastm[i] = lastm[i-1];
  lastm[0] = lm;
}

/*
   This makes any characters (after the first) follow
   the leader.
*/
void follow(void)
{
  int i;

  if (numchrs == 1)
    return;
  for (i=1;i<numchrs;i++)
  {
    if (lastm[i] == 0)
      return;
    g_ent[i].facing = lastm[i]-1;
    g_ent[i].moving = lastm[i];
    g_ent[i].movcnt = 15;
    switch(lastm[i])
    {
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

/*
   Choose a random direction for the entity to walk in and set up the
   vars to do so.
*/
void wander(int i)
{
  if (g_ent[i].delayctr < g_ent[i].delay)
  {
    g_ent[i].delayctr++;
    return;
  }
  g_ent[i].delayctr = 0;
  switch (rand()%8)
  {
    case 0: moveup(i); break;
    case 1: movedown(i); break;
    case 2: moveleft(i); break;
    case 3: moveright(i); break;
  }
}

/*
   This is the replacement for process_controls that used to be in kq.c
   I realized that all the work in process_controls was already being
   done in process_entity... I just had to make this exception for the
   player controlled dude. 
*/
void player_move(void)
{
  readcontrols();

  if (balt)
    activate();
  if (benter)
    menu();
  if (key[KEY_F10])
  {
    unpress();
    do_luacheat();
  }
  if (right)
  {
    moveright(0);
    if (g_ent[0].moving > 0)
    {
      lastm_check(MOVE_RIGHT);
      follow();
    }
    return;
  }
  if (down)
  {
    movedown(0);
    if (g_ent[0].moving > 0)
    {
      lastm_check(MOVE_DOWN);
      follow();
    }
    return;
  }
  if (left)
  {
    moveleft(0);
    if (g_ent[0].moving > 0)
    {
      lastm_check(MOVE_LEFT);
      follow();
    }
    return;
  }
  if (up)
  {
    moveup(0);
    if (g_ent[0].moving > 0)
    {
      lastm_check(MOVE_UP);
      follow();
    }
    return;
  }
}

/*
   Set up the entity vars to move upwards (if possible).
*/
void moveup(int i)
{
  int tx, ty;

  emoved = 0;
  tx = g_ent[i].x/16;
  ty = g_ent[i].y/16;
  g_ent[i].facing = FACE_UP;
  if (ty == 0)
    return;
  if (g_ent[i].obsmode == 1 && (obstruction(tx,ty,0,-1) || entityat(tx,ty-1,i)))
    return;
  g_ent[i].tiley--;
  g_ent[i].moving=MOVE_UP;
  g_ent[i].movcnt=15;
  g_ent[i].y--;
  emoved = 1;
}

/*
   Set up the entity vars to move down (if possible).
*/
void movedown(int i)
{
  int tx, ty;

  emoved = 0;
  tx = g_ent[i].x/16;
  ty = g_ent[i].y/16;
  g_ent[i].facing = FACE_DOWN;
  if (ty == g_map.ysize-1)
    return;
  if (g_ent[i].obsmode == 1 && (obstruction(tx,ty,0,1) || entityat(tx,ty+1,i)))
    return;
  g_ent[i].tiley++;
  g_ent[i].moving=MOVE_DOWN;
  g_ent[i].movcnt=15;
  g_ent[i].y++;
  emoved = 1;
}

/*
   Set up the entity vars to move right (if possible).
*/
void moveright(int i)
{
  int tx, ty;

  emoved = 0;
  tx = g_ent[i].x/16;
  ty = g_ent[i].y/16;
  g_ent[i].facing = FACE_RIGHT;
  if (tx == g_map.xsize-1)
    return;
  if (g_ent[i].obsmode == 1 && (obstruction(tx,ty,1,0) || entityat(tx+1,ty,i)))
    return;
  g_ent[i].tilex++;
  g_ent[i].moving=MOVE_RIGHT;
  g_ent[i].movcnt=15;
  g_ent[i].x++;
  emoved = 1;
}

/*
   Set up the entity vars to move left (if possible).
*/
void moveleft(int i)
{
  int tx, ty;

  emoved = 0;
  tx = g_ent[i].x/16;
  ty = g_ent[i].y/16;
  g_ent[i].facing = FACE_LEFT;
  if (tx == 0)
    return;
  if (g_ent[i].obsmode == 1 && (obstruction(tx,ty,-1,0) || entityat(tx-1,ty,i)))
    return;
  g_ent[i].tilex--;
  g_ent[i].moving=MOVE_LEFT;
  g_ent[i].movcnt=15;
  g_ent[i].x--;
  emoved = 1;
}

/*
   Check for any map based obstructions in the specified co-ordinates.
*/
int obstruction(int ox, int oy, int mx, int my)
{
  int son,sto,tox,toy;

  tox = ox+mx;
  if (tox < 0 || tox > g_map.xsize-1)
    return 1;
  toy = oy+my;
  if (toy < 0 || toy > g_map.ysize-1)
    return 1;
  son = o_seg[(oy*g_map.xsize)+ox];
  sto = o_seg[(toy*g_map.xsize)+tox];
  if (sto == 1)
    return 1;
  if (mx == 0)
  {
    if (my == -1)
    {
      if (son == 2 || sto == 4)
        return 1;
    }
    if (my == 1)
    {
      if (son == 4 || sto == 2)
        return 1;
    }
  }
  if (my == 0)
  {
    if (mx == -1)
    {
      if (son == 5 || sto == 3)
        return 1;
    }
    if (mx == 1)
    {
      if (son == 3 || sto == 5)
        return 1;
    }
  }
  return 0;
}

/*
   Check for any entities in the specified co-ordinates.
*/
int entityat(int ox, int oy, int who)
{
  int i;

  for (i=0;i<MAX_ENT;i++)
  {
    if (g_ent[i].active)
    {
      if (ox == g_ent[i].tilex && oy == g_ent[i].tiley)
      {
        if (who >= PSIZE)
        {
          if (g_ent[who].eid == ID_ENEMY && i < PSIZE)
          {
            if (combat_check(ox,oy) == 1)
              g_ent[who].active = 0;
            return 0;
          }
          return i+1;
        }
        else
        {
          if (g_ent[i].eid == ID_ENEMY)
          {
            if (combat_check(ox,oy) == 1)
              g_ent[i].active = 0;
            return 0;
          }
          if (i >= PSIZE)
            return i+1;
        }
      }
    }
  }
  return 0;
}

/*
   This parses the movement script for a value that relates 
   to a command.  This is from Verge1.
*/
void parsems(int n)
{
  int p=0;
  char tok[10];
  char s;

  s = g_ent[n].script[g_ent[n].sidx];
  while (s >= 48 && s <= 57)
  {
    tok[p] = s;
    g_ent[n].sidx++;
    s = g_ent[n].script[g_ent[n].sidx];
    p++;
  }
  tok[p] = 0;
  g_ent[n].cmdnum = atoi(tok);
}

/*
   This processes entity commands from the movement script.
   This is from Verge1.
*/
void getcommand(int n)
{
  char s;

  s = g_ent[n].script[g_ent[n].sidx++];
  switch (s)
  {
    case 'U':
      g_ent[n].cmd = 1;
      parsems(n);
      break;
    case 'D':
      g_ent[n].cmd = 2;
      parsems(n);
      break;
    case 'L':
      g_ent[n].cmd = 3;
      parsems(n);
      break;
    case 'R':
      g_ent[n].cmd = 4;
      parsems(n);
      break;
    case 'W':
      g_ent[n].cmd = 5;
      parsems(n);
      break;
    case 0:
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
      parsems(n);
      break;
    case 'Y':
      g_ent[n].cmd = 9;
      parsems(n);
      break;
    case 'F':
      g_ent[n].cmd = 10;
      parsems(n);
      break;
    default:
      program_death("Invalid entity command!");
  }
}

/*
   This executes script commands.  This is from Verge1.
*/
void entscript(int n)
{
  if (g_ent[n].cmd == 0)
    getcommand(n);
  switch (g_ent[n].cmd)
  {
    case 1:
      moveup(n);
      if (emoved)
        g_ent[n].cmdnum--;
      break;
    case 2:
      movedown(n);
      if (emoved)
        g_ent[n].cmdnum--;
      break;
    case 3:
      moveleft(n);
      if (emoved)
        g_ent[n].cmdnum--;
      break;
    case 4:
      moveright(n);
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
        moveright(n);
      if (g_ent[n].tilex > g_ent[n].cmdnum)
        moveleft(n);
      if (g_ent[n].tilex == g_ent[n].cmdnum)
        g_ent[n].cmdnum = 0;
      break;
    case 9:
      if (g_ent[n].tiley < g_ent[n].cmdnum)
        movedown(n);
      if (g_ent[n].tiley > g_ent[n].cmdnum)
        moveup(n);
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

/*
   This is used to set up an entity with a movement script so that
   it can be automatically controlled.
*/
void set_script(int n, char *movestring)
{
  g_ent[n].active = 1;
  g_ent[n].moving = MOVE_NOT;
  g_ent[n].movcnt = 0;
  g_ent[n].cmd = 0;
  g_ent[n].sidx = 0;
  g_ent[n].cmdnum = 0;
  g_ent[n].movemode = 2;
  strcpy(g_ent[n].script,movestring);
}

/*
   Position an entity manually.
*/
void place_ent(int en, int ex, int ey)
{
  g_ent[en].tilex = ex;
  g_ent[en].tiley = ey;
  g_ent[en].x = g_ent[en].tilex*16;
  g_ent[en].y = g_ent[en].tiley*16;
}

/*
   Force calculation of the 'noe' variable.
*/
void count_entities(void)
{
  int p;

  noe = 0;
  for (p=0;p<MAX_ENT;p++)
    if (g_ent[p].active == 1)
      noe = p+1;
}

