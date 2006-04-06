/*
   KQ is Copyright (C) 2002 by Josh Bolduc

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
 * \brief Handles shops
 *
 * \author JB
 * \date ????????
 */

#include <stdio.h>
#include <string.h>

#include "setup.h"
#include "shopmenu.h"
#include "itemmenu.h"
#include "kq.h"
#include "draw.h"
#include "res.h"
#include "progress.h"
#include "itemdefs.h"
#include "fade.h"
#include "music.h"
#include "timing.h"



/* TT add: */
/*! \brief Items in shop
 *
 * vars: 
 * - [NUMSHOPS] = index of shop
 * - [SHOPITEMS] = number of items a shop can sell
 * - [3] = info about items sold:
 *  -   [0] = index of item
 *  -   [1] = quantity of items
 *  -   [2] = quantity of items (special.)
 *            After long gameplay, shops will sell
 *            quantity of #[2] of items instead of #[1]
 *            (see shop() for details)
 *
 * PH: I don't think this is correct. I think
 * the code says 
 * that the shop will replenish its stock
 * to [1], after a time-out of [2] minutes.
 */
unsigned short shops[NUMSHOPS][SHOPITEMS][3] = {
   {{I_SHIELD1, 6, 10},
    {I_CAP1, 4, 10},
    {I_ROBE1, 3, 10},
    {I_ARMOR1, 3, 10},
    {I_GLOVES1, 4, 10},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_SWORD1, 5, 10},
    {I_MACE1, 4, 10},
    {I_KNIFE1, 9, 10},
    {I_STAFF1, 2, 20},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_MHERB, 50, 5},
    {I_NLEAF, 20, 10},
    {I_SALVE, 5, 15},
    {I_B_SHIELD, 2, 25},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_B_CURE1, 1, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_MHERB, 60, 10},
    {I_NLEAF, 20, 10},
    {I_SALVE, 20, 10},
    {I_PCURING, 5, 30},
    {I_B_SLEEP, 1, 30},
    {I_B_SILENCE, 1, 30},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_SPEAR1, 3, 20},
    {I_MACE2, 3, 20},
    {I_ROD1, 2, 30},
    {I_HELM1, 4, 15},
    {I_ROBE2, 2, 30},
    {I_ARMOR2, 2, 20},
    {I_SUIT2, 2, 20},
    {I_BAND1, 4, 10},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_AXE1, 2, 20},
    {I_SWORD2, 2, 30},
    {I_ROD2, 1, 30},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_SHIELD2, 5, 15},
    {I_HELM2, 3, 20},
    {I_ARMOR3, 3, 20},
    {I_GAUNTLET1, 4, 10},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_B_CURE1, 5, 20},
    {I_B_CURE2, 2, 30},
    {I_B_RESTORE, 3, 30},
    {I_B_SCORCH, 4, 20},
    {I_B_DRAIN, 3, 30},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_MHERB, 40, 10},
    {I_OSEED, 10, 20},
    {I_NLEAF, 30, 10},
    {I_NPOULTICE, 20, 10},
    {I_SALVE, 15, 10},
    {I_RRUNE, 4, 30},
    {I_ERUNE, 3, 30},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_SALVE, 30, 10},
    {I_LTONIC, 5, 20},
    {I_EDAENRA, 20, 15},
    {I_SSTONE, 5, 20},
    {I_B_HOLD, 4, 20},
    {I_B_SHELL, 1, 30},
    {I_B_HASTEN, 2, 30},
    {I_B_FLOOD, 1, 30},
    {I_B_SLOW, 2, 20},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_HAMMER1, 2, 30},
    {I_SWORD3, 2, 20},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_CAP2, 3, 30},
    {I_CAP3, 3, 30},
    {I_ROBE3, 2, 30},
    {I_ARMOR4, 5, 20},
    {I_SUIT3, 3, 30},
    {I_BAND2, 12, 10},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_AXE2, 2, 30},
    {I_SHIELD3, 2, 30},
    {I_ARMOR5, 2, 30},
    {I_SUIT3, 2, 30},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_PCURING, 20, 20},
    {I_EDROPS, 10, 20},
    {I_SSTONE, 30, 10},
    {I_POWERBRACE, 1, 0},
    {I_B_REPULSE, 1, 30},
    {I_B_VISION, 2, 20},
    {I_B_VIRUS, 2, 20},
    {I_B_HOLYMIGHT, 3, 20},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_B_LIGHTNING, 2, 30},
    {I_B_DIFFUSE, 1, 10},
    {I_B_FROST, 3, 10},
    {I_B_CONFUSE, 2, 20},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_B_CURE3, 2, 30},
    {I_B_THROUGH, 1, 30},
    {I_B_SHIELDALL, 1, 20},
    {I_B_BLESS, 4, 10},
    {I_B_FADE, 2, 20},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_RRUNE, 9, 20},
    {I_ERUNE, 6, 30},
    {I_FRUNE, 6, 30},
    {I_WRUNE, 6, 30},
    {I_IRUNE, 6, 30},
    {I_PCURING, 30, 10},
    {I_LTONIC, 20, 10},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_HELM3, 4, 20},
    {I_ROBE3, 4, 20},
    {I_ARMOR5, 4, 20},
    {I_SUIT3, 4, 20},
    {I_BAND2, 10, 10},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_HAMMER2, 2, 20},
    {I_ROD4, 1, 30},
    {I_SPEAR2, 2, 10},
    {I_STAFF3, 1, 30},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_WATERRING, 1, 0},
    {I_SSTONE, 20, 10},
    {I_RUNECLOAK, 1, 0},
    {I_SPIRITCAPE, 1, 0},
    {I_TP100S, 90, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_SHIELD4, 1, 20},
    {I_HELM3, 4, 30},
    {I_ARMOR5, 5, 10},
    {I_GAUNTLET3, 1, 30},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_EDROPS, 20, 20},
    {I_PCURING, 30, 10},
    {I_EDAENRA, 30, 10},
    {I_B_WALL, 1, 30},
    {I_B_FIREBLAST, 2, 30},
    {I_B_STONE, 1, 30},
    {I_B_RECOVERY, 3, 10},
    {I_B_REGENERATE, 2, 20},
    {I_B_NAUSEA, 1, 20},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   /* Rufus's shop */
   {{I_DYNAMITE, 20, 20},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}},

   {{I_HAMMER3, 5, 12},
    {I_ROD5, 5, 7},
    {I_SPEAR3, 5, 13},
    {I_STAFF4, 5, 4},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}}
};

/*! \brief Quantities in each shop */
unsigned short shopq[NUMSHOPS][SHOPITEMS];


/*  internal variables  */
/*! \brief Current shop index */
static unsigned char shop_no;
/*! \brief Names of shops */
static char shopn[NUMSHOPS][40] = {
   "Alner's Armour",
   "Shielt's Weaponry",
   "Blaknard's Miscellany",
   "Little Magic Shop",
   "Ignar's Item Shop",
   "Nelgar's Armory",
   "Shend's Weaponry",
   "Brand's Fine Armours",
   "The Library of Andra",
   "Dreskan Item Shop",
   "Golerak's Item Shop",
   "Resdin's Weapons",
   "Nester's Fine Armour",
   "Vezdin's Arms and Armours",
   "Vezdin's Wonderous Items",
   "Black Magic",
   "White Magic",
   "Ajantara Items",
   "Ajantara Armor",
   "Alderic's Weapons",
   "Ezbaran's Items",
   "Agrasian's Armor",
   "The Colosseum Shop",
   "Rufus's Cabin",
   "Bentley's Armour"
};



/*  internal functions  */
static void draw_sideshot (int);
static void buy_menu (void);
static void buy_item (int, int);
static void sell_menu (void);
static void sell_howmany (int, int);
static void sell_item (int, int);



/*! \brief Show status info
 *
 * Well, it used to be on the side, but now it's on the bottom.
 * This displays the characters and whether or not they are
 * able to use/equip what is being looked at, and how it would
 * improve their stats (if applicable).
 *
 * \param   selected_item Item being looked at.
 */
static void draw_sideshot (int selected_item)
{
   int a, j, ownd = 0, eqp = 0, wx, wy, slot;
   int cs[13];

   menubox (double_buffer, 80 + xofs, 192 + yofs, 18, 4, BLUE);
   for (a = 0; a < numchrs; a++) {
      wx = a * 72 + 88 + xofs;
      wy = 200 + yofs;
      draw_sprite (double_buffer, frames[pidx[a]][2], wx, wy);
   }
   if (selected_item == -1)
      return;
   slot = items[selected_item].type;
   for (a = 0; a < numchrs; a++) {
      wx = a * 72 + 88 + xofs;
      wy = 200 + yofs;
      for (j = 0; j < 6; j++)
         if (party[pidx[a]].eqp[j] == selected_item)
            eqp++;
      if (slot < 6) {
         if (party[pidx[a]].eqp[slot] > 0) {
            for (j = 0; j < 13; j++)
               cs[j] =
                  items[selected_item].stats[j] -
                  items[party[pidx[a]].eqp[slot]].stats[j];
         } else {
            for (j = 0; j < 13; j++)
               cs[j] = items[selected_item].stats[j];
         }
         if (slot == 0) {
            draw_icon (double_buffer, 3, wx + 16, wy);
            print_font (double_buffer, wx + 16, wy + 8, "%", FNORMAL);
            for (j = 0; j < 2; j++) {
               if (cs[j + 8] < 0) {
                  sprintf (strbuf, "%-4d", cs[j + 8]);
                  print_font (double_buffer, wx + 24, j * 8 + wy, strbuf,
                              FRED);
               }
               if (cs[j + 8] > 0) {
                  sprintf (strbuf, "+%-3d", cs[j + 8]);
                  print_font (double_buffer, wx + 24, j * 8 + wy, strbuf,
                              FGREEN);
               }
               if (cs[j + 8] == 0)
                  print_font (double_buffer, wx + 24, j * 8 + wy, "=",
                              FNORMAL);
            }
         } else {
            draw_icon (double_buffer, 9, wx + 16, wy);
            print_font (double_buffer, wx + 16, wy + 8, "%", FNORMAL);
            draw_icon (double_buffer, 47, wx + 16, wy + 16);
            for (j = 0; j < 3; j++) {
               if (cs[j + 10] < 0) {
                  sprintf (strbuf, "%-4d", cs[j + 10]);
                  print_font (double_buffer, wx + 24, j * 8 + wy, strbuf,
                              FRED);
               }
               if (cs[j + 10] > 0) {
                  sprintf (strbuf, "+%-3d", cs[j + 10]);
                  print_font (double_buffer, wx + 24, j * 8 + wy, strbuf,
                              FGREEN);
               }
               if (cs[j + 10] == 0)
                  print_font (double_buffer, wx + 24, j * 8 + wy, "=",
                              FNORMAL);
            }
         }
         if (items[selected_item].eq[pidx[a]] == 0)
            draw_sprite (double_buffer, noway, wx, wy);
      } else {
         if (items[selected_item].icon == W_SBOOK
             || items[selected_item].icon == W_ABOOK) {
            for (j = 0; j < 60; j++)
               if (party[pidx[a]].spells[j] == items[selected_item].hnds)
                  draw_sprite (double_buffer, noway, wx, wy);
         }
      }
   }
   for (j = 0; j < MAX_INV; j++)
      if (g_inv[j][0] == selected_item)
         ownd += g_inv[j][1];   // quantity of this item
   sprintf (strbuf, "Own: %d", ownd);
   print_font (double_buffer, 88 + xofs, 224 + yofs, strbuf, FNORMAL);
   if (slot < 6) {
      sprintf (strbuf, "Eqp: %d", eqp);
      print_font (double_buffer, 160 + xofs, 224 + yofs, strbuf, FNORMAL);
   }
}



/*! \brief Display amount of gold
 *
 * Display the party's funds.
 */
void draw_shopgold (void)
{
   menubox (double_buffer, 248 + xofs, 208 + yofs, 7, 2, BLUE);
   print_font (double_buffer, 256 + xofs, 216 + yofs, "Gold:", FGOLD);
   sprintf (strbuf, "%d", gp);
   print_font (double_buffer, 312 - (strlen (strbuf) * 8) + xofs, 224 + yofs,
               strbuf, FNORMAL);
}



/*! \brief Main entry point to shop functions
 *
 * The initial shop dialog.  This function calculates item quantities
 * and then just asks if we're buying or selling.
 *
 * \param   shop_num Index of this shop
 * \returns 1 if shop has no items, 0 otherwise
 */
int shop (int shop_num)
{
   int ptr = 0, stop = 0, a;

   shop_no = shop_num;
   strcpy (sname, shopn[shop_no]);
   for (a = 0; a < SHOPITEMS; a++) {
      if (shops[shop_no][a][2] > 0)
         /* Shops replenish after a certain time? */
         if ((khr * 60) + kmin - progress[P_SHOPSTART + shop_no] >
             shops[shop_no][a][2])
            shopq[shop_no][a] = shops[shop_no][a][1];
      shin[a] = shops[shop_no][a][0];
   }
   noi = SHOPITEMS - 1;
   for (a = SHOPITEMS - 1; a >= 0; a--)
      if (shin[a] == 0)
         noi = a;
   if (noi == 0)
      return 1;
   unpress ();
   play_effect (SND_MENU, 128);
   while (!stop) {
      check_animation ();
      drawmap ();
      menubox (double_buffer, 152 - (strlen (sname) * 4) + xofs, yofs,
               strlen (sname), 1, BLUE);
      print_font (double_buffer, 160 - (strlen (sname) * 4) + xofs,
                  8 + yofs, sname, FGOLD);
      menubox (double_buffer, 32 + xofs, 24 + yofs, 30, 1, BLUE);
      menubox (double_buffer, ptr * 80 + 32 + xofs, 24 + yofs, 10, 1,
               DARKBLUE);
      print_font (double_buffer, 68 + xofs, 32 + yofs, "Buy", FGOLD);
      print_font (double_buffer, 144 + xofs, 32 + yofs, "Sell", FGOLD);
      print_font (double_buffer, 224 + xofs, 32 + yofs, "Exit", FGOLD);
      draw_sideshot (-1);
      draw_shopgold ();
      blit2screen (xofs, yofs);

      readcontrols ();

      if (left && ptr > 0) {
         unpress ();
         ptr--;
         play_effect (SND_CLICK, 128);
      }
      if (right && ptr < 2) {
         unpress ();
         ptr++;
         play_effect (SND_CLICK, 128);
      }
      if (balt) {
         unpress ();
         if (ptr == 0)
            buy_menu ();
         if (ptr == 1)
            sell_menu ();
         if (ptr == 2)
            stop = 1;
      }
      if (bctrl) {
         unpress ();
         stop = 1;
      }
   }
   progress[P_SHOPSTART + shop_no] = khr * 60 + kmin;
   return 0;
}



/*! \brief Show items to buy
 *
 * Show the player a list of items which can be bought
 * and wait for him/her to choose something or exit.
 */
static void buy_menu (void)
{
   int stop = 0, cost;
   int xptr = 1, yptr = 0, k, i, j, a, max, max_x = 0;

   for (a = 0; a < noi; a++)
      if (shopq[shop_no][a] > max_x)
         max_x = shopq[shop_no][a];
   if (max_x > 9)
      max_x = 9;
   while (!stop) {
      check_animation ();
      drawmap ();
      menubox (double_buffer, 152 - (strlen (sname) * 4) + xofs, yofs,
               strlen (sname), 1, BLUE);
      print_font (double_buffer, 160 - (strlen (sname) * 4) + xofs,
                  8 + yofs, sname, FGOLD);
      menubox (double_buffer, xofs, 208 + yofs, 7, 2, BLUE);
      print_font (double_buffer, 24 + xofs, 220 + yofs, "Buy", FGOLD);
      menubox (double_buffer, 32 + xofs, 24 + yofs, 30, 16, BLUE);
      menubox (double_buffer, 32 + xofs, 168 + yofs, 30, 1, BLUE);
      draw_shopgold ();
      for (i = 0; i < noi; i++) {
         j = shin[i];
         max = shopq[shop_no][i];
         if (xptr <= max)
            max = xptr;
         draw_icon (double_buffer, items[j].icon, 48 + xofs,
                    i * 8 + 32 + yofs);
         cost = max * items[j].price;
         if (cost > gp)
            k = FDARK;
         else
            k = FNORMAL;
         print_font (double_buffer, 56 + xofs, i * 8 + 32 + yofs,
                     items[j].name, k);
         if (max > 1) {
            sprintf (strbuf, "(%d)", max);
            print_font (double_buffer, 256 + xofs, i * 8 + 32 + yofs,
                        strbuf, k);
         }
         if (max > 0) {
            sprintf (strbuf, "%d", cost);
            print_font (double_buffer, 248 - (strlen (strbuf) * 8) + xofs,
                        i * 8 + 32 + yofs, strbuf, k);
         } else
            print_font (double_buffer, 200 + xofs, i * 8 + 32 + yofs,
                        "Sold Out!", k);
      }
      print_font (double_buffer,
                  160 - (strlen (items[shin[yptr]].desc) * 4) + xofs,
                  176 + yofs, items[shin[yptr]].desc, FNORMAL);
      draw_sideshot (shin[yptr]);
      draw_sprite (double_buffer, menuptr, 32 + xofs, yptr * 8 + 32 + yofs);
      blit2screen (xofs, yofs);

      readcontrols ();
      if (up) {
         unpress ();
         yptr--;
         if (yptr < 0)
            yptr = noi - 1;
         play_effect (SND_CLICK, 128);
      }
      if (down) {
         unpress ();
         yptr++;
         if (yptr > noi - 1)
            yptr = 0;
         play_effect (SND_CLICK, 128);
      }
      if (left && xptr > 1) {
         unpress ();
         xptr--;
         play_effect (SND_CLICK, 128);
      }
      if (right && xptr < max_x) {
         unpress ();
         xptr++;
         play_effect (SND_CLICK, 128);
      }
      if (balt) {
         unpress ();
         blit (double_buffer, back, xofs, 192 + yofs, 0, 0, 320, 48);
         max = shopq[shop_no][yptr];
         if (xptr <= max)
            max = xptr;
         buy_item (max, yptr);
      }
      if (bctrl) {
         unpress ();
         stop = 1;
      }
   }
}



/*! \brief Actually purchase the item
 *
 * This is used after selecting an item, from the above
 * menu, to determine who to give it to.  Then it gives
 * it to them and deducts the cash.
 *
 * \param   how_many Quantity
 * \param   item_no Index of item
 */
static void buy_item (int how_many, int item_no)
{
   int z = 0, l, stop = 0, cost;

   l = shin[item_no];
   cost = items[l].price * how_many;
   if (cost > gp || how_many == 0) {
      play_effect (SND_BAD, 128);
      return;
   }
   while (!stop) {
      check_animation ();
      blit (back, double_buffer, 0, 0, xofs, 192 + yofs, 320, 48);
      menubox (double_buffer, 32 + xofs, 168 + yofs, 30, 1, DARKBLUE);
      print_font (double_buffer, 104 + xofs, 176 + yofs, "Confirm/Cancel",
                  FNORMAL);
      draw_sideshot (shin[item_no]);
      blit2screen (xofs, yofs);

      readcontrols ();
      if (balt) {
         unpress ();
         stop = 1;
      }
      if (bctrl) {
         unpress ();
         return;
      }
   }
   z = check_inventory (l, how_many);
   if (z > 0) {
      gp = gp - cost;
      shopq[shop_no][item_no] -= how_many;
      play_effect (SND_MONEY, 128);
      return;
   }
   play_effect (SND_BAD, 128);
   message ("No room!", -1, 0, xofs, yofs);
   return;
}



/*! \brief Show items that can be sold
 *
 * Display a list of items that are in inventory and ask which
 * one(s) to sell.
 */
static void sell_menu (void)
{
   int yptr = 0, stop = 0, z, p, k, sp, inv_page = 0;

   while (!stop) {
      check_animation ();
      drawmap ();
      menubox (double_buffer, 152 - (strlen (sname) * 4) + xofs, yofs,
               strlen (sname), 1, BLUE);
      print_font (double_buffer, 160 - (strlen (sname) * 4) + xofs,
                  8 + yofs, sname, FGOLD);
      menubox (double_buffer, xofs, 208 + yofs, 7, 2, BLUE);
      print_font (double_buffer, 20 + xofs, 220 + yofs, "Sell", FGOLD);
      menubox (double_buffer, 32 + xofs, 24 + yofs, 30, 16, BLUE);
      menubox (double_buffer, 32 + xofs, 168 + yofs, 30, 1, BLUE);
      draw_shopgold ();
      for (p = 0; p < 16; p++) {
         z = g_inv[inv_page * 16 + p][0];
         if (items[z].price == 0)
            k = FDARK;
         else
            k = FNORMAL;
         draw_icon (double_buffer, items[z].icon, 48 + xofs,
                    p * 8 + 32 + yofs);
         print_font (double_buffer, 56 + xofs, p * 8 + 32 + yofs,
                     items[z].name, k);
         // Check if quantity of this item > 1
         if (g_inv[inv_page * 16 + p][1] > 1) {
            // The '^' in this is an 'x' in allfonts.pcx
            sprintf (strbuf, "^%d", g_inv[inv_page * 16 + p][1]);
            print_font (double_buffer, 264 + xofs, p * 8 + 32 + yofs,
                        strbuf, k);
         }
      }
      sp = items[g_inv[inv_page * 16 + yptr][0]].price * 50 / 100;
      if (items[g_inv[inv_page * 16 + yptr][0]].price > 0) {
         if (g_inv[inv_page * 16 + yptr][1] > 1) {
            // Check if there is more than one item
            sprintf (strbuf, "%d gp for each one.", sp);
            print_font (double_buffer, 160 - (strlen (strbuf) * 4) + xofs,
                        176 + yofs, strbuf, FNORMAL);
         } else {
            // There is only one of this item
            sprintf (strbuf, "That's worth %d gp.", sp);
            print_font (double_buffer, 160 - (strlen (strbuf) * 4) + xofs,
                        176 + yofs, strbuf, FNORMAL);
         }
      } else {
         if (g_inv[inv_page * 16 + yptr][0] > 0)
            print_font (double_buffer, 76 + xofs, 192 + yofs,
                        "That can not be sold!", FNORMAL);
      }
      draw_sprite (double_buffer, menuptr, 32 + xofs, yptr * 8 + 32 + yofs);
      draw_sprite (double_buffer, pgb[inv_page], 278 + xofs, 158 + yofs);
      blit2screen (xofs, yofs);

      readcontrols ();

      if (down) {
         unpress ();
         yptr++;
         if (yptr > 15)
            yptr = 0;
         play_effect (SND_CLICK, 128);
      }
      if (up) {
         unpress ();
         yptr--;
         if (yptr < 0)
            yptr = 15;
         play_effect (SND_CLICK, 128);
      }
      if (left) {
         unpress ();
         inv_page--;
         if (inv_page < 0)
            inv_page = MAX_INV / 16 - 1;
         play_effect (SND_CLICK, 128);
      }
      if (right) {
         unpress ();
         inv_page++;
         if (inv_page > (MAX_INV / 16 - 1))
            inv_page = 0;
         play_effect (SND_CLICK, 128);
      }
      if (balt) {
         unpress ();
         if (g_inv[inv_page * 16 + yptr][0] > 0
             && items[g_inv[inv_page * 16 + yptr][0]].price > 0)
            sell_howmany (yptr, inv_page);
      }
      if (bctrl) {
         unpress ();
         stop = 1;
      }
   }
}



/*! \brief Ask player the quantity to sell
 *
 * Inquire as to what quantity of the current item, the
 * character wishes to sell.
 *
 * \param   item_no Index of item in inventory
 * \param   inv_page Page of the inventory
 */
static void sell_howmany (int item_no, int inv_page)
{
   int l, max_items, prc, my = 1, stop;

   stop = 0;
   l = g_inv[inv_page * 16 + item_no][0];
   prc = items[l].price;
   if (prc == 0) {
      play_effect (SND_BAD, 128);
      return;
   }
   // Maximum (total) number of items
   max_items = g_inv[inv_page * 16 + item_no][1];
   if (max_items == 1) {
      menubox (double_buffer, 32 + xofs, 168 + yofs, 30, 1, DARKBLUE);
      sprintf (strbuf, "Sell for %d gp?", prc * 50 / 100);
      print_font (double_buffer, 160 - (strlen (strbuf) * 4) + xofs,
                  176 + yofs, strbuf, FNORMAL);
      sell_item (inv_page * 16 + item_no, 1);
      stop = 1;
   }
   while (!stop) {
      check_animation ();
      drawmap ();
      menubox (double_buffer, 32 + xofs, 168 + yofs, 30, 1, DARKBLUE);
      print_font (double_buffer, 124 + xofs, 176 + yofs, "How many?", FNORMAL);
      menubox (double_buffer, 32 + xofs, item_no * 8 + 24 + yofs, 30, 1,
               DARKBLUE);
      draw_icon (double_buffer, items[l].icon, 48 + xofs,
                 item_no * 8 + 32 + yofs);
      print_font (double_buffer, 56 + xofs, item_no * 8 + 32 + yofs,
                  items[l].name, FNORMAL);
      sprintf (strbuf, "%d of %d", my, max_items);
      print_font (double_buffer, 280 - (strlen (strbuf) * 8) + xofs,
                  item_no * 8 + 32 + yofs, strbuf, FNORMAL);
      blit2screen (xofs, yofs);

      readcontrols ();
      if (up || right) {
         if (my < max_items) {
            unpress ();
            my++;
         } else {
            unpress ();
            my = 1;
         }
      }
      if (down || left) {
         if (my > 1) {
            unpress ();
            my--;
         } else {
            unpress ();
            my = max_items;
         }
      }
      if (balt) {
         unpress ();
         menubox (double_buffer, 32 + xofs, 168 + yofs, 30, 1, DARKBLUE);
         sprintf (strbuf, "Sell for %d gp?", (prc * 50 / 100) * my);
         print_font (double_buffer, 160 - (strlen (strbuf) * 4) + xofs,
                     176 + yofs, strbuf, FNORMAL);
         sell_item (inv_page * 16 + item_no, my);
         stop = 1;
      }
      if (bctrl) {
         unpress ();
         stop = 1;
      }
   }
}



/*! \brief Actually sell item
 *
 * Confirm the price of the sale with the player, and then
 * complete the transaction.
 *
 * \param   itno Index of item
 * \param   ni Quantity being sold
 */
static void sell_item (int itno, int ni)
{
   int l, stop = 0, sp, a;

   l = g_inv[itno][0];
   sp = (items[l].price * 50 / 100) * ni;
   menubox (double_buffer, 96 + xofs, 192 + yofs, 14, 1, DARKBLUE);
   print_font (double_buffer, 104 + xofs, 200 + yofs, "Confirm/Cancel",
               FNORMAL);
   blit2screen (xofs, yofs);
   while (!stop) {
      readcontrols ();
      if (balt) {
         unpress ();
         gp += sp;
         for (a = 0; a < SHOPITEMS; a++) {
            if (l > 0 && shops[shop_no][a][0] == l) {
               shopq[shop_no][a] += ni;
               if (shopq[shop_no][a] > shops[shop_no][a][1])
                  shopq[shop_no][a] = shops[shop_no][a][1];
            }
         }
         play_effect (SND_MONEY, 128);
         remove_item (itno, ni);
         stop = 1;
      }
      if (bctrl) {
         unpress ();
         stop = 1;
      }
   }
}



/*! \brief Handle Inn functions
 *
 * This is simply used for staying at the inn.  Remember
 * it costs more money to stay if your characters require
 * healing or resurrection.
 *
 * \param   iname Name of Inn
 * \param   gpc Gold per character (base price)
 * \param   pay If 0, staying is free.
 */
void inn (char *iname, int gpc, int pay)
{
   int a, b, my = 0, stop = 0, gpts;

   if (pay == 0) {
      /* TT add: (pay) is also used now to indicate whether we should wait
       *         (fade in/out) or just heal the heroes and be done
       */
      do_inn_effects (pay);
      return;
   }
   unpress ();
   drawmap ();
   menubox (double_buffer, 152 - (strlen (iname) * 4) + xofs, yofs,
            strlen (iname), 1, BLUE);
   print_font (double_buffer, 160 - (strlen (iname) * 4) + xofs, 8 + yofs,
               iname, FGOLD);
   gpts = gpc;
   for (a = 0; a < numchrs; a++) {
      if (party[pidx[a]].sts[S_POISON] != 0)
         gpts += gpc * 50 / 100;
      if (party[pidx[a]].sts[S_BLIND] != 0)
         gpts += gpc * 50 / 100;
      if (party[pidx[a]].sts[S_MUTE] != 0)
         gpts += gpc * 50 / 100;
      if (party[pidx[a]].sts[S_DEAD] != 0) {
         b = gpc * 50 / 100;
         gpts += (b * party[pidx[a]].lvl / 5);
      }
   }
   while (!stop) {
      check_animation ();
      drawmap ();

      sprintf (strbuf, "The cost is %d gp for the night.", gpts);
      menubox (double_buffer, 152 - (strlen (strbuf) * 4) + xofs, 48 + yofs,
               strlen (strbuf), 1, BLUE);
      print_font (double_buffer, 160 - (strlen (strbuf) * 4) + xofs, 56 + yofs,
                  strbuf, FNORMAL);
      menubox (double_buffer, 248 + xofs, 168 + yofs, 7, 2, BLUE);
      print_font (double_buffer, 256 + xofs, 176 + yofs, "Gold:", FGOLD);
      sprintf (strbuf, "%d", gp);
      print_font (double_buffer, 312 - (strlen (strbuf) * 8) + xofs,
                  184 + yofs, strbuf, FNORMAL);
      if (gp >= gpts) {
         menubox (double_buffer, 52 + xofs, 96 + yofs, 25, 2, BLUE);
         print_font (double_buffer, 60 + xofs, 108 + yofs,
                     "Do you wish to stay?", FNORMAL);
      } else {
         menubox (double_buffer, 32 + xofs, 96 + yofs, 30, 2, BLUE);
         print_font (double_buffer, 40 + xofs, 108 + yofs,
                     "You can't afford to stay here.", FNORMAL);
         blit2screen (xofs, yofs);
         wait_enter ();
         return;
      }

      menubox (double_buffer, 220 + xofs, 96 + yofs, 4, 2, DARKBLUE);
      print_font (double_buffer, 236 + xofs, 104 + yofs, "yes", FNORMAL);
      print_font (double_buffer, 236 + xofs, 112 + yofs, "no", FNORMAL);
      draw_sprite (double_buffer, menuptr, 220 + xofs, my * 8 + 104 + yofs);
      blit2screen (xofs, yofs);
      readcontrols ();
      if (down) {
         unpress ();
         if (my == 0)
            my = 1;
         else
            my = 0;
         play_effect (SND_CLICK, 128);
      }
      if (up) {
         unpress ();
         if (my == 0)
            my = 1;
         else
            my = 0;
         play_effect (SND_CLICK, 128);
      }
      if (balt) {
         unpress ();
         if (my == 0) {
            gp -= gpts;
            do_inn_effects (pay);
            stop = 1;
         } else {
            stop = 2;
         }
      }
   }
   timer_count = 0;
}



/*! \brief Restore characters according to Inn effects.
 *
 * This is separate so that these effects can be done from anywhere.
 *
 * \param   do_delay Whether or not to wait during the darkness...
 */
void do_inn_effects (int do_delay)
{
   int a, b, c;

   for (a = 0; a < numchrs; a++) {
      c = pidx[a];
      party[c].hp = party[c].mhp;
      party[c].mp = party[c].mmp;
      for (b = 0; b < 8; b++)
         party[c].sts[b] = 0;
   }
   pause_music ();
   play_effect (36, 128);
   if (do_delay) {
      do_transition (TRANS_FADE_OUT, 2);
      drawmap ();
      blit2screen (xofs, yofs);
   }
   progress[P_REPULSE] = 0;
   if (do_delay) {
      wait (1500);
      do_transition (TRANS_FADE_IN, 2);
   }
   resume_music ();
}
