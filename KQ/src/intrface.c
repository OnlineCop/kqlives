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
 * \brief Interface functions
 * \author JB, PH
 * \date Created ????????
 * \date Updated 20021125 -- Added extra functions
 * \date Updated 20030308 -- Added object interface
 *
 * This file implements the interface between
 * the C code and the Lua scripts.
 *
 */

#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "intrface.h"
#include "setup.h"
#include "effects.h"
#include "draw.h"
#include "kq.h"
#include "combat.h"
#include "heroc.h"
#include "menu.h"
#include "enemyc.h"
#include "magic.h"
#include "sgame.h"
#include "itemmenu.h"
#include "masmenu.h"
#include "res.h"
#include "progress.h"
#include "itemdefs.h"
#include "shopmenu.h"
#include "fade.h"
#include "timing.h"
#include "music.h"
#include "selector.h"


/*  internal functions  */
static int KQ_get_pidx (lua_State *);
static int KQ_get_progress (lua_State *);
static int KQ_set_progress (lua_State *);
static int KQ_get_party_name (lua_State *);
static int KQ_get_party_xp (lua_State *);
static int KQ_set_party_xp (lua_State *);
static int KQ_get_party_next (lua_State *);
static int KQ_set_party_next (lua_State *);
static int KQ_get_party_lvl (lua_State *);
static int KQ_set_party_lvl (lua_State *);
static int KQ_get_party_mrp (lua_State *);
static int KQ_set_party_mrp (lua_State *);
static int KQ_get_party_hp (lua_State *);
static int KQ_set_party_hp (lua_State *);
static int KQ_get_party_mhp (lua_State *);
static int KQ_set_party_mhp (lua_State *);
static int KQ_get_party_mp (lua_State *);
static int KQ_set_party_mp (lua_State *);
static int KQ_get_party_mmp (lua_State *);
static int KQ_set_party_mmp (lua_State *);
static int KQ_get_party_eqp (lua_State *);
static int KQ_set_party_eqp (lua_State *);
static int KQ_get_party_stats (lua_State *);
static int KQ_set_party_stats (lua_State *);
static int KQ_get_party_res (lua_State *);
static int KQ_set_party_res (lua_State *);
static int KQ_get_ent_chrx (lua_State *);
static int KQ_set_ent_chrx (lua_State *);
static int KQ_get_ent_tilex (lua_State *);
static int KQ_set_ent_tilex (lua_State *);
static int KQ_get_ent_tiley (lua_State *);
static int KQ_set_ent_tiley (lua_State *);
static int KQ_get_ent_id (lua_State *);
static int KQ_set_ent_id (lua_State *);
static int KQ_get_ent_active (lua_State *);
static int KQ_set_ent_active (lua_State *);
static int KQ_get_ent_facing (lua_State *);
static int KQ_set_ent_facing (lua_State *);
static int KQ_get_ent_movemode (lua_State *);
static int KQ_set_ent_movemode (lua_State *);
static int KQ_get_ent_obsmode (lua_State *);
static int KQ_set_ent_obsmode (lua_State *);
static int KQ_get_ent_speed (lua_State *);
static int KQ_set_ent_speed (lua_State *);
static int KQ_get_ent_atype (lua_State *);
static int KQ_set_ent_atype (lua_State *);
static int KQ_get_ent_snapback (lua_State *);
static int KQ_set_ent_snapback (lua_State *);
static int KQ_get_ent_facehero (lua_State *);
static int KQ_set_ent_facehero (lua_State *);
static int KQ_get_ent_transl (lua_State *);
static int KQ_set_ent_transl (lua_State *);
static int KQ_set_ent_script (lua_State *);
static int KQ_get_treasure (lua_State *);
static int KQ_get_gp (lua_State *);
static int KQ_set_gp (lua_State *);
static int KQ_get_vx (lua_State *);
static int KQ_set_vx (lua_State *);
static int KQ_get_vy (lua_State *);
static int KQ_set_vy (lua_State *);
static int KQ_get_alldead (lua_State *);
static int KQ_set_alldead (lua_State *);
static int KQ_get_autoparty (lua_State *);
static int KQ_set_autoparty (lua_State *);
static int KQ_get_numchrs (lua_State *);
static int KQ_set_all_equip (lua_State *);
static int KQ_set_btile (lua_State *);
static int KQ_set_mtile (lua_State *);
static int KQ_set_ftile (lua_State *);
static int KQ_set_obs (lua_State *);
static int KQ_set_zone (lua_State *);
static int KQ_set_shadow (lua_State *);
static int KQ_set_tile_all (lua_State *);
static int KQ_set_desc (lua_State *);
static int KQ_set_foreground (lua_State *);
static int KQ_set_holdfade (lua_State *);
static int KQ_get_noe (lua_State *);
static int KQ_set_noe (lua_State *);
static int KQ_set_run (lua_State *);
static int KQ_set_save (lua_State *);
static int KQ_set_sstone (lua_State *);
static int KQ_set_warp (lua_State *);
static int KQ_set_vfollow (lua_State *);
static int KQ_create_df (lua_State *);
static int KQ_destroy_df (lua_State *);
static int KQ_create_bmp (lua_State *);
static int KQ_destroy_bmp (lua_State *);
static int KQ_df2bmp (lua_State *);
static int KQ_blit (lua_State *);
static int KQ_maskblit (lua_State *);
static int KQ_drawsprite (lua_State *);
static int KQ_screen_dump (lua_State *);
static int KQ_drawmap (lua_State *);
static int KQ_prompt (lua_State *);
static int KQ_bubble_ex (lua_State *);
static int KQ_thought_ex (lua_State *);
static int KQ_copy_ent (lua_State *);
static int KQ_place_ent (lua_State *);
static int KQ_face_each_other (lua_State *);
static int KQ_wait_for_entity (lua_State *);
static int KQ_add_chr (lua_State *);
static int KQ_remove_chr (lua_State *);
static int KQ_krnd (lua_State *);
static int KQ_do_fadeout (lua_State *);
static int KQ_do_fadein (lua_State *);
static int KQ_shop (lua_State *);
static int KQ_inn (lua_State *);
static int KQ_door_in (lua_State *);
static int KQ_door_out (lua_State *);
static int KQ_calc_viewport (lua_State *);
static int KQ_change_map (lua_State *);
static int KQ_give_item (lua_State *);
static int KQ_warp (lua_State *);
static int KQ_give_xp (lua_State *);
static int KQ_chest (lua_State *);
static int KQ_combat (lua_State *);
static int KQ_wait (lua_State *);
static int KQ_rest (lua_State *);
static int KQ_msg (lua_State *);
static int KQ_move_camera (lua_State *);
static int KQ_in_forest (lua_State *);
static int KQ_orient_heroes (lua_State *);
static int KQ_pause_song (lua_State *);
static int KQ_unpause_map_song (lua_State *);
static int KQ_play_map_song (lua_State *);
static int KQ_play_song (lua_State *);
static int KQ_stop_song (lua_State *);
static int KQ_view_range (lua_State *);
static int KQ_sfx (lua_State *);
static int KQ_clear_buffer (lua_State *);
static int KQ_mbox (lua_State *);
static int KQ_dark_mbox (lua_State *);
static int KQ_light_mbox (lua_State *);
static int KQ_drawframe (lua_State *);
static int KQ_draw_pstat (lua_State *);
static int KQ_ptext (lua_State *);
static int KQ_pnum (lua_State *);
static int KQ_read_controls (lua_State *);
static int KQ_check_key (lua_State *);
static int KQ_log (lua_State *);
static int KQ_wait_enter (lua_State *);
static int KQ_istable (lua_State *);
static void check_map_change (void);
static int KQ_set_map_mode (lua_State *);

/* New functions */
#if 0
/* Not used yet */
static int KQ_get_tile_all (lua_State *);
#endif

static int KQ_copy_tile_all (lua_State *);
static int KQ_use_up (lua_State *);
static int KQ_battle (lua_State *);
static int KQ_select_team (lua_State *);
static int KQ_set_ent_target (lua_State *);

static const struct luaL_reg lrs[] = {
   {"get_pidx", KQ_get_pidx},
   {"get_progress", KQ_get_progress},
   {"set_progress", KQ_set_progress},
   {"get_party_name", KQ_get_party_name},
   {"get_party_xp", KQ_get_party_xp},
   {"set_party_xp", KQ_set_party_xp},
   {"get_party_next", KQ_get_party_next},
   {"set_party_next", KQ_set_party_next},
   {"get_party_lvl", KQ_get_party_lvl},
   {"set_party_lvl", KQ_set_party_lvl},
   {"get_party_mrp", KQ_get_party_mrp},
   {"set_party_mrp", KQ_set_party_mrp},
   {"get_party_hp", KQ_get_party_hp},
   {"set_party_hp", KQ_set_party_hp},
   {"get_party_mhp", KQ_get_party_mhp},
   {"set_party_mhp", KQ_set_party_mhp},
   {"get_party_mp", KQ_get_party_mp},
   {"set_party_mp", KQ_set_party_mp},
   {"get_party_mmp", KQ_get_party_mmp},
   {"set_party_mmp", KQ_set_party_mmp},
   {"get_party_eqp", KQ_get_party_eqp},
   {"set_party_eqp", KQ_set_party_eqp},
   {"get_party_stats", KQ_get_party_stats},
   {"set_party_stats", KQ_set_party_stats},
   {"get_party_res", KQ_get_party_res},
   {"set_party_res", KQ_set_party_res},
   {"get_ent_chrx", KQ_get_ent_chrx},
   {"set_ent_chrx", KQ_set_ent_chrx},
   {"get_ent_tilex", KQ_get_ent_tilex},
   {"set_ent_tilex", KQ_set_ent_tilex},
   {"get_ent_tiley", KQ_get_ent_tiley},
   {"set_ent_tiley", KQ_set_ent_tiley},
   {"get_ent_id", KQ_get_ent_id},
   {"set_ent_id", KQ_set_ent_id},
   {"get_ent_active", KQ_get_ent_active},
   {"set_ent_active", KQ_set_ent_active},
   {"get_ent_facing", KQ_get_ent_facing},
   {"set_ent_facing", KQ_set_ent_facing},
   {"get_ent_movemode", KQ_get_ent_movemode},
   {"set_ent_movemode", KQ_set_ent_movemode},
   {"get_ent_obsmode", KQ_get_ent_obsmode},
   {"set_ent_obsmode", KQ_set_ent_obsmode},
   {"get_ent_speed", KQ_get_ent_speed},
   {"set_ent_speed", KQ_set_ent_speed},
   {"get_ent_atype", KQ_get_ent_atype},
   {"set_ent_atype", KQ_set_ent_atype},
   {"get_ent_snapback", KQ_get_ent_snapback},
   {"set_ent_snapback", KQ_set_ent_snapback},
   {"get_ent_facehero", KQ_get_ent_facehero},
   {"set_ent_facehero", KQ_set_ent_facehero},
   {"get_ent_transl", KQ_get_ent_transl},
   {"set_ent_transl", KQ_set_ent_transl},
   {"set_ent_script", KQ_set_ent_script},
   {"get_treasure", KQ_get_treasure},
   {"get_gp", KQ_get_gp},
   {"set_gp", KQ_set_gp},
   {"get_vx", KQ_get_vx},
   {"set_vx", KQ_set_vx},
   {"get_vy", KQ_get_vy},
   {"set_vy", KQ_set_vy},
   {"get_alldead", KQ_get_alldead},
   {"set_alldead", KQ_set_alldead},
   {"get_autoparty", KQ_get_autoparty},
   {"set_autoparty", KQ_set_autoparty},
   {"get_numchrs", KQ_get_numchrs},
   {"set_all_equip", KQ_set_all_equip},
   {"set_btile", KQ_set_btile},
   {"set_mtile", KQ_set_mtile},
   {"set_ftile", KQ_set_ftile},
   {"set_obs", KQ_set_obs},
   {"set_zone", KQ_set_zone},
   {"set_shadow", KQ_set_shadow},
   {"set_tile_all", KQ_set_tile_all},
   {"set_desc", KQ_set_desc},
   {"set_foreground", KQ_set_foreground},
   {"set_holdfade", KQ_set_holdfade},
   {"get_noe", KQ_get_noe},
   {"set_noe", KQ_set_noe},
   {"set_run", KQ_set_run},
   {"set_save", KQ_set_save},
   {"set_sstone", KQ_set_sstone},
   {"set_warp", KQ_set_warp},
   {"set_vfollow", KQ_set_vfollow},
   {"create_df", KQ_create_df},
   {"destroy_df", KQ_destroy_df},
   {"create_bmp", KQ_create_bmp},
   {"destroy_bmp", KQ_destroy_bmp},
   {"df2bmp", KQ_df2bmp},
   {"blit", KQ_blit},
   {"maskblit", KQ_maskblit},
   {"drawsprite", KQ_drawsprite},
   {"screen_dump", KQ_screen_dump},
   {"drawmap", KQ_drawmap},
   {"prompt", KQ_prompt},
   {"bubble_ex", KQ_bubble_ex},
   {"thought_ex", KQ_thought_ex},
   {"copy_ent", KQ_copy_ent},
   {"place_ent", KQ_place_ent},
   {"face_each_other", KQ_face_each_other},
   {"wait_for_entity", KQ_wait_for_entity},
   {"add_chr", KQ_add_chr},
   {"remove_chr", KQ_remove_chr},
   {"krnd", KQ_krnd},
   {"do_fadeout", KQ_do_fadeout},
   {"do_fadein", KQ_do_fadein},
   {"shop", KQ_shop},
   {"inn", KQ_inn},
   {"door_in", KQ_door_in},
   {"door_out", KQ_door_out},
   {"calc_viewport", KQ_calc_viewport},
   {"change_map", KQ_change_map},
   {"give_item", KQ_give_item},
   {"warp", KQ_warp},
   {"give_xp", KQ_give_xp},
   {"chest", KQ_chest},
   {"combat", KQ_combat},
   {"wait", KQ_wait},
   {"rest", KQ_rest},
   {"msg", KQ_msg},
   {"move_camera", KQ_move_camera},
   {"in_forest", KQ_in_forest},
   {"orient_heroes", KQ_orient_heroes},
   {"pause_song", KQ_pause_song},
   {"unpause_map_song", KQ_unpause_map_song},
   {"play_map_song", KQ_play_map_song},
   {"play_song", KQ_play_song},
   {"stop_song", KQ_stop_song},
   {"view_range", KQ_view_range},
   {"sfx", KQ_sfx},
   {"clear_buffer", KQ_clear_buffer},
   {"mbox", KQ_mbox},
   {"dark_mbox", KQ_dark_mbox},
   {"light_mbox", KQ_light_mbox},
   {"drawframe", KQ_drawframe},
   {"draw_pstat", KQ_draw_pstat},
   {"ptext", KQ_ptext},
   {"pnum", KQ_pnum},
   {"read_controls", KQ_read_controls},
   {"check_key", KQ_check_key},
   {"log", KQ_log},
   {"wait_enter", KQ_wait_enter},
   {"istable", KQ_istable},
   /*   {"get_tile_all", KQ_get_tile_all}, */
   {"copy_tile_all", KQ_copy_tile_all},
   {"use_up", KQ_use_up},
   {"battle", KQ_battle},
   {"select_team", KQ_select_team},
   {"set_map_mode", KQ_set_map_mode},
   {"set_ent_target", KQ_set_ent_target},
   {NULL, NULL}
};

/*! \brief Holds a reference to the zone handler function */
int ref_zone_handler;

/*! \brief Maps a text field name to an identifier */
static struct s_field
{
   const char *name;
   int id;
}


// *INDENT-OFF*
fields[] = {
   // Name of entity
   {"name", 0},
   // Entity experience
   {"xp", 1},
   // Experience left for next level-up
   {"next", 2},
   // Current level of entity
   {"lvl", 3},
   // Magic actually required for a spell (can be reduced with I_MANALOCKET)
   {"mrp", 4},
   // Entity's current hit points
   {"hp", 5},
   // Maximum hit points
   {"mhp", 6},
   // Current magic points
   {"mp", 7},
   // Maximum magic points
   {"mmp", 8},
   // Index # of entity, which determines look and skills
   {"id", 9}
};
// *INDENT-ON*


static int fieldcmp (const void *pa, const void *pb)
{
   const struct s_field *a = (const struct s_field *) pa;
   const struct s_field *b = (const struct s_field *) pb;
   return strcmp (a->name, b->name);
}



/*! \brief Sort field array
 *
 * This uses qsort to sort the fields, ready for bsearch to search them
 * \author PH
 * \date Created 20030407
 */
static void fieldsort (void)
{
   qsort (fields, sizeof (fields) / sizeof (*fields), sizeof (struct s_field),
          fieldcmp);
}



/*! \brief Get the field number from a name.
 *
 * Note that the field list MUST be sorted first
 * \author PH 20030309
 * \param   n The field name
 * \returns the index, or -1 if not found
 */
static int get_field (const char *n)
{
   struct s_field *ans;
   struct s_field st;
   st.name = n;
   ans =
      bsearch (&st, fields, sizeof (fields) / sizeof (*fields),
               sizeof (struct s_field), fieldcmp);
   return ans ? ans->id : -1;
}



/*! \brief Object interface for party
 *
 * This implements the settable tag method
 */
int KQ_party_setter (lua_State * L)
{
   int prop;
   s_player *pl;
   prop = get_field (lua_tostring (L, 2));
   lua_pushstring (L, "_obj");
   lua_rawget (L, 1);
   pl = (s_player *) lua_touserdata (L, -1);
   lua_pop (L, 1);
   switch (prop) {
   case 0:
      strncpy (pl->name, lua_tostring (L, 3), sizeof (pl->name));
      break;
   case 1:
      pl->xp = (int) lua_tonumber (L, 3);
      break;
   case 2:
      pl->next = (int) lua_tonumber (L, 3);
      break;
   case 3:
      pl->lvl = (int) lua_tonumber (L, 3);
      break;
   case 4:
      pl->mrp = (int) lua_tonumber (L, 3);
      break;
   case 5:
      pl->hp = (int) lua_tonumber (L, 3);
      break;
   case 6:
      pl->mhp = (int) lua_tonumber (L, 3);
      break;
   case 7:
      pl->mp = (int) lua_tonumber (L, 3);
      break;
   case 8:
      pl->mmp = (int) lua_tonumber (L, 3);
      break;
   case 9:
      /* id is readonly */
      break;
   default:
      lua_rawset (L, 1);
      break;
   }
   return 0;
}



/*! \brief Object interface for party
 *
 * This implements the gettable tag method
 */
int KQ_party_getter (lua_State * L)
{
   int prop;
   s_player *pl;
   prop = get_field (lua_tostring (L, 2));
   lua_pushstring (L, "_obj");
   lua_rawget (L, 1);
   pl = (s_player *) lua_touserdata (L, -1);
   lua_pop (L, 1);
   switch (prop) {
   case 0:
      lua_pushstring (L, pl->name);
      break;
   case 1:
      lua_pushnumber (L, pl->xp);
      break;
   case 2:
      lua_pushnumber (L, pl->next);
      break;
   case 3:
      lua_pushnumber (L, pl->lvl);
      break;
   case 4:
      lua_pushnumber (L, pl->mrp);
      break;
   case 5:
      lua_pushnumber (L, pl->hp);
      break;
   case 6:
      lua_pushnumber (L, pl->mhp);
      break;
   case 7:
      lua_pushnumber (L, pl->mp);
      break;
   case 8:
      lua_pushnumber (L, pl->mmp);
      break;
   case 9:
      lua_pushnumber (L, pl - party);
      break;
   default:
      lua_rawget (L, 1);
      break;
   }
   return 1;
}



/*! \brief Initialise the object interface for heroes and entities
 *
 * This registers a new tag type for the heroes and adds the gettable method
 * to it. It then creates global variables for all heroes with their names as
 * defined (Sensar etc.). Then it sets the 'player[]' global (all heroes) and
 * the 'party[]' global (all heroes currently in play). Finally it sets the
 * 'entity[]' array.
 *
 * \param L the Lua state object
 */
static void init_obj (lua_State * L)
{
   int i = 0;
   int tag = lua_newtag (L);
   for (i = 0; i < MAXCHRS; ++i) {
      lua_newtable (L);
      lua_settag (L, tag);
      lua_pushstring (L, "_obj");
      lua_pushuserdata (L, &party[i]);
      lua_rawset (L, -3);
      lua_setglobal (L, party[i].name);
   }
   lua_pushcfunction (L, KQ_party_getter);
   lua_settagmethod (L, tag, "gettable");
   lua_pushcfunction (L, KQ_party_setter);
   lua_settagmethod (L, tag, "settable");
   /* party[] array */
   lua_newtable (L);
   for (i = 0; i < numchrs; ++i) {
      lua_getglobal (L, party[pidx[i]].name);
      /* also fill in the entity reference */
      lua_pushstring (L, "_ent");
      lua_pushuserdata (L, &g_ent[i]);
      lua_rawset (L, -3);
      /* and add to the array */
      lua_rawseti (L, -2, i);
   }
   lua_setglobal (L, "party");
   /* player[] array */
   lua_newtable (L);
   for (i = 0; i < MAXCHRS; ++i) {
      lua_getglobal (L, party[i].name);
      lua_rawseti (L, -2, i);
   }
   lua_setglobal (L, "player");
   /* entity[] array
    * at the moment, NPCs aren't fully implemented - the
    * only thing you can do is call bubble()/thought()
    * so they can just be an index
    */
   lua_newtable (L);
   for (i = 0; i < numchrs; ++i) {
      lua_getglobal (L, party[pidx[i]].name);
      lua_rawseti (L, -2, i);
   }
   for (i = PSIZE; i < noe + PSIZE; ++i) {
      lua_pushnumber (L, i);
      lua_rawseti (L, -2, i);
   }
   lua_setglobal (L, "entity");
}


int g_trk, g_keys[8];
BITMAP *g_bmp[5];
DATAFILE *g_df;
lua_State *theL;
char tmap_name[16];
int tmx, tmy, tmvx, tmvy, changing_map = 0;



/*! \brief Process HERO1 and HERO2 pseudo-entity numbers
 *
 * Calculate what's the real entity number,
 * given an enemy number or HERO1 or HERO2
 *
 * \param   ee Input number
 * \returns real entity number
 */
static int real_entity_num (int ee)
{
   if (ee == 255)
      return 255;
   if (ee == HERO1 || ee == HERO2)
      return ee - 200;
   return ee + PSIZE;
}



/*! \brief Get player ID
 *
 * This just gets the player's ID
 *
 * \param   L::1 Which character inquired about
 * \returns character's ID
 */
static int KQ_get_pidx (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   lua_pushnumber (L, pidx[a]);
   return 1;
}



/*! \brief Get player progress
 *
 * This just gets the player's progress through the game
 *
 * \param   L::1 Which Progress to evaluate
 * \returns the value of the Progress
 */
static int KQ_get_progress (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 1999)
      lua_pushnumber (L, progress[a]);
   return 1;
}



/*! \brief Set player progress
 *
 * This just sets the player's progress through the game
 *
 * \param   L::1 The index of the Progress to evaluate
 * \param   L::2 The value of L::1
 * \returns 0 when done
 */
static int KQ_set_progress (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 1999)
      progress[a] = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's name
 *
 * This gets the name of one of the people in the party
 *
 * \param   L::1 Which party member to evaluate
 * \returns person's name
 */
static int KQ_get_party_name (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushstring (L, (const char *) party[a].name);
   return 1;
}



/*! \brief Get person's experience
 *
 * This gets the selected player's experience
 *
 * \param   L::1 Which person's EXP to get
 * \returns person's experience
 */
static int KQ_get_party_xp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, party[a].xp);
   return 1;
}



/*! \brief Set person's experience
 *
 * This sets the selected player's experience
 *
 * \param   L::1 Which person's EXP to set
 * \param   L::2 Amount of EXP to set
 * \returns 0 when done
 */
static int KQ_set_party_xp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      party[a].xp = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's needed experience
 *
 * This gets the experience still needed to get a level-up for a person
 *
 * \param   L::1 Which person's EXP to evaluate
 * \returns 0 when done
 */
static int KQ_get_party_next (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, party[a].next);
   return 1;
}



/*! \brief Set person's needed experience
 *
 * This sets the experience still needed to get a level-up for a person
 *
 * \param   L::1 Which person's EXP to evaluate
 * \param   L::2 Amount of EXP to set
 * \returns 0 when done
 */
static int KQ_set_party_next (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      party[a].next = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's level
 *
 * This gets the person's current level
 *
 * \param   L::1 Which person's level to check
 * \returns person's level
 */
static int KQ_get_party_lvl (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, party[a].lvl);
   return 1;
}



/*! \brief Set person's level
 *
 * This sets the person's current level
 *
 * \param   L::1 Which person's level to check
 * \param   L::2 Which level to set L::1 to
 * \returns 0 when done
 */
static int KQ_set_party_lvl (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      party[a].lvl = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's mrp
 *
 * This gets the person's current mrp
 *
 * \param   L::1 Which person's mrp to check
 * \returns person's mrp
 */
static int KQ_get_party_mrp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, party[a].mrp);
   return 1;
}



/*! \brief Set person's mrp
 *
 * This sets the person's current mrp
 *
 * \param   L::1 Which person's mrp to set
 * \param   L::2 Amount of mrp to set to L::1
 * \returns 0 when done
 */
static int KQ_set_party_mrp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      party[a].mrp = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's HP
 *
 * This gets the person's current hit points
 *
 * \param   L::1 Which person's HP to check
 * \returns person's HP
 */
static int KQ_get_party_hp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, party[a].hp);
   return 1;
}



/*! \brief Set person's HP
 *
 * This sets the person's current hit points
 *
 * \param   L::1 Which person's level to set
 * \param   L::2 Amount of HP to set to L::1
 * \returns 0 when done
 */
static int KQ_set_party_hp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      party[a].hp = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's MHP
 *
 * This gets the person's maximum hit points
 *
 * \param   L::1 Which person's MHP to check
 * \returns person's MHP
 */
static int KQ_get_party_mhp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, party[a].mhp);
   return 1;
}



/*! \brief Set person's level
 *
 * This sets the person's maximum hit points
 *
 * \param   L::1 Which person's MHP to set
 * \param   L::2 Amount of MHP to set to L::1
 * \returns 0 when done
 */
static int KQ_set_party_mhp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      party[a].mhp = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's MP
 *
 * This gets the person's current magic points
 *
 * \param   L::1 Which person's mp to check
 * \returns person's MP
 */
static int KQ_get_party_mp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, party[a].mp);
   return 1;
}



/*! \brief Set person's MP
 *
 * This sets the person's current magic points
 *
 * \param   L::1 Which person's MP to set
 * \param   L::2 Amount of MP to set to L::1
 * \returns 0 when done
 */
static int KQ_set_party_mp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      party[a].mp = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's MMP
 *
 * This gets the person's maximum magic points
 *
 * \param   L::1 Which person's MMP to check
 * \returns person's MMP
 */
static int KQ_get_party_mmp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, party[a].mmp);
   return 1;
}



/*! \brief Set person's MMP
 *
 * This sets the person's maximum magic points
 *
 * \param   L::1 Which person's MMP to set
 * \param   L::2 Amont of MMP to set to L::1
 * \returns 0 when done
 */
static int KQ_set_party_mmp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      party[a].mmp = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Get person's equipment (unused)
 *
 * This gets the person's current equipment
 *
 * \param   L::1 Which person to check
 * \param   L::2 Which equipment slot
 * \returns person's equipment
 */
static int KQ_get_party_eqp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b = (int) lua_tonumber (L, 2);

   if (a >= 0 && a <= 7 && b >= 0 && b <= 5)
      lua_pushnumber (L, party[a].eqp[b]);
   return 1;
}



/*! \brief Set person's equipment (unused)
 *
 * This sets the person's current equipment
 *
 * \param   L::1 Which person to check
 * \param   L::2 Which equipment slot
 * \param   L::3 Equipment to set in slot L::2
 * \returns 0 when done
 */
static int KQ_set_party_eqp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b = (int) lua_tonumber (L, 2);

   if (a >= 0 && a <= 7 && b >= 0 && b <= 5)
      party[a].eqp[b] = (int) lua_tonumber (L, 3);
   return 0;
}



/*! \brief Get person's stats
 *
 * This gets the person's stats
 *
 * \param   L::1 Which person to check
 * \param   L::2 Which stat to check
 * \returns person's stats
 */
static int KQ_get_party_stats (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b = (int) lua_tonumber (L, 2);

   if (a >= 0 && a <= 7 && b >= 0 && b <= 12)
      lua_pushnumber (L, party[a].stats[b]);
   return 1;
}



/*! \brief Set person's stats (unused)
 *
 * This sets the person's stats
 *
 * \param   L::1 Which person to check
 * \param   L::2 Which stats id to evaluate
 * \param   L::3 Value to stick in L::2
 * \returns 0 when done
 */
static int KQ_set_party_stats (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b = (int) lua_tonumber (L, 2);

   if (a >= 0 && a <= 7 && b >= 0 && b <= 12)
      party[a].stats[b] = (int) lua_tonumber (L, 3);
   return 0;
}



/*! \brief Get person's res
 *
 * This gets the person's resistance
 *
 * \param   L::1 Which person to check
 * \param   L::2 Which res to check
 * \returns person's res
 */
static int KQ_get_party_res (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b = (int) lua_tonumber (L, 2);

   if (a >= 0 && a <= 7 && b >= 0 && b <= 15)
      lua_pushnumber (L, party[a].res[b]);
   return 1;
}



/*! \brief Set person's res
 *
 * This sets the person's resistance
 *
 * \param   L::1 Which person's res to set
 * \param   L::2 Which res to set
 * \param   L::3 Value to set to res
 * \returns 0 when done
 */
static int KQ_set_party_res (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b = (int) lua_tonumber (L, 2);

   if (a >= 0 && a <= 7 && b >= 0 && b <= 12)
      party[a].res[b] = (int) lua_tonumber (L, 3);
   return 0;
}



static int KQ_get_ent_chrx (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].chrx);
   return 1;
}



static int KQ_set_ent_chrx (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   g_ent[a].chrx = (int) lua_tonumber (L, 2);
   return 0;
}



/*! \brief Make entity go to a certain point
 *
 * Enter target move mode. (MM_TARGET) Entity
 * will try and go to the specified point.
 * You still need to call wait_entity after this.
 *
 * \param L::1 entity to set
 * \param L::2 x-coord (tile) to go to
 * \param L::3 y-coord
 * \returns Nothing
 */
static int KQ_set_ent_target (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   g_ent[a].target_x = (int) lua_tonumber (L, 2);
   g_ent[a].target_y = (int) lua_tonumber (L, 3);
   g_ent[a].movemode = MM_TARGET;
   return 0;
}



static int KQ_get_ent_tilex (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].tilex);
   return 1;
}



static int KQ_set_ent_tilex (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   g_ent[a].tilex = (int) lua_tonumber (L, 2);
   g_ent[a].x = g_ent[a].tilex * 16;
   return 0;
}



static int KQ_get_ent_tiley (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].tiley);
   return 1;
}



static int KQ_set_ent_tiley (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   g_ent[a].tiley = (int) lua_tonumber (L, 2);
   g_ent[a].y = g_ent[a].tiley * 16;
   return 0;
}



static int KQ_get_ent_id (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].eid);
   return 1;
}



static int KQ_set_ent_id (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   g_ent[a].eid = (int) lua_tonumber (L, 2);
   return 0;
}



static int KQ_get_ent_active (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].active);
   return 1;
}



static int KQ_set_ent_active (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = (int) lua_tonumber (L, 2);

   if (b == 0 || b == 1)
      g_ent[a].active = b;
   return 0;
}



static int KQ_get_ent_facing (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].facing);
   return 1;
}



static int KQ_set_ent_facing (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = (int) lua_tonumber (L, 2);

   if (b >= FACE_DOWN && b <= FACE_RIGHT)
      g_ent[a].facing = b;
   return 0;
}



static int KQ_get_ent_movemode (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].movemode);
   return 1;
}



static int KQ_set_ent_movemode (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = (int) lua_tonumber (L, 2);

   if (b >= 0 && b <= 3)
      g_ent[a].movemode = b;
   return 0;
}



static int KQ_get_ent_obsmode (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].obsmode);
   return 1;
}



static int KQ_set_ent_obsmode (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = (int) lua_tonumber (L, 2);

   if (b == 0 || b == 1)
      g_ent[a].obsmode = b;
   return 0;
}



static int KQ_get_ent_speed (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].speed);
   return 1;
}



static int KQ_set_ent_speed (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = (int) lua_tonumber (L, 2);

   if (b >= 1 && b <= 7)
      g_ent[a].speed = b;
   return 0;
}



static int KQ_get_ent_atype (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].atype);
   return 1;
}



static int KQ_set_ent_atype (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   g_ent[a].snapback = (int) lua_tonumber (L, 2);
   return 0;
}



static int KQ_get_ent_snapback (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].snapback);
   return 1;
}



static int KQ_set_ent_snapback (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = (int) lua_tonumber (L, 2);

   if (b == 0 || b == 1)
      g_ent[a].snapback = b;
   return 0;
}



static int KQ_get_ent_facehero (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].facehero);
   return 1;
}



static int KQ_set_ent_facehero (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = (int) lua_tonumber (L, 2);

   if (b == 0 || b == 1)
      g_ent[a].facehero = b;
   return 0;
}



static int KQ_get_ent_transl (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   lua_pushnumber (L, g_ent[a].transl);
   return 1;
}



static int KQ_set_ent_transl (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = (int) lua_tonumber (L, 2);

   if (b == 0 || b == 1)
      g_ent[a].transl = b;
   return 0;
}



static int KQ_set_ent_script (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   set_script (a, (char *) lua_tostring (L, 2));
   return 0;
}



static int KQ_get_treasure (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 999)
      lua_pushnumber (L, treasure[a]);
   return 1;
}



static int KQ_get_gp (lua_State * L)
{
   lua_pushnumber (L, gp);
   return 1;
}



static int KQ_set_gp (lua_State * L)
{
   gp = (int) lua_tonumber (L, 1);
   return 0;
}



static int KQ_get_vx (lua_State * L)
{
   lua_pushnumber (L, vx);
   return 1;
}



static int KQ_set_vx (lua_State * L)
{
   vx = (int) lua_tonumber (L, 1);
   return 0;
}



static int KQ_get_vy (lua_State * L)
{
   lua_pushnumber (L, vy);
   return 1;
}



static int KQ_set_vy (lua_State * L)
{
   vy = (int) lua_tonumber (L, 1);
   return 0;
}



static int KQ_get_alldead (lua_State * L)
{
   lua_pushnumber (L, alldead);
   return 1;
}



static int KQ_set_alldead (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      alldead = a;
   return 0;
}



static int KQ_get_autoparty (lua_State * L)
{
   lua_pushnumber (L, autoparty);
   return 1;
}



static int KQ_set_autoparty (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      autoparty = a;
   return 0;
}



static int KQ_get_numchrs (lua_State * L)
{
   lua_pushnumber (L, numchrs);
   return 1;
}



static int KQ_set_all_equip (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b, c;

   if (a < 0 || a > MAXCHRS)
      return 0;
   for (b = 0; b < 6; b++) {
      c = (int) lua_tonumber (L, b + 2);
      if (c >= 0)
         party[a].eqp[b] = c;
   }
   return 0;
}



static int KQ_set_btile (lua_State * L)
{
   set_btile ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2),
              (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_set_mtile (lua_State * L)
{
   set_mtile ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2),
              (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_set_ftile (lua_State * L)
{
   set_ftile ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2),
              (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_set_obs (lua_State * L)
{
   set_obs ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2),
            (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_set_zone (lua_State * L)
{
   set_zone ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2),
             (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_set_shadow (lua_State * L)
{
   set_shadow ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2),
               (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_set_tile_all (lua_State * L)
{
   int a, tvs[8];

   for (a = 0; a < 8; a++)
      tvs[a] = (int) lua_tonumber (L, a + 1);
   if (tvs[2] >= 0)
      set_btile (tvs[0], tvs[1], tvs[2]);
   if (tvs[3] >= 0)
      set_mtile (tvs[0], tvs[1], tvs[3]);
   if (tvs[4] >= 0)
      set_ftile (tvs[0], tvs[1], tvs[4]);
   if (tvs[5] >= 0)
      set_zone (tvs[0], tvs[1], tvs[5]);
   if (tvs[6] >= 0)
      set_obs (tvs[0], tvs[1], tvs[6]);
   if (tvs[7] >= 0)
      set_shadow (tvs[0], tvs[1], tvs[7]);
   return 0;
}



/* PH: not used (yet?), therefore commented out */
#if 0
static int KQ_get_tile_all (lua_State * L)
{
   int tx = (int) lua_tonumber (L, 1);
   int ty = (int) lua_tonumber (L, 2);
   int offset = tx + ty * g_map.xsize;

   lua_pushnumber (L, map_seg[offset]);
   lua_pushnumber (L, b_seg[offset]);
   lua_pushnumber (L, f_seg[offset]);
   lua_pushnumber (L, z_seg[offset]);
   lua_pushnumber (L, o_seg[offset]);
   lua_pushnumber (L, s_seg[offset]);
   return 6;
}
#endif



/*! \brief Copy tile block
 * \author PH
 * \date Created 20021125
 *
 * Copies a region of the map (all layers).
 * Invocation: copy_tile_all(source_x, source_y, dest_x, dest_y, width,
 *                           height)
 * These params are meant to be similar to the allegro blit() function.
 *
 * \param   L The Lua VM
 * \returns 0 (no values returned to Lua)
 * \bugs    No error checking is done. Uses direct access to the struct s_map.
 */
static int KQ_copy_tile_all (lua_State * L)
{
   int sx = (int) lua_tonumber (L, 1);
   int sy = (int) lua_tonumber (L, 2);
   int dx = (int) lua_tonumber (L, 3);
   int dy = (int) lua_tonumber (L, 4);
   int wid = (int) lua_tonumber (L, 5);
   int hgt = (int) lua_tonumber (L, 6);
   int os, od, i, j;
   /*      sprintf (strbuf, "Copy (%d,%d)x(%d,%d) to (%d,%d)", sx, sy, wid,
      hgt, dx, dy);
      klog(strbuf);
    */
   for (j = 0; j < hgt; ++j) {
      for (i = 0; i < wid; ++i) {
         os = sx + i + g_map.xsize * (sy + j);
         od = dx + i + g_map.xsize * (dy + j);
         map_seg[od] = map_seg[os];
         f_seg[od] = f_seg[os];
         b_seg[od] = b_seg[os];
         z_seg[od] = z_seg[os];
         o_seg[od] = o_seg[os];
         s_seg[od] = s_seg[os];
      }
   }
   return 0;
}



static int KQ_set_desc (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      display_desc = a;
   return 0;
}



static int KQ_set_foreground (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      draw_foreground = a;
   return 0;
}



static int KQ_set_holdfade (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      hold_fade = a;
   return 0;
}



static int KQ_get_noe (lua_State * L)
{
   lua_pushnumber (L, noe);
   return 1;
}



static int KQ_set_noe (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 50 + PSIZE)
      noe = a;
   return 0;
}



static int KQ_set_run (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      can_run = a;
   return 0;
}



static int KQ_set_save (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      cansave = a;
   return 0;
}



static int KQ_set_sstone (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      use_sstone = a;
   return 0;
}



static int KQ_set_warp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      g_map.can_warp = a;
   g_map.warpx = (int) lua_tonumber (L, 2);
   g_map.warpy = (int) lua_tonumber (L, 3);
   return 0;
}



static int KQ_set_vfollow (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a == 0 || a == 1)
      vfollow = a;
   return 0;
}



static int KQ_create_df (lua_State * L)
{
   g_df = load_datafile_object (lua_tostring (L, 1), lua_tostring (L, 2));
   return 0;
}



static int KQ_destroy_df (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   unload_datafile_object (g_df);
   return 0;
}



static int KQ_create_bmp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 4)
      g_bmp[a] =
         create_bitmap ((int) lua_tonumber (L, 2), (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_destroy_bmp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 4)
      destroy_bitmap (g_bmp[a]);
   return 0;
}



static int KQ_df2bmp (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 4)
      blit ((BITMAP *) g_df->dat, g_bmp[a], (int) lua_tonumber (L, 2),
            (int) lua_tonumber (L, 3), (int) lua_tonumber (L, 4),
            (int) lua_tonumber (L, 5), (int) lua_tonumber (L, 6),
            (int) lua_tonumber (L, 7));
   return 0;
}



static int KQ_blit (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 4)
      blit (g_bmp[a], double_buffer, (int) lua_tonumber (L, 2),
            (int) lua_tonumber (L, 3), (int) lua_tonumber (L, 4) + xofs,
            (int) lua_tonumber (L, 5) + yofs, (int) lua_tonumber (L, 6),
            (int) lua_tonumber (L, 7));
   return 0;
}



static int KQ_maskblit (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 4)
      masked_blit (g_bmp[a], double_buffer, (int) lua_tonumber (L, 2),
                   (int) lua_tonumber (L, 3), (int) lua_tonumber (L, 4) + xofs,
                   (int) lua_tonumber (L, 5) + yofs, (int) lua_tonumber (L, 6),
                   (int) lua_tonumber (L, 7));
   return 0;
}



static int KQ_drawsprite (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 4)
      draw_sprite (double_buffer, g_bmp[a], (int) lua_tonumber (L, 2) + xofs,
                   (int) lua_tonumber (L, 3) + yofs);
   return 0;
}

/*! \brief Update the screen */
static int KQ_screen_dump (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   blit2screen (xofs, yofs);
   return 0;
}



static int KQ_drawmap (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   drawmap ();
   return 0;
}



/*! \brief Implement Lua prompt dialog
 *
 * This is an interface from the old-style prompt to the new prompt_ex.
 * In the old style you could specify talk prompts or thought prompts,
 * and specify the number of options (n). The prompt would consist of (L-n)
 * text lines and n options, where L is the number of _non-blank_ lines,
 * up to 4.
 *
 * \date 20030506
 * \author PH
 * \bug Long strings will overflow the buffer
 */
static int KQ_prompt (lua_State * L)
{
   char *txt[4];
   char pbuf[256];
   int a, b, nopts, nonblank;

   /* The B_TEXT or B_THOUGHT is ignored */
   b = real_entity_num ((int) lua_tonumber (L, 1));
   nopts = (int) lua_tonumber (L, 2);

   if (nopts > 4)
      nopts = 4;

   pbuf[0] = '\0';
   nonblank = 0;

   for (a = 0; a < 4; a++) {
      txt[a] = (char *) lua_tostring (L, a + 4);
      if (txt[a] && (strlen (txt[a]) > 0))
         nonblank = a + 1;
   }

   if (nonblank > nopts) {
      /* bug: long strings will crash it! */
      for (a = 0; a < nonblank - nopts; ++a) {
         if (a != 0)
            strcat (pbuf, "\n");
         strcat (pbuf, txt[a]);
      }
      lua_pushnumber (L, prompt_ex (b, pbuf, &txt[nonblank - nopts], nopts));
   } else {
      /* User asked for all the lines to be options */
      lua_pushnumber (L, prompt_ex (b, "Choose one", txt, nopts));
   }

   return 1;
}



static int KQ_copy_ent (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = real_entity_num ((int) lua_tonumber (L, 2));

   g_ent[b] = g_ent[a];
   return 0;
}



static int KQ_place_ent (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));

   place_ent (a, (int) lua_tonumber (L, 2), (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_face_each_other (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b = real_entity_num ((int) lua_tonumber (L, 2));

   if (numchrs == 2) {
      if (g_ent[a].tilex == g_ent[b].tilex) {
         if (g_ent[a].tiley > g_ent[b].tiley) {
            g_ent[a].facing = FACE_UP;
            g_ent[b].facing = FACE_DOWN;
         } else {
            g_ent[a].facing = FACE_DOWN;
            g_ent[b].facing = FACE_UP;
         }
      } else {
         if (g_ent[a].tilex > g_ent[b].tilex) {
            g_ent[a].facing = FACE_LEFT;
            g_ent[b].facing = FACE_RIGHT;
         } else {
            g_ent[a].facing = FACE_RIGHT;
            g_ent[b].facing = FACE_LEFT;
         }
      }
   }
   return 0;
}



static int KQ_wait_for_entity (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   int b =
      lua_gettop (L) > 1 ? real_entity_num ((int) lua_tonumber (L, 2)) : a;

   wait_for_entity (a, b);
   return 0;
}



static int KQ_add_chr (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (numchrs < MAXCHRS) {
      pidx[numchrs] = a;
      g_ent[numchrs].active = 1;
      g_ent[numchrs].eid = a;
      g_ent[numchrs].chrx = 0;
      numchrs++;
   }
   return 0;
}



static int KQ_remove_chr (lua_State * L)
{
   int a, b;

   if (numchrs > 0) {
      a = in_party ((int) lua_tonumber (L, 1));
      if (a > 0) {
         a--;
         pidx[a] = -1;
         numchrs--;
         if (a != MAXCHRS - 1) {
            for (b = 0; b < MAXCHRS - 1; b++) {
               if (pidx[b] == -1) {
                  pidx[b] = pidx[b + 1];
                  pidx[b + 1] = -1;
               }
            }
         }
      }
   }
   return 0;
}



static int KQ_krnd (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a > 0)
      lua_pushnumber (L, rand () % a);
   else
      lua_pushnumber (L, 0);
   return 1;
}



static int KQ_do_fadeout (lua_State * L)
{
   do_transition (TRANS_FADE_OUT, (int) lua_tonumber (L, 1));
   return 0;
}



static int KQ_do_fadein (lua_State * L)
{
   do_transition (TRANS_FADE_IN, (int) lua_tonumber (L, 1));
   return 0;
}



static int KQ_shop (lua_State * L)
{
   shop ((int) lua_tonumber (L, 1));
   return 0;
}



static int KQ_inn (lua_State * L)
{
   inn ((char *) lua_tostring (L, 1), (int) lua_tonumber (L, 2),
        (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_door_in (lua_State * L)
{
   int hx, hy, hy2, db, dt;

   use_sstone = 0;

   hx = g_ent[0].tilex;
   hy = g_ent[0].tiley;
   hy2 = hy - 1;
   db = map_seg[hy * g_map.xsize + hx];
   dt = map_seg[hy2 * g_map.xsize + hx];
   if (g_map.tileset == 1) {
      set_btile (hx, hy, db + 433);
      if (dt == 149)
         set_btile (hx, hy2, 571);
      else
         set_btile (hx, hy2, dt + 433);
   } else {
      set_btile (hx, hy, db + 3);
   }
   play_effect (25, 128);
   drawmap ();
   blit2screen (xofs, yofs);
   wait (50);
   set_view (1, (int) lua_tonumber (L, 3), (int) lua_tonumber (L, 4),
             (int) lua_tonumber (L, 5), (int) lua_tonumber (L, 6));
   warp ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2), 8);
   set_btile (hx, hy, db);
   if (g_map.tileset == 1)
      set_btile (hx, hy2, dt);
   return 0;
}



static int KQ_door_out (lua_State * L)
{
   use_sstone = 1;
   set_view (0, 0, 0, 0, 0);
   warp ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2), 8);
   return 0;
}



static int KQ_calc_viewport (lua_State * L)
{
   calc_viewport ((int) lua_tonumber (L, 1));
   return 0;
}



static int KQ_change_map (lua_State * L)
{
   strcpy (tmap_name, (char *) lua_tostring (L, 1));
   tmx = (int) lua_tonumber (L, 2);
   tmy = (int) lua_tonumber (L, 3);
   tmvx = (int) lua_tonumber (L, 4);
   tmvy = (int) lua_tonumber (L, 5);
   changing_map = 1;
   return 0;
}



static int KQ_give_item (lua_State * L)
{
   check_inventory ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2));
   return 0;
}



static int KQ_warp (lua_State * L)
{
   warp ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2),
         (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_give_xp (lua_State * L)
{
   give_xp ((int) lua_tonumber (L, 1), (int) lua_tonumber (L, 2),
            (int) lua_tonumber (L, 3));
   return 0;
}



static int KQ_chest (lua_State * L)
{
   int gd = 0, a = 0;
   int tno = (int) lua_tonumber (L, 1);
   int ino = (int) lua_tonumber (L, 2);
   int amt = (int) lua_tonumber (L, 3);

   if (tno > -1)
      if (treasure[tno] != 0)
         return 0;
   if (ino == 0) {
      gp += amt;
      sprintf (strbuf, "Found %d gp!", amt);
      play_effect (SND_MONEY, 128);
      message (strbuf, 255, 0, xofs, yofs);
      if (tno > -1)
         treasure[tno] = 1;
      return 0;
   }
   /* PH TODO: This next bit is not needed because the inventory is shared */
   if (numchrs == 1) {
      if (check_inventory (ino, amt) > 0)
         gd = 1;
   } else {
      while (a < numchrs) {
         if (check_inventory (ino, amt) > 0) {
            gd = 1;
            a = numchrs;
         }
         a++;
      }
   }
   if (gd == 1) {
      if (amt == 1)
         sprintf (strbuf, "%s procured!", items[ino].name);
      else
         sprintf (strbuf, "%s ^%d procured", items[ino].name, amt);
      play_effect (SND_UNEQUIP, 128);
      message (strbuf, items[ino].icon, 0, xofs, yofs);
      if (tno > -1)
         treasure[tno] = 1;
      return 0;
   }
   if (amt == 1)
      sprintf (strbuf, "%s not taken!", items[ino].name);
   else
      sprintf (strbuf, "%s ^%d not taken!", items[ino].name, amt);
   message (strbuf, items[ino].icon, 0, xofs, yofs);
   return 0;
}



static int KQ_combat (lua_State * L)
{
   combat ((int) lua_tonumber (L, 1));
   return 0;
}



static int KQ_wait (lua_State * L)
{
   kwait ((int) lua_tonumber (L, 1));
   return 0;
}



static int KQ_rest (lua_State * L)
{
   wait ((int) lua_tonumber (L, 1));
   return 0;
}


/*! \brief Show message on the screen
 *
 * Show a brief message for a set period of time, or
 * until ALT is pressed.
 *
 * \param L::1 String message to show
 * \param L::2 Icon number or 255 for none (icons
 *             are displayed, for instance, when items are procured)
 * \param L::3 Delay time (see wait()) , or 0 for indefinite
 * \returns 0 (no value returned)
 *
 * 20040308 PH added code to default missing L::2 parameter to 255 (instead of 0)
 */
static int KQ_msg (lua_State * L)
{
   int icn = lua_isnumber (L, 2) ? (int) lua_tonumber (L, 2) : 255;
   message ((char *) lua_tostring (L, 1), icn, (int) lua_tonumber (L, 3), xofs,
            yofs);
   return 0;
}



static int KQ_move_camera (lua_State * L)
{
   int ct2 = 0, xinc = 0, yinc = 0, xtot = 0, ytot = 0;
   int mcx = (int) lua_tonumber (L, 1);
   int mcy = (int) lua_tonumber (L, 2);
   int dtime = (int) lua_tonumber (L, 3);

   if (mcx > vx) {
      xinc = 1;
      xtot = mcx - vx;
   }
   if (mcx < vx) {
      xinc = -1;
      xtot = vx - mcx;
   }
   if (mcy > vy) {
      yinc = 1;
      ytot = mcy - vy;
   }
   if (mcy < vy) {
      yinc = -1;
      ytot = vy - mcy;
   }
   autoparty = 1;
   timer_count = 0;
   while (ytot > 0 || xtot > 0) {
      while (timer_count > 0) {
         timer_count--;
         ct2++;
         if (ct2 > dtime) {
            if (xtot > 0) {
               vx += xinc;
               xtot--;
            }
            if (ytot > 0) {
               vy += yinc;
               ytot--;
            }
            ct2 = 0;
         }
         check_animation ();
      }
      drawmap ();
      blit2screen (xofs, yofs);
   }
   timer_count = 0;
   autoparty = 0;
   return 0;
}



static int KQ_in_forest (lua_State * L)
{
   int a = real_entity_num ((int) lua_tonumber (L, 1));
   lua_pushnumber (L, is_forestsquare (g_ent[a].tilex, g_ent[a].tiley));
   return 1;
}



static int KQ_orient_heroes (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   if (numchrs == 2) {
      lastm[1] = MOVE_NOT;
      if (g_ent[0].tilex == g_ent[1].tilex && g_ent[0].tiley == g_ent[1].tiley) {
         lastm[0] = MOVE_NOT;
         return 0;
      }
      if (g_ent[0].tilex == g_ent[1].tilex) {
         if (g_ent[0].tiley < g_ent[1].tiley)
            lastm[0] = MOVE_UP;
         else
            lastm[0] = MOVE_DOWN;
         return 0;
      }
      if (g_ent[0].tiley == g_ent[1].tiley) {
         if (g_ent[0].tilex < g_ent[1].tilex)
            lastm[0] = MOVE_LEFT;
         else
            lastm[0] = MOVE_RIGHT;
         return 0;
      }
   }
   return 0;
}



static int KQ_pause_song (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   pause_music ();
   return 0;
}



static int KQ_unpause_map_song (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   resume_music ();
   return 0;
}



static int KQ_play_map_song (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   play_music (g_map.song_file, 0);
   return 0;
}



static int KQ_play_song (lua_State * L)
{
   play_music ((char *) lua_tostring (L, 1), 0);
   return 0;
}



static int KQ_stop_song (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   stop_music ();
   return 0;
}



static int KQ_view_range (lua_State * L)
{
   int a, b[5];

   for (a = 0; a < 5; a++)
      b[a] = (int) lua_tonumber (L, a + 1);
   set_view (b[0], b[1], b[2], b[3], b[4]);
   return 0;
}



static int KQ_sfx (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   play_effect (a, 128);
   return 0;
}



static int KQ_clear_buffer (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   clear_bitmap (double_buffer);
   return 0;
}



static int KQ_mbox (lua_State * L)
{
   menubox (double_buffer, (int) lua_tonumber (L, 1) + xofs,
            (int) lua_tonumber (L, 2) + yofs, (int) lua_tonumber (L, 3),
            (int) lua_tonumber (L, 4), BLUE);
   return 0;
}



static int KQ_dark_mbox (lua_State * L)
{
   menubox (double_buffer, (int) lua_tonumber (L, 1) + xofs,
            (int) lua_tonumber (L, 2) + yofs, (int) lua_tonumber (L, 3),
            (int) lua_tonumber (L, 4), DARKBLUE);
   return 0;
}



static int KQ_light_mbox (lua_State * L)
{
   menubox (double_buffer, (int) lua_tonumber (L, 1) + xofs,
            (int) lua_tonumber (L, 2) + yofs, (int) lua_tonumber (L, 3),
            (int) lua_tonumber (L, 4), DARKRED);
   return 0;
}



static int KQ_drawframe (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b = (int) lua_tonumber (L, 2);

   draw_sprite (double_buffer, frames[a][b], (int) lua_tonumber (L, 3) + xofs,
                (int) lua_tonumber (L, 4) + yofs);
   return 0;
}



static int KQ_draw_pstat (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      draw_playerstat (double_buffer, a, (int) lua_tonumber (L, 2) + xofs,
                       (int) lua_tonumber (L, 3) + yofs);
   return 0;
}



static int KQ_ptext (lua_State * L)
{
   print_font (double_buffer, (int) lua_tonumber (L, 1) + xofs,
               (int) lua_tonumber (L, 2) + yofs, (char *) lua_tostring (L, 3),
               (int) lua_tonumber (L, 4));
   return 0;
}



static int KQ_pnum (lua_State * L)
{
   int a = (int) lua_tonumber (L, 3);

   sprintf (strbuf, "%d", a);
   print_font (double_buffer, (int) lua_tonumber (L, 1) + xofs,
               (int) lua_tonumber (L, 2) + yofs, strbuf, (int) lua_tonumber (L,
                                                                             4));
   return 0;
}



static int KQ_read_controls (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);
   int b = (int) lua_tonumber (L, 2);
   int c = (int) lua_tonumber (L, 3);
   int d = (int) lua_tonumber (L, 4);
   int e = (int) lua_tonumber (L, 5);
   int f = (int) lua_tonumber (L, 6);
   int g = (int) lua_tonumber (L, 7);
   int h = (int) lua_tonumber (L, 8);
   int z;

   for (z = 0; z < 8; z++)
      g_keys[z] = 0;

   readcontrols ();
   if (up && a == 1) {
      unpress ();
      g_keys[0] = 1;
   }
   if (down && b == 1) {
      unpress ();
      g_keys[1] = 1;
   }
   if (left && c == 1) {
      unpress ();
      g_keys[2] = 1;
   }
   if (right && d == 1) {
      unpress ();
      g_keys[3] = 1;
   }
   if (balt && e == 1) {
      unpress ();
      g_keys[4] = 1;
   }
   if (bctrl && f == 1) {
      unpress ();
      g_keys[5] = 1;
   }
   if (benter && g == 1) {
      unpress ();
      g_keys[6] = 1;
   }
   if (besc && h == 1) {
      unpress ();
      g_keys[7] = 1;
   }
   return 0;
}



static int KQ_check_key (lua_State * L)
{
   int a = (int) lua_tonumber (L, 1);

   if (a >= 0 && a <= 7)
      lua_pushnumber (L, g_keys[a]);
   else
      lua_pushnumber (L, 0);
   return 1;
}



static int KQ_log (lua_State * L)
{
   klog ((char *) lua_tostring (L, 1));
   return 0;
}



static int KQ_wait_enter (lua_State * L)
{
/*  RB: TODO  */
   if (L != NULL)
      L = L;
   wait_enter ();
   return 0;
}


extern int useup_item (int);
static int KQ_use_up (lua_State * L)
{
   int i = (int) lua_tonumber (L, 1);
   lua_pushnumber (L, useup_item (i));
   return 1;
}



/* The text_ex function just takes one string, and does the line breaks automatically.
 * The old bubble/thought functions which took four strings are handled by
 * code in global.lua. This is for backward compatibility with the old scripts.
 * You can use either, but bubble_ex() does avoid some extra processing.
 * The 'ent' param can be a number, or an object e.g. party[0].
 */
int KQ_bubble_ex (lua_State * L)
{
   int entity;
   s_entity *ent;
   const char *msg = lua_tostring (L, 2);

   switch (lua_type (L, 1)) {
   case LUA_TNUMBER:
      entity = real_entity_num ((int) lua_tonumber (L, 1));
      break;
   case LUA_TTABLE:
      lua_pushstring (L, "_ent");
      lua_rawget (L, 1);
      ent = lua_touserdata (L, -1);
      /* convert from pointer to an index for text_ex */
      entity = ent ? ent - g_ent : 255;
      break;
   default:
      entity = 255;
      break;
   }
   text_ex (B_TEXT, entity, msg);
   return 0;
}


int KQ_thought_ex (lua_State * L)
{
   int entity = real_entity_num ((int) lua_tonumber (L, 1));
   const char *msg = lua_tostring (L, 2);
   text_ex (B_THOUGHT, entity, msg);
   return 0;
}



/*! \brief Do a battle
 *
 * Usage: battle (bg_name, mus_name, encounter, encounter, ...);
 * bg_name: name of background tile
 * mus_name: name of music track
 * encounter: list of encounters
 *
 * Format of encounter: {prob, lvl; monster, monster, ...}
 * prob: probability this will occur
 * lvl: level of this battle
 * monster: name of a monster
 */
int KQ_battle (lua_State * L)
{
   L = L;
   return 1;
}



int KQ_istable (lua_State * L)
{
   if (lua_istable (L, 1)) {
      lua_pushnumber (L, 1);
   } else {
      lua_pushnil (L);
   }
   return 1;
}



int KQ_select_team (lua_State * L)
{
   static int team[MAXCHRS];
   int i, t;
   for (i = 0; i < MAXCHRS; ++i) {
      lua_rawgeti (L, 1, i + 1);
      if (lua_type (L, -1) == LUA_TNIL) {
         team[i] = -1;
      } else {
         team[i] = (int) lua_tonumber (L, -1);
         lua_pushnil (L);
         lua_rawseti (L, 1, i + 1);
      }
      lua_pop (L, 1);
   }
   select_party (team, MAXCHRS, 2);
   t = 1;
   for (i = 0; i < MAXCHRS; ++i) {
      if (team[i] != -1) {
         lua_pushnumber (L, team[i]);
         lua_rawseti (L, 1, t++);
      }
   }
   return 1;
}

int KQ_set_map_mode (lua_State * L)
{
   g_map.map_mode = (int) lua_tonumber (L, 1);
   return 0;
}

/*! \brief Initialise scripting engine
 *
 * Initialise the Lua scripting engine by loading from a file. A new VM is
 * created each time.
 *
 * \param   fname Base name of script; xxxxx loads script scripts/xxxxx.lob
 */
void do_luainit (char *fname)
{
   int oldtop;
   char sname[32];
   const struct luaL_reg *rg = lrs;
   if (theL != NULL) {
      do_luakill ();
   }
   theL = lua_open (0);
   if (theL == NULL)
      program_death ("Could not initialise scripting engine");
   fieldsort ();
   while (rg->name) {
      lua_register (theL, rg->name, rg->func);
      ++rg;
   }
   init_obj (theL);
   oldtop = lua_gettop (theL);
   if (lua_dofile (theL, kqres (SCRIPT_DIR, "global.lob")) != 0) {
      sprintf (strbuf, "Could not open script: global.lob");
      program_death (strbuf);
   }

   sprintf (sname, "%s.lob", fname);
   if (lua_dofile (theL, kqres (SCRIPT_DIR, sname)) != 0) {
      sprintf (strbuf, "Could not open script:%s", fname);
      program_death (strbuf);
   }
   lua_getglobal (theL, "zone_handler");
   ref_zone_handler = lua_ref (theL, 1);
   lua_settop (theL, oldtop);
}



#ifdef KQ_CHEATS
/*! \brief Load cheat code
 *
 * Load the contents of scripts/cheat.lob, usually in response to F10 being
 * pressed.  This can contain any scripting code, in the function cheat().
 * The variable \p cheat_loaded appears to be provided to ensure the cheat
 * code is loaded once only. However it's never set, so the check never fails.
 */
void do_luacheat (void)
{
   int oldtop;

   oldtop = lua_gettop (theL);
   if (cheat_loaded == 0) {
      lua_dofile (theL, kqres (SCRIPT_DIR, "cheat.lob"));
      /* PH FIXME cheat_loaded=1; here surely?? */
      /* TT: Sure, why not? */
      cheat_loaded = 1;
   }
   lua_getglobal (theL, "cheat");
   lua_call (theL, 0, 0);
   lua_settop (theL, oldtop);
   check_map_change ();
   message ("Cheating complete.", 255, 50, xofs, yofs);
}
#endif



/*! \brief Kill the lua VM
 *
 * Close the Lua virtual machine, and note that the cheat code should be
 * reloaded if called.
 */
void do_luakill (void)
{
   if (theL) {
      lua_unref (theL, ref_zone_handler);
      lua_close (theL);
      theL = NULL;
   }
#ifdef KQ_CHEATS
   cheat_loaded = 0;
#endif
}



/*! \brief Run initial code
 *
 * Calls the function autoexec() which should contain some initial start-up
 * routines for this map.  This occurs while the map is faded out.  This code
 * should NOT call any graphical functions because this causes KQ to lock.
 * Instead, use postexec().
 */
void do_autoexec (void)
{
   int oldtop = lua_gettop (theL);

   lua_getglobal (theL, "autoexec");
   lua_call (theL, 0, 0);
   lua_settop (theL, oldtop);
   check_map_change ();
}



/*! \brief Run initial graphical code
 *
 * This function is called after the map is faded back in.  It's possible to
 * show speech, move entities, etc. here.
 */
void do_postexec (void)
{
   int oldtop = lua_gettop (theL);

   lua_getglobal (theL, "postexec");
   lua_call (theL, 0, 0);
   lua_settop (theL, oldtop);
   check_map_change ();
}



/*! \brief Trigger zone action
 *
 * Run the Lua function zone_handler(int) to take action based on the zone
 * that the hero has just stepped on.  This function is not called for zone 0,
 * unless the map property zero_zone is non-zero.
 *
 * PH used the Lua ref system here; should be quicker to get a reference to
 * 'zone_handler' than to do a look-up based on the name.  Whether this is
 * significant I don't know.
 *
 * \param   zn_num Zone number
 */
void do_zone (int zn_num)
{
   int oldtop = lua_gettop (theL);

   lua_getref (theL, ref_zone_handler);
   lua_pushnumber (theL, zn_num);
   lua_call (theL, 1, 0);
   lua_settop (theL, oldtop);
   check_map_change ();
}



/*! \brief Trigger entity action
 *
 * Run the lua function entity_handler(int) to take action based on the entity
 * that the hero has just approached and pressed ALT.
 *
 * \param   en_num Entity number
 */
void do_entity (int en_num)
{
   int oldtop = lua_gettop (theL);

   lua_getglobal (theL, "entity_handler");
   lua_pushnumber (theL, en_num - PSIZE);
   lua_call (theL, 1, 0);
   lua_settop (theL, oldtop);
   check_map_change ();
}



/*! \brief Check to change the map
 *
 * Check to see if we can change the map.  Does nothing if we are already in
 * the process of changing the map.
 */
static void check_map_change (void)
{
   if (changing_map == 0)
      return;
   changing_map = 0;
   change_map (tmap_name, tmx, tmy, tmvx, tmvy);
}
