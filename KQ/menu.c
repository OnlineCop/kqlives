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
#include "menu.h"
#include "draw.h"
#include "selector.h"
#include "setup.h"
#include "itemmenu.h"
#include "masmenu.h"
#include "eqpmenu.h"
#include "res.h"
#include "progress.h"
#include "itemdefs.h"

/*
   Main menu that calls all the other little menus :)
*/
void menu(void)
{
  int st=0,rd=1,ptr=0,z;

  play_effect(SND_MENU,128);
  while (st == 0)
  {
    if (rd == 1)
    {
      drawmap();
      draw_mainmenu(-1);
      draw_sprite(double_buffer,menuptr,204+xofs,ptr*8+73+yofs);
      blit2screen(xofs,yofs);
    }
    rd = 0;
    readcontrols();
    if (up)
    {
      unpress();
      ptr--;
      if (ptr < 0)
        ptr = 4;
      play_effect(SND_CLICK,128);
      rd = 1;
    }
    if (down)
    {
      unpress();
      ptr++;
      if (ptr > 4)
        ptr = 0;
      play_effect(SND_CLICK,128);
      rd = 1;
    }
    if (balt)
    {
      unpress();
      if (ptr == 0 || ptr == 3)
      {
        if (ptr == 0)
          camp_item_menu();
        else
          spec_items();
      }
      else
      {
        z = select_player();
        if (z >= 0)
        {
          switch(ptr)
          {
            case 1:
              camp_spell_menu(z);
              break;
            case 2:
              equip_menu(z);
              break;
            case 4:
              status_screen(z);
              break;
          }
        }
      }
      rd = 1;
    }
    if (bctrl)
    {
      unpress();
      st = 1;
    }
    if (close_menu == 1)
    {
      close_menu = 0;
      st = 1;
    }
  }
}

/*
   Draw the main menu that is used above.
*/
void draw_mainmenu(int swho)
{
  int p;

  drawmap();
  for (p=0;p<2;p++)
    menubox(double_buffer,44+xofs,p*56+64+yofs,18,5,BLUE);
  menubox(double_buffer,204+xofs,64+yofs,7,5,BLUE);
  menubox(double_buffer,204+xofs,120+yofs,7,5,BLUE);
  print_font(double_buffer,220+xofs,72+yofs,"Items",FGOLD);
  print_font(double_buffer,220+xofs,80+yofs,"Magic",FGOLD);
  print_font(double_buffer,220+xofs,88+yofs,"Equip",FGOLD);
  print_font(double_buffer,220+xofs,96+yofs,"Spec.",FGOLD);
  print_font(double_buffer,220+xofs,104+yofs,"Stats",FGOLD);
  print_font(double_buffer,212+xofs,128+yofs,"Time:",FGOLD);
  if (kmin < 10)
    sprintf(strbuf,":0%d",kmin);
  else
    sprintf(strbuf,":%d",kmin);
  print_font(double_buffer,244+xofs,136+yofs,strbuf,FNORMAL);
  sprintf(strbuf,"%d",khr);
  print_font(double_buffer,244-(strlen(strbuf)*8)+xofs,136+yofs,strbuf,FNORMAL);
  print_font(double_buffer,212+xofs,152+yofs,"Gold:",FGOLD);
  sprintf(strbuf,"%d",gp);
  print_font(double_buffer,268-(strlen(strbuf)*8)+xofs,160+yofs,strbuf,FNORMAL);
  if (swho != -1)
    menubox(double_buffer,44+xofs,swho*56+64+yofs,18,5,DARKBLUE);
  for (p=0;p<numchrs;p++)
    draw_playerstat(double_buffer,pidx[p],52+xofs,p*56+72+yofs);
}

/*
   Draw the terse stats of a single player.
*/
void draw_playerstat(BITMAP *where, int i, int dx, int dy)
{
  int j;

  draw_sprite(where,portrait[i],dx,dy);
  print_font(where,dx+48,dy,party[i].name,FNORMAL);
  draw_stsicon(where,0,i,8,dx+48,dy+8);
  print_font(where,dx+48,dy+16,"LV",FGOLD);
  sprintf(strbuf,"%d",party[i].lvl);
  print_font(where,dx+104-(strlen(strbuf)*8),dy+16,strbuf,FNORMAL);
  print_font(where,dx+48,dy+24,"HP",FGOLD);
  print_font(where,dx+104,dy+24,"/",FNORMAL);
  sprintf(strbuf,"%d",party[i].hp);
  j = strlen(strbuf)*8;
  print_font(where,dx+104-j,dy+24,strbuf,FNORMAL);
  sprintf(strbuf,"%d",party[i].mhp);
  j = strlen(strbuf)*8;
  print_font(where,dx+144-j,dy+24,strbuf,FNORMAL);
  print_font(where,dx+48,dy+32,"MP",FGOLD);
  print_font(where,dx+104,dy+32,"/",FNORMAL);
  sprintf(strbuf,"%d",party[i].mp);
  j = strlen(strbuf)*8;
  print_font(where,dx+104-j,dy+32,strbuf,FNORMAL);
  sprintf(strbuf,"%d",party[i].mmp);
  j = strlen(strbuf)*8;
  print_font(where,dx+144-j,dy+32,strbuf,FNORMAL);
}

/*
   List any special items that the party has.
*/
void spec_items(void)
{
  int a,ii=0,stop=0,ptr=0,rd=1;
  char silist[20][20],spicon[20];
  char sidesc[20][20],siq[20];

  if (progress[P_UCOIN] == 3)
  {
    strcpy(silist[ii],"Unadium coin");
    strcpy(sidesc[ii],"Use to reach ruins");
    siq[ii] = 1;
    spicon[ii] = 50;
    ii++;
  }
  if (progress[P_CANCELROD] == 1)
  {
    strcpy(silist[ii],"Cancellation Rod");
    strcpy(sidesc[ii],"Nullify magic");
    siq[ii] = 1;
    spicon[ii] = 51;
    ii++;
  }
  if (progress[P_GOBLINITEM] == 1)
  {
    strcpy(silist[ii],"Jade Pendant");
    strcpy(sidesc[ii],"Magical goblin gem");
    siq[ii] = 1;
    spicon[ii] = 52;
    ii++;
  }
  if (progress[P_UNDEADJEWEL] == 1)
  {
    strcpy(silist[ii],"Goblin Jewel");
    strcpy(sidesc[ii],"Precious artifact");
    siq[ii] = 1;
    spicon[ii] = 53;
    ii++;
  }
  if (progress[P_WSTONES] > 0)
  {
    strcpy(silist[ii],"White Stone");
    strcpy(sidesc[ii],"Smooth white rock");
    siq[ii] = progress[P_WSTONES];
    spicon[ii] = 54;
    ii++;
  }
  if (progress[P_BSTONES] > 0)
  {
    strcpy(silist[ii],"Black Stone");
    strcpy(sidesc[ii],"Smooth black rock");
    siq[ii] = progress[P_BSTONES];
    spicon[ii] = 55;
    ii++;
  }
  if (progress[P_EMBERSKEY] == 2)
  {
    strcpy(silist[ii],"Ember's Key");
    strcpy(sidesc[ii],"Unlock stuff");
    siq[ii] = 1;
    spicon[ii] = 56;
    ii++;
  }
  if (progress[P_BRONZEKEY] == 1)
  {
    strcpy(silist[ii],"Bronze Key");
    strcpy(sidesc[ii],"Unlock stuff");
    siq[ii] = 1;
    spicon[ii] = 57;
    ii++;
  }
  if (progress[P_DENORIAN] == 3 || progress[P_DENORIAN] == 4)
  {
    strcpy(silist[ii],"Denorian Statue");
    strcpy(sidesc[ii],"An idol");
    siq[ii] = 1;
    spicon[ii] = 58;
    ii++;
  }
  if (progress[P_OPALHELMET] == 1)
  {
    strcpy(silist[ii],"Opal Helmet");
    strcpy(sidesc[ii],"Piece of opal set");
    siq[ii] = 1;
    spicon[ii] = 59;
    ii++;
  }
  if (progress[P_OPALSHIELD] == 1)
  {
    strcpy(silist[ii],"Opal Shield");
    strcpy(sidesc[ii],"Piece of opal set");
    siq[ii] = 1;
    spicon[ii] = 60;
    ii++;
  }
  if (progress[P_IRONKEY] == 1)
  {
    strcpy(silist[ii],"Iron Key");
    strcpy(sidesc[ii],"Unlock stuff");
    siq[ii] = 1;
    spicon[ii] = 61;
    ii++;
  }
  if (progress[P_OPALBAND] == 1)
  {
    strcpy(silist[ii],"Opal Band");
    strcpy(sidesc[ii],"Piece of opal set");
    siq[ii] = 1;
    spicon[ii] = 62;
    ii++;
  }
  if (progress[P_CAVEKEY] == 1)
  {
    strcpy(silist[ii],"Cave Key");
    strcpy(sidesc[ii],"Unlock stuff");
    siq[ii] = 1;
    spicon[ii] = 63;
    ii++;
  }
  if (ii == 0)
  {
    play_effect(SND_BAD,128);
    return;
  }
  play_effect(SND_MENU,128);
  while (!stop)
  {
    if (rd == 1)
    {
      drawmap();
      menubox(double_buffer,72+xofs,12+yofs,20,1,BLUE);
      print_font(double_buffer,108+xofs,20+yofs,"Special Items",FGOLD);
      menubox(double_buffer,72+xofs,36+yofs,20,19,BLUE);
      for (a=0;a<ii;a++)
      {
        draw_icon(double_buffer,spicon[a],88+xofs,a*8+44+yofs);
        print_font(double_buffer,96+xofs,a*8+44+yofs,silist[a],FNORMAL);
        if (siq[a] > 1)
        {
          sprintf(strbuf,"^%d",siq[a]);
          print_font(double_buffer,224+xofs,a*8+68+yofs,strbuf,FNORMAL);
        }
      }
      menubox(double_buffer,72+xofs,204+yofs,20,1,BLUE);
      a = strlen(sidesc[ptr])*4;
      print_font(double_buffer,160-a+xofs,212+yofs,sidesc[ptr],FNORMAL);
      draw_sprite(double_buffer,menuptr,72+xofs,ptr*8+44+yofs);
      blit2screen(xofs,yofs);
    }
    readcontrols();
    rd = 0;
    if (down)
    {
      unpress();
      ptr++;
      if (ptr > ii-1)
        ptr = 0;
      play_effect(SND_CLICK,128);
      rd = 1;
    }
    if (up)
    {
      unpress();
      ptr--;
      if (ptr < 0)
        ptr = ii-1;
      play_effect(SND_CLICK,128);
      rd = 1;
    }
    if (bctrl)
    {
      unpress();
      stop = 1;
    }
  }
}

/*
   Draw the verbose stats of a single player.
*/
void status_screen(int ch)
{
  int stop=0;
  int c,rd=1,p,i,z=0,bc=0;

  play_effect(SND_MENU,128);
  c = pidx[ch];
  update_equipstats();
  while (!stop)
  {
    if (rd == 1)
    {
      drawmap();
      menubox(double_buffer,xofs,16+yofs,18,5,BLUE);
      draw_playerstat(double_buffer,c,8+xofs,24+yofs);
      menubox(double_buffer,xofs,72+yofs,18,17,BLUE);
      print_font(double_buffer,8+xofs,80+yofs,"Exp:",FGOLD);
      sprintf(strbuf,"%d",party[c].xp);
      print_font(double_buffer,152-(strlen(strbuf)*8)+xofs,80+yofs,strbuf,FNORMAL);
      print_font(double_buffer,8+xofs,88+yofs,"Next:",FGOLD);
      if (party[c].lvl < 50)
        sprintf(strbuf,"%d",party[c].next-party[c].xp);
      else
        sprintf(strbuf,"%d",0);
      print_font(double_buffer,152-(strlen(strbuf)*8)+xofs,88+yofs,strbuf,FNORMAL);
      print_font(double_buffer,8+xofs,104+yofs,"Strength",FGOLD);
      print_font(double_buffer,8+xofs,112+yofs,"Agility",FGOLD);
      print_font(double_buffer,8+xofs,120+yofs,"Vitality",FGOLD);
      print_font(double_buffer,8+xofs,128+yofs,"Intellect",FGOLD);
      print_font(double_buffer,8+xofs,136+yofs,"Sagacity",FGOLD);
      print_font(double_buffer,8+xofs,144+yofs,"Speed",FGOLD);
      print_font(double_buffer,8+xofs,152+yofs,"Aura",FGOLD);
      print_font(double_buffer,8+xofs,160+yofs,"Spirit",FGOLD);
      print_font(double_buffer,8+xofs,176+yofs,"Attack",FGOLD);
      print_font(double_buffer,8+xofs,184+yofs,"Hit",FGOLD);
      print_font(double_buffer,8+xofs,192+yofs,"Defense",FGOLD);
      print_font(double_buffer,8+xofs,200+yofs,"Evade",FGOLD);
      print_font(double_buffer,8+xofs,208+yofs,"Mag.Def",FGOLD);
      for (p=0;p<13;p++)
      {
        i = p*8+104;
        if (p > 7)
          i+=8;
        print_font(double_buffer,96+xofs,i+yofs,"$",FGOLD);
        sprintf(strbuf,"%d",fighter[ch].stats[p]);
        print_font(double_buffer,152-(strlen(strbuf)*8)+xofs,i+yofs,strbuf,FNORMAL);
      }

      menubox(double_buffer,160+xofs,16+yofs,18,16,BLUE);
      print_font(double_buffer,168+xofs,24+yofs,"Earth",FNORMAL);
      print_font(double_buffer,168+xofs,32+yofs,"Black",FNORMAL);
      print_font(double_buffer,168+xofs,40+yofs,"Fire",FNORMAL);
      print_font(double_buffer,168+xofs,48+yofs,"Thunder",FNORMAL);
      print_font(double_buffer,168+xofs,56+yofs,"Air",FNORMAL);
      print_font(double_buffer,168+xofs,64+yofs,"White",FNORMAL);
      print_font(double_buffer,168+xofs,72+yofs,"Water",FNORMAL);
      print_font(double_buffer,168+xofs,80+yofs,"Ice",FNORMAL);
      print_font(double_buffer,168+xofs,88+yofs,"Poison",FNORMAL);
      print_font(double_buffer,168+xofs,96+yofs,"Blind",FNORMAL);
      print_font(double_buffer,168+xofs,104+yofs,"Charm",FNORMAL);
      print_font(double_buffer,168+xofs,112+yofs,"Paralyze",FNORMAL);
      print_font(double_buffer,168+xofs,120+yofs,"Petrify",FNORMAL);
      print_font(double_buffer,168+xofs,128+yofs,"Silence",FNORMAL);
      print_font(double_buffer,168+xofs,136+yofs,"Sleep",FNORMAL);
      print_font(double_buffer,168+xofs,144+yofs,"Time",FNORMAL);

      for (i=0;i<16;i++)
      {
        rectfill(double_buffer,240+xofs,i*8+25+yofs,310+xofs,i*8+31+yofs,3);
        if (fighter[ch].res[i] > 10)
        {
          bc = 50;
          z = fighter[ch].res[i]-10;
        }
        if (fighter[ch].res[i] < 0)
        {
          bc = 18;
          z = abs(fighter[ch].res[i]);
        }
        if (fighter[ch].res[i] >= 0 && fighter[ch].res[i] <= 10)
        {
          bc = 34;
          z = fighter[ch].res[i];
        }
        if (z > 0)
          for (p=0;p<z;p++)
            rectfill(double_buffer,p*7+241+xofs,i*8+26+yofs,p*7+246+xofs,i*8+30+yofs,bc+p);
      }
      menubox(double_buffer,160+xofs,160+yofs,18,6,BLUE);
      for (i=0;i<6;i++)
      {
        draw_icon(double_buffer,items[party[c].eqp[i]].icon,168+xofs,i*8+168+yofs);
        print_font(double_buffer,176+xofs,i*8+168+yofs,items[party[c].eqp[i]].name,FNORMAL);
      }
    }
    blit2screen(xofs,yofs);
    readcontrols();
    rd = 0;
    if (left && ch > 0)
    {
      unpress();
      ch--;
      c = pidx[ch];
      play_effect(SND_MENU,128);
      rd = 1;
    }
    if (right && ch < numchrs-1)
    {
      unpress();
      ch++;
      c = pidx[ch];
      play_effect(SND_MENU,128);
      rd = 1;
    }
    if (bctrl)
    {
      unpress();
      play_effect(SND_MENU,128);
      stop = 1;
    } 
  }
}

/*
   Just used to convert all characters in party from party structure
   to fighter structure.
*/
void update_equipstats(void)
{
  int i;
  
  for (i=0;i<numchrs;i++)
    fighter[i] = player2fighter(pidx[i]);
}

/*
   This function converts from the party structure to fighter structure.
   Pass the character index and the function returns a fighter structure.
   This is used for combat and for menu functions.
*/
s_fighter player2fighter(int who)
{
  int j,a,b;
  int z[5] = {5,3,2,1,4};
  s_fighter tf;

  tf.imb_s = 0;
  tf.imb_a = 0;
  tf.imb[0] = 0;
  tf.imb[1] = 0;
  strcpy(tf.name,party[who].name);
  tf.xp = party[who].xp;
  tf.lvl = party[who].lvl;
  tf.hp = party[who].hp;
  tf.mhp = party[who].mhp;
  tf.mp = party[who].mp;
  tf.mmp = party[who].mmp;
  for (j=0;j<24;j++)
    tf.sts[j] = 0;
  for (j=0;j<8;j++)
    tf.sts[j] = party[who].sts[j];
  for (j=0;j<13;j++)
  {
    tf.stats[j] = (party[who].lvl-1)*lup[who][j+4];
    tf.stats[j]+=party[who].stats[j];
    tf.stats[j]/=100;
  }
  for (j=0;j<16;j++)
    tf.res[j] = party[who].res[j];
  /*
     set weapon elemental power and imbuements for easy use in combat
  */
  tf.welem = items[party[who].eqp[0]].elem;
  if (items[party[who].eqp[0]].use == USE_ATTACK)
  {
    tf.imb_s = items[party[who].eqp[0]].imb;
    tf.imb_a = items[party[who].eqp[0]].stats[A_ATT];
  }
  /*
     Set instants for equipment... these are imbuements that
     take effect at the start of combat.  Technically, there
     are only two imbue slots but there are five pieces of equipment
     that can be imbued, so some item types get priority over
     others... hence the need to run through each in this loop.
  */
  for (a=0;a<5;a++)
  {
    if (items[party[who].eqp[z[a]]].use == USE_IMBUED)
    {
      for (b=0;b<2;b++)
      {
        if (tf.imb[b] == 0)
        {
          tf.imb[b] = items[party[who].eqp[z[a]]].imb;
          b = 2;
        }
      }
    }
  }
  /*
     any weapon used by Ajathar gains the power of White if
     it has no other power to begin with
  */
  if (who == AJATHAR && tf.welem == 0)
    tf.welem = 6;
  for (j=0;j<6;j++)
  {
    a = party[who].eqp[j];
    if (j == 0)
    {
      if (a == 0)
        tf.bonus = 50;
      else
        tf.bonus = items[a].bon;
      if (items[a].icon == 1 || items[a].icon == 3 || items[a].icon == 21)
        tf.bstat = 1;
      else
        tf.bstat = 0;
      tf.cwt = items[a].icon;
      if (tf.cwt == W_CHENDIGAL)
        tf.cwt = W_SWORD;
    }
    for (b=0;b<13;b++)
    {
      if (b == A_SPI && who == TEMMIN)
      {
        if (items[a].stats[A_SPI] > 0)
          tf.stats[A_SPI]+=items[a].stats[A_SPI];
      }
      else
        tf.stats[b]+=items[a].stats[b];
    }
    for (b=0;b<16;b++)
      tf.res[b]+=items[a].res[b];
  }
  if (who == CORIN)
  {
    tf.res[R_EARTH]+=tf.lvl/4;
    tf.res[R_FIRE]+=tf.lvl/4;
    tf.res[R_AIR]+=tf.lvl/4;
    tf.res[R_WATER]+=tf.lvl/4;
  }
  if (party[who].eqp[5] == I_AGRAN)
  {
    a = 0;
    for (j=0;j<16;j++)
      a+=tf.res[j];
    b = ((a*10)/16+5)/10;
    for (j=0;j<16;j++)
      tf.res[j] = b;
  }
  for (j=0;j<8;j++)
  {
    if (tf.res[j] < -10)
      tf.res[j] = -10;
    if (tf.res[j] > 20)
      tf.res[j] = 20;
  }
  for (j=8;j<16;j++)
  {
    if (tf.res[j] < 0)
      tf.res[j] = 0;
    if (tf.res[j] > 10)
      tf.res[j] = 10;
  }
  if (party[who].eqp[5] == I_MANALOCKET)
    tf.mrp = party[who].mrp/2;
  else
    tf.mrp = party[who].mrp;
  tf.stats[A_HIT]+=tf.stats[A_STR]/5;
  tf.stats[A_HIT]+=tf.stats[A_AGI]/5;
  tf.stats[A_DEF]+=tf.stats[A_VIT]/8;
  tf.stats[A_EVD]+=tf.stats[A_AGI]/5;
  tf.stats[A_MAG]+=(tf.stats[A_INT]+tf.stats[A_SAG])/20;
  for (j=0;j<13;j++)
    if (tf.stats[j] < 1)
      tf.stats[j] = 1;
  tf.crit = 1;
  tf.aux = 0;
  tf.unl = 0;
  return tf;
}

/*
   This converts from fighter to player structure.  Used when leaving combat.
*/
void revert_equipstats(void)
{
  int i,j,g,nc;

  if (numchrs > PSIZE)
    nc = PSIZE;
  else
    nc = numchrs;
  for (i=0;i<nc;i++)
  {
    g = pidx[i];
    party[g].hp = fighter[i].hp;
    if (party[g].hp > party[g].mhp)
      party[g].hp = party[g].mhp;
    party[g].mp = fighter[i].mp;
    if (party[g].mp > party[g].mmp)
      party[g].mp = party[g].mmp;
    for (j=0;j<12;j++)
      party[g].sts[j] = 0;
    party[g].sts[0] = fighter[i].sts[0];
    party[g].sts[1] = fighter[i].sts[1];
    party[g].sts[5] = fighter[i].sts[5];
    party[g].sts[7] = fighter[i].sts[7];
    for (j=0;j<12;j++)
      if (j != 0 && j != 1 && j != 5 && j != 7)
        fighter[i].sts[j] = 0;
  }
}

/*
   Yep.
*/
int give_xp(int pl, int the_xp, int ls)
{
  party[pl].xp+=the_xp;
  return check_xp(pl,ls);
}

/*
   Checks for level ups.
*/
int check_xp(int pl, int ls)
{
  int stp=0,z=0;

  if (party[pl].lvl >= 50)
    return 0;
  while (!stp)
  {
    if (party[pl].xp >= party[pl].next)
    {
      level_up(pl);
      z = 1;
    }
    else
      stp = 1;
    if (party[pl].lvl >= 50)
      stp = 1;
  }
  if (ls == 1)
    learn_new_spells(pl);
  return z;
}

/*
   Performs a level up.
*/
void level_up(int pr)
{
  int a,b=0;
  float z;
  int bxp,xpi;
  s_fighter tmpf;

  tmpf = player2fighter(pr);
  xpi = lup[pr][0];
  bxp = lup[pr][1];
  party[pr].lvl++;
  a = party[pr].lvl+1;
  z = ((a/3)+(xpi*(a/20+1)-1))*(((a-2)/2)*(a-1));
  z+=(bxp*(a/20+1)*(a-1));
  party[pr].next+=z;
  a = (rand()%(lup[pr][2]/2))+lup[pr][2]+(tmpf.stats[A_VIT]/5);
  party[pr].hp+=a;
  party[pr].mhp+=a;
  b = (rand()%(lup[pr][3]/2))+lup[pr][3];
  b+=(tmpf.stats[A_INT]+tmpf.stats[A_SAG])/25;
  party[pr].mp+=b;
  party[pr].mmp+=b;
}


