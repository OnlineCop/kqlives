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
 * \brief Hero skills
 *
 * \author JB
 * \date ????????
 */

#include <stdio.h>
#include <string.h>

#include "effects.h"
#include "kq.h"
#include "hskill.h"
#include "selector.h"
#include "itemmenu.h"
#include "draw.h"
#include "combat.h"
#include "magic.h"
#include "heroc.h"
#include "setup.h"
#include "res.h"
#include "progress.h"
#include "timing.h"
#include "fade.h"



/*  internal function  */
static void infusion (int, int);


/*! \brief Is hero's special skill available?
 *
 * Check if the hero can use his/her special
 * skill at this point in combat.
 *
 * \param   dude Hero to check (index into pidx[])
 * \returns 1 if skill is available, 0 otherwise
*/
int hero_skillcheck (int dude)
{
   int a, b = 0;

   switch (pidx[dude])
     {
     case SENSAR:
        if (fighter[dude].hp <= fighter[dude].mhp / 10)
           return 0;
        else
           return 1;
        break;
     case SARINA:
        b = items[party[pidx[dude]].eqp[0]].icon;
        if (b != W_SWORD && b != W_AXE && b != W_KNIFE && b != W_CHENDIGAL)
           return 0;
        b = 0;
        for (a = PSIZE; a < PSIZE + numens; a++)
           if (fighter[a].sts[S_DEAD] == 0 && fighter[a].sts[S_STONE] == 0)
              b++;
        if (b > 1)
           return 1;
        else
           return 0;
        break;
     case CORIN:
        if (fighter[dude].sts[S_MUTE] > 0)
           return 0;
        if (fighter[dude].aux == 0)
          {
             fighter[dude].aux = 2;
             b = available_spells (dude);
             fighter[dude].aux = 0;
             if (b > 0)
                return 1;
             else
                return 0;
          }
        else
           return 0;
     case AJATHAR:
        if (fighter[dude].sts[S_MUTE] > 0)
           return 0;
        for (a = PSIZE; a < PSIZE + numens; a++)
           if (fighter[a].sts[S_DEAD] == 0 && fighter[a].sts[S_STONE] == 0)
              b += fighter[a].unl;
        if (b > 0)
           fighter[dude].unl = 1;
        else
           fighter[dude].unl = 0;
        return 1;
        break;
     case CASANDRA:
        if (fighter[dude].sts[S_MUTE] > 0)
           return 0;
        if (fighter[dude].aux == 0)
          {
             fighter[dude].atrack[2] = fighter[dude].mrp;
             fighter[dude].mrp = fighter[dude].mrp * 15 / 10;
             b = available_spells (dude);
             fighter[dude].mrp = fighter[dude].atrack[2];
             if (b > 0)
                return 1;
             else
                return 0;
          }
        else
           return 0;
        break;
     case TEMMIN:
        if (numchrs == 1)
           return 0;
        for (a = 0; a < numchrs; a++)
           if (fighter[a].sts[S_DEAD] == 0)
              b++;
        if (b > 1)
           return 1;
        else
           return 0;
        break;
     case AYLA:
     case NOSLOM:
        return 1;
        break;
     }
   return 0;
}



/*! \brief Activate the special skill 
 *
 * This function activates the special skill for a hero,
 * including targetting etc. if required.
 *
 * \param   who Hero to process
 * \returns 1 if the skill was used, otherwise 0
*/
int skill_use (int who)
{
   int tgt, fitm, a, b, c, p, d = 0, cts, tx, ty, g = 0, nt =
      0, nn[NUM_FIGHTERS];
   BITMAP *temp;

   tempa = status_adjust (who);
   switch (pidx[who])
     {
     case SENSAR:
        tgt = select_enemy (who, 0);
        if (tgt == -1)
           return 0;
        temp = create_bitmap (320, 240);
        blit ((BITMAP *) backart->dat, temp, 0, 0, 0, 0, 320, 240);
        color_scale (temp, (BITMAP *) backart->dat, 16, 31);
        a = fighter[who].mhp / 20;
        strcpy (ctext, "Rage");
        dct = 1;
        tempa.stats[A_ATT] = fighter[who].stats[A_ATT];
        tempa.stats[A_HIT] = fighter[who].stats[A_HIT];
        if (fighter[tgt].crit == 1)
          {
             tempa.stats[A_ATT] += a;
             tempa.stats[A_HIT] += a;
          }
        fight (who, tgt, 1);
        if (fighter[tgt].sts[S_DEAD] == 1)
          {
             for (a = PSIZE; a < PSIZE + numens; a++)
               {
                  if (fighter[a].sts[S_DEAD] == 0)
                    {
                       nn[nt] = a;
                       nt++;
                    }
               }
             if (nt > 0)
               {
                  tgt = nn[rand () % nt];
                  fight (who, tgt, 1);
               }
          }
        /* PH Is this a bug? 'a' is initially set to fighter[who].mhp/20, which makes sense */
        /* however 'a' is re-used in the loop above, so its value is a bit indeterminate here */
        fighter[who].hp -= (a * 2);
        ta[who] = (a * 2);
        dct = 0;
        blit (temp, (BITMAP *) backart->dat, 0, 0, 0, 0, 320, 240);
        display_amount (who, FDECIDE, 0);
        if (fighter[who].sts[S_DEAD] == 0 && fighter[who].hp <= 0)
          {
             fkill (who);
             death_animation (who, 0);
          }
        destroy_bitmap (temp);
        break;
     case SARINA:
        fighter[who].ctmem = 1000;
        strcpy (ctext, "Sweep");
        dct = 1;
        tempa.stats[A_ATT] = tempa.stats[A_ATT] * 75 / 100;
        fighter[who].aframe = 6;
        curx = -1;
        cury = -1;
        battle_render (0, 0, 0);
        blit2screen (0, 0);
        wait (150);
        multi_fight (who);
        dct = 0;
        break;
     case CORIN:
        strcpy (ctext, "Elemental Infusion");
        dct = 1;
        fighter[who].aux = 2;
        if (combat_spell_menu (who) == 1)
          {
             draw_castersprite (who, eff[magic[fighter[who].csmem].eff].kolor);
             curx = -1;
             cury = -1;
             play_effect (22, 128);
             convert_cframes (who,
                              eff[magic[fighter[who].csmem].eff].kolor - 3,
                              eff[magic[fighter[who].csmem].eff].kolor + 3, 0);
             battle_render (0, 0, 0);
             blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
             for (p = 0; p < 2; p++)
               {
                  for (a = 0; a < 16; a++)
                    {
                       tx = fighter[who].cx + (fighter[who].cw / 2);
                       ty = fighter[who].cy + (fighter[who].cl / 2);
                       if (p == 0)
                          circlefill (double_buffer, tx, ty, a,
                                      eff[magic[fighter[who].csmem].eff].kolor);
                       else
                         {
                            circlefill (double_buffer, tx, ty, 15 - a,
                                        eff[magic[fighter[who].csmem].eff].
                                        kolor);
                            draw_fighter (who, 0);
                         }
                       blit2screen (0, 0);
                       wait (50);
                       blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
                    }
               }
             revert_cframes (who, 0);
             battle_render (0, 0, 0);
             blit2screen (0, 0);
             infusion (who, fighter[who].csmem);
             c = mp_needed (who, fighter[who].csmem);
             if (c < 1)
                c = 1;
             fighter[who].mp -= c;
             cact[who] = 0;
             fighter[who].aux = 1;
          }
        else
          {
             fighter[who].aux = 0;
             dct = 0;
             return 0;
          }
        dct = 0;
        fighter[who].sts[S_INFUSE] = magic[fighter[who].csmem].elem;
        break;
     case AJATHAR:
        if (fighter[who].unl > 0)
          {
             strcpy (ctext, "Dispel Undead");
             dct = 1;
             blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
             for (a = 0; a < 14; a++)
               {
                  convert_cframes (PSIZE, 1 + a, 15, 1);
                  for (g = PSIZE; g < PSIZE + numens; g++)
                     if (is_active (g))
                        draw_fighter (g, 0);
                  blit2screen (0, 0);
                  wait (50);
                  blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
               }
             revert_cframes (PSIZE, 1);
             dct = 0;
             b = fighter[who].lvl * 25;
             for (g = PSIZE; g < PSIZE + numens; g++)
               {
                  if (fighter[g].sts[S_DEAD] == 0 && fighter[g].mhp > 0)
                    {
                       if (fighter[g].unl == 99 || fighter[g].unl == 0)
                          cts = 0;
                       else
                         {
                            a = (fighter[who].lvl + 5) - fighter[g].unl;
                            if (a > 0)
                               cts = a * 10;
                            else
                               cts = 0;
                         }
                       if (rand () % 100 < cts)
                         {
                            if (b >= fighter[g].hp)
                              {
                                 b -= fighter[g].hp;
                                 deffect[g] = 1;
                                 fkill (g);
                              }
                         }
                    }
               }
             death_animation (PSIZE, 1);
             curx = -1;
             cury = -1;
             battle_render (who, who, 0);
          }
        else
          {
             a = rand () % 100;
             c = fighter[who].lvl / 10 + 1;
             if (a < 25)
                b = rand () % (5 * c) + 1;
             else
               {
                  if (a < 90)
                     b = rand () % (10 * c) + (20 * c);
                  else
                     b = rand () % (25 * c) + (50 * c);
               }
             strcpy (ctext, "Divine Cure");
             dct = 1;
             draw_spellsprite (0, 1, 15, 1);
             dct = 0;
             for (a = 0; a < numchrs; a++)
               {
                  if (fighter[a].sts[S_STONE] == 0
                      && fighter[a].sts[S_DEAD] == 0)
                    {
                       ta[a] = b;
                       ta[a] = do_shell_check (a, ta[a]);
                    }
               }
             display_amount (0, FYELLOW, 1);
             for (a = 0; a < numchrs; a++)
                if (fighter[a].sts[S_STONE] == 0 && fighter[a].sts[S_DEAD] == 0)
                   adjust_hp (a, ta[a]);
          }
        break;
     case CASANDRA:
        fighter[who].atrack[0] = fighter[who].stats[A_AUR];
        fighter[who].atrack[1] = fighter[who].stats[A_SPI];
        fighter[who].stats[A_AUR] = fighter[who].stats[A_AUR] * 15 / 10;
        fighter[who].stats[A_SPI] = fighter[who].stats[A_SPI] * 15 / 10;
        fighter[who].atrack[2] = fighter[who].mrp;
        fighter[who].mrp = fighter[who].mrp * 15 / 10;
        if (combat_spell_menu (who) == 1)
          {
             cact[who] = 0;
             fighter[who].aux = 1;
             fighter[who].stats[A_AUR] = fighter[who].atrack[0];
             fighter[who].stats[A_SPI] = fighter[who].atrack[1];
             fighter[who].mrp = fighter[who].atrack[2];
          }
        else
          {
             fighter[who].stats[A_AUR] = fighter[who].atrack[0];
             fighter[who].stats[A_SPI] = fighter[who].atrack[1];
             fighter[who].mrp = fighter[who].atrack[2];
             return 0;
          }
        break;
     case TEMMIN:
        fighter[who].aux = 1;
        fighter[who].defend = 1;
        break;
     case AYLA:
        tgt = select_enemy (who, 0);
        if (tgt == -1)
           return 0;
        cts = party[pidx[who]].lvl * 2 + 35;
        if (cts > 95)
           cts = 95;
        tx = fighter[who].cx;
        ty = fighter[who].cy;
        fighter[who].cx = fighter[tgt].cx - 16;
        fighter[who].cy = fighter[tgt].cy + fighter[tgt].cl - 40;
        fighter[who].facing = 1;
        strcpy (ctext, "Steal");
        dct = 1;
        battle_render (0, who + 1, 0);
        blit2screen (0, 0);
        wait (100);
        play_effect (SND_MENU, 128);
        wait (500);
        dct = 0;
        battle_render (who, who, 0);
        fitm = 0;
        if (rand () % 100 < cts)
          {
             if (fighter[tgt].sitmc > 0)
                fitm = fighter[tgt].sitmc;
             if (fighter[tgt].sitmr > 0)
               {
                  b = rand () % 100;
                  if (b < 5)
                    {
                       fitm = fighter[tgt].sitmr;
                       fighter[tgt].sitmr = 0;
                    }
                  else
                    {
                       if (fitm > 0)
                          fighter[tgt].sitmc = 0;
                    }
               }
             if (fitm > 0)
               {
                  if (check_inventory (fitm, 1) != 0)
                    {
                       sprintf (strbuf, "%s taken!", items[fitm].name);
                       message (strbuf, items[fitm].icon, 0, 0, 0);
                    }
               }
             else
               {
                  if (fighter[tgt].sitmc == 0 && fighter[tgt].sitmr == 0)
                     message ("Nothing to steal!", 255, 0, 0, 0);
                  else
                     message ("Couldn't steal!", 255, 0, 0, 0);
               }
          }
        else
           message ("Couldn't steal!", 255, 0, 0, 0);
        fighter[who].cx = tx;
        fighter[who].cy = ty;
        dct = 0;
        fighter[who].facing = 0;
        battle_render (who, who, 0);
        blit2screen (0, 0);
        break;
     case NOSLOM:
        tgt = select_enemy (who, 0);
        if (tgt == -1)
           return 0;
        do_transition (TRANS_FADE_OUT, 4);
        menubox (double_buffer, 84, 56, 17, 13, BLUE);
        sprintf (strbuf, "Name: %s", fighter[tgt].name);
        print_font (double_buffer, 92, 64, strbuf, FNORMAL);
        sprintf (strbuf, "Level: %d", fighter[tgt].lvl);
        print_font (double_buffer, 92, 72, strbuf, FNORMAL);
        sprintf (strbuf, "HP: %d/%d", fighter[tgt].hp, fighter[tgt].mhp);
        print_font (double_buffer, 92, 80, strbuf, FNORMAL);
        sprintf (strbuf, "MP: %d/%d", fighter[tgt].mp, fighter[tgt].mmp);
        print_font (double_buffer, 92, 88, strbuf, FNORMAL);
        print_font (double_buffer, 92, 96, "Earth", FNORMAL);
        print_font (double_buffer, 92, 104, "Black", FNORMAL);
        print_font (double_buffer, 92, 112, "Fire", FNORMAL);
        print_font (double_buffer, 92, 120, "Thunder", FNORMAL);
        print_font (double_buffer, 92, 128, "Air", FNORMAL);
        print_font (double_buffer, 92, 136, "White", FNORMAL);
        print_font (double_buffer, 92, 144, "Water", FNORMAL);
        print_font (double_buffer, 92, 152, "Ice", FNORMAL);
        for (c = 0; c < 8; c++)
          {
             rectfill (double_buffer, 156, c * 8 + 97, 226, c * 8 + 103, 3);
             if (fighter[tgt].res[c] > 10)
               {
                  g = 50;
                  d = fighter[tgt].res[c] - 10;
               }
             if (fighter[tgt].res[c] < 0)
               {
                  g = 18;
                  d = abs (fighter[tgt].res[c]);
               }
             if (fighter[tgt].res[c] >= 0 && fighter[tgt].res[c] <= 10)
               {
                  g = 34;
                  d = fighter[tgt].res[c];
               }
             if (d > 0)
                for (b = 0; b < d; b++)
                   rectfill (double_buffer, b * 7 + 157, c * 8 + 98,
                             b * 7 + 162, c * 8 + 102, g + b);
          }
        blit2screen (0, 0);
        do_transition (TRANS_FADE_IN, 4);
        wait_enter ();
        break;
     }
   return 1;
}



/*! \brief Do infusion skill
 *
 * This function is only used for Corin when he
 * uses his Infuse ability.
 *
 * \param   c Hero (index in fighter[] array)
 * \param   sn Thing to infuse
*/
static void infusion (int c, int sn)
{
   int j;

   switch (sn)
     {
     case M_VENOM:
        fighter[c].res[R_POISON] += 4;
        j = fighter[c].mhp / 10;
        if (j < 10)
           j = 10;
        fighter[c].hp += j;
        fighter[c].mhp += j;
        fighter[c].welem = 8;
        break;
     case M_SCORCH:
        fighter[c].res[R_FIRE] += 4;
        fighter[c].res[R_WATER]--;
        fighter[c].res[R_ICE]--;
        fighter[c].stats[A_ATT] += 10;
        fighter[c].stats[A_HIT] += 10;
        fighter[c].welem = 2;
        break;
     case M_SHOCK:
        fighter[c].res[R_THUNDER] += 3;
        fighter[c].stats[A_EVD] += 10;
        fighter[c].welem = 3;
        break;
     case M_FADE:
        fighter[c].res[R_WHITE] += 5;
        fighter[c].res[R_BLACK] -= 2;
        fighter[c].stats[A_SPI] += 10;
        fighter[c].welem = 5;
        break;
     case M_GLOOM:
        fighter[c].res[R_BLACK] += 8;
        fighter[c].res[R_WHITE] -= 4;
        fighter[c].stats[A_AUR] += 20;
        fighter[c].welem = 1;
        break;
     case M_FROST:
        fighter[c].res[R_ICE] += 7;
        fighter[c].res[R_WATER] += 4;
        fighter[c].res[R_FIRE] -= 5;
        fighter[c].stats[A_DEF] += 10;
        fighter[c].welem = 7;
        break;
     case M_FIREBLAST:
        fighter[c].res[R_FIRE] += 8;
        fighter[c].res[R_WATER] -= 2;
        fighter[c].res[R_ICE] -= 2;
        fighter[c].stats[A_ATT] += 20;
        fighter[c].stats[A_HIT] += 20;
        fighter[c].welem = 2;
        break;
     case M_WHIRLWIND:
        fighter[c].res[R_AIR] += 5;
        fighter[c].stats[A_EVD] += 15;
        fighter[c].stats[A_SPD] += 10;
        fighter[c].welem = 4;
        break;
     case M_TREMOR:
        fighter[c].res[R_EARTH] += 5;
        fighter[c].stats[A_DEF] += 15;
        fighter[c].stats[A_MAG] += 10;
        fighter[c].welem = 0;
        break;
     case M_VIRUS:
        fighter[c].res[R_POISON] += 8;
        j = fighter[c].mhp * 25 / 100;
        if (j < 40)
           j = 40;
        fighter[c].hp += j;
        fighter[c].mhp += j;
        fighter[c].welem = 8;
        break;
     case M_LIGHTNING:
        fighter[c].res[R_THUNDER] += 6;
        fighter[c].stats[A_EVD] += 25;
        fighter[c].welem = 3;
        break;
     case M_FLOOD:
        fighter[c].res[R_WATER] += 5;
        fighter[c].res[R_THUNDER] -= 5;
        for (j = 9; j < 16; j++)
          {
             fighter[c].res[j] += 3;
             if (fighter[c].res[j] > 10)
                fighter[c].res[j] = 10;
          }
        fighter[c].welem = 6;
        break;
     case M_LUMINE:
        fighter[c].res[R_WHITE] += 10;
        fighter[c].res[R_BLACK] -= 5;
        fighter[c].stats[A_SPI] += 25;
        fighter[c].welem = 5;
        break;
     case M_FLAMEWALL:
        fighter[c].res[R_FIRE] += 12;
        fighter[c].res[R_WATER] -= 4;
        fighter[c].res[R_ICE] -= 4;
        fighter[c].stats[A_ATT] += 40;
        fighter[c].stats[A_HIT] += 40;
        fighter[c].welem = 2;
        break;
     case M_BLIZZARD:
        fighter[c].res[R_ICE] += 14;
        fighter[c].res[R_WATER] += 8;
        fighter[c].res[R_FIRE] -= 10;
        fighter[c].stats[A_DEF] += 25;
        fighter[c].welem = 7;
        break;
     case M_TORNADO:
        fighter[c].res[R_AIR] += 10;
        fighter[c].stats[A_EVD] += 30;
        fighter[c].stats[A_SPD] += 20;
        fighter[c].welem = 4;
        break;
     case M_EARTHQUAKE:
        fighter[c].res[R_EARTH] += 10;
        fighter[c].stats[A_DEF] += 30;
        fighter[c].stats[A_MAG] += 20;
        fighter[c].welem = 0;
        break;
     case M_TSUNAMI:
        fighter[c].res[R_WATER] += 10;
        fighter[c].res[R_THUNDER] -= 10;
        for (j = 9; j < 16; j++)
          {
             fighter[c].res[j] += 6;
             if (fighter[c].res[j] > 10)
                fighter[c].res[j] = 10;
          }
        fighter[c].welem = 6;
        break;
     case M_THUNDERSTORM:
        fighter[c].res[R_THUNDER] += 12;
        fighter[c].stats[A_EVD] += 50;
        fighter[c].welem = 3;
        break;
     case M_NEGATIS:
        fighter[c].res[R_BLACK] += 16;
        fighter[c].res[R_WHITE] -= 8;
        fighter[c].stats[A_AUR] += 40;
        fighter[c].welem = 1;
        break;
     case M_PLAGUE:
        fighter[c].res[R_POISON] += 12;
        j = fighter[c].mhp * 4 / 10;
        if (j < 80)
           j = 80;
        fighter[c].hp += j;
        fighter[c].mhp += j;
        fighter[c].welem = 8;
        break;
     }
   for (j = 0; j < 9; j++)
     {
        if (fighter[c].res[j] < -10)
           fighter[c].res[j] = -10;
        if (fighter[c].res[j] > 20)
           fighter[c].res[j] = 20;
     }
}
