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

#ifndef __INTRFACE_H
#define __INTRFACE_H

#include "lua.h"

#define NUM_IFUNCS   142

struct luaL_reg
{
   const char *name;
   lua_CFunction func;
};

int KQ_get_pidx (lua_State *);
int KQ_get_progress (lua_State *);
int KQ_set_progress (lua_State *);
int KQ_get_party_name (lua_State *);
int KQ_get_party_xp (lua_State *);
int KQ_set_party_xp (lua_State *);
int KQ_get_party_next (lua_State *);
int KQ_set_party_next (lua_State *);
int KQ_get_party_lvl (lua_State *);
int KQ_set_party_lvl (lua_State *);
int KQ_get_party_mrp (lua_State *);
int KQ_set_party_mrp (lua_State *);
int KQ_get_party_hp (lua_State *);
int KQ_set_party_hp (lua_State *);
int KQ_get_party_mhp (lua_State *);
int KQ_set_party_mhp (lua_State *);
int KQ_get_party_mp (lua_State *);
int KQ_set_party_mp (lua_State *);
int KQ_get_party_mmp (lua_State *);
int KQ_set_party_mmp (lua_State *);
int KQ_get_party_eqp (lua_State *);
int KQ_set_party_eqp (lua_State *);
int KQ_get_party_stats (lua_State *);
int KQ_set_party_stats (lua_State *);
int KQ_get_party_res (lua_State *);
int KQ_set_party_res (lua_State *);
int KQ_get_ent_chrx (lua_State *);
int KQ_set_ent_chrx (lua_State *);
int KQ_get_ent_tilex (lua_State *);
int KQ_set_ent_tilex (lua_State *);
int KQ_get_ent_tiley (lua_State *);
int KQ_set_ent_tiley (lua_State *);
int KQ_get_ent_id (lua_State *);
int KQ_set_ent_id (lua_State *);
int KQ_get_ent_active (lua_State *);
int KQ_set_ent_active (lua_State *);
int KQ_get_ent_facing (lua_State *);
int KQ_set_ent_facing (lua_State *);
int KQ_get_ent_movemode (lua_State *);
int KQ_set_ent_movemode (lua_State *);
int KQ_get_ent_obsmode (lua_State *);
int KQ_set_ent_obsmode (lua_State *);
int KQ_get_ent_speed (lua_State *);
int KQ_set_ent_speed (lua_State *);
int KQ_get_ent_atype (lua_State *);
int KQ_set_ent_atype (lua_State *);
int KQ_get_ent_snapback (lua_State *);
int KQ_set_ent_snapback (lua_State *);
int KQ_get_ent_facehero (lua_State *);
int KQ_set_ent_facehero (lua_State *);
int KQ_get_ent_transl (lua_State *);
int KQ_set_ent_transl (lua_State *);
int KQ_set_ent_script (lua_State *);
int KQ_get_treasure (lua_State *);
int KQ_get_gp (lua_State *);
int KQ_set_gp (lua_State *);
int KQ_get_vx (lua_State *);
int KQ_set_vx (lua_State *);
int KQ_get_vy (lua_State *);
int KQ_set_vy (lua_State *);
int KQ_get_alldead (lua_State *);
int KQ_set_alldead (lua_State *);
int KQ_get_autoparty (lua_State *);
int KQ_set_autoparty (lua_State *);
int KQ_get_numchrs (lua_State *);
int KQ_set_all_equip (lua_State *);
int KQ_set_btile (lua_State *);
int KQ_set_mtile (lua_State *);
int KQ_set_ftile (lua_State *);
int KQ_set_obs (lua_State *);
int KQ_set_zone (lua_State *);
int KQ_set_shadow (lua_State *);
int KQ_set_tile_all (lua_State *);
int KQ_set_desc (lua_State *);
int KQ_set_foreground (lua_State *);
int KQ_set_holdfade (lua_State *);
int KQ_get_noe (lua_State *);
int KQ_set_noe (lua_State *);
int KQ_set_run (lua_State *);
int KQ_set_save (lua_State *);
int KQ_set_sstone (lua_State *);
int KQ_set_warp (lua_State *);
int KQ_set_vfollow (lua_State *);
int KQ_create_df (lua_State *);
int KQ_destroy_df (lua_State *);
int KQ_create_bmp (lua_State *);
int KQ_destroy_bmp (lua_State *);
int KQ_df2bmp (lua_State *);
int KQ_blit (lua_State *);
int KQ_maskblit (lua_State *);
int KQ_drawsprite (lua_State *);
int KQ_screen_dump (lua_State *);
int KQ_drawmap (lua_State *);
int KQ_prompt (lua_State *);
int KQ_bubble (lua_State *);
int KQ_thought (lua_State *);
int KQ_copy_ent (lua_State *);
int KQ_place_ent (lua_State *);
int KQ_face_each_other (lua_State *);
int KQ_wait_for_entity (lua_State *);
int KQ_add_chr (lua_State *);
int KQ_remove_chr (lua_State *);
int KQ_krnd (lua_State *);
int KQ_book_talk (lua_State *);
int KQ_do_fadeout (lua_State *);
int KQ_do_fadein (lua_State *);
int KQ_shop (lua_State *);
int KQ_inn (lua_State *);
int KQ_door_in (lua_State *);
int KQ_door_out (lua_State *);
int KQ_calc_viewport (lua_State *);
int KQ_change_map (lua_State *);
int KQ_give_item (lua_State *);
int KQ_warp (lua_State *);
int KQ_give_xp (lua_State *);
int KQ_chest (lua_State *);
int KQ_combat (lua_State *);
int KQ_wait (lua_State *);
int KQ_rest (lua_State *);
int KQ_msg (lua_State *);
int KQ_move_camera (lua_State *);
int KQ_in_forest (lua_State *);
int KQ_orient_heroes (lua_State *);
int KQ_pause_song (lua_State *);
int KQ_unpause_map_song (lua_State *);
int KQ_play_map_song (lua_State *);
int KQ_play_song (lua_State *);
int KQ_stop_song (lua_State *);
int KQ_view_range (lua_State *);
int KQ_sfx (lua_State *);
int KQ_clear_buffer (lua_State *);
int KQ_mbox (lua_State *);
int KQ_dark_mbox (lua_State *);
int KQ_light_mbox (lua_State *);
int KQ_drawframe (lua_State *);
int KQ_draw_pstat (lua_State *);
int KQ_ptext (lua_State *);
int KQ_pnum (lua_State *);
int KQ_read_controls (lua_State *);
int KQ_check_key (lua_State *);
int KQ_log (lua_State *);
int KQ_wait_enter (lua_State *);

void do_luainit (char *);
void do_luacheat (void);
void do_luakill (void);
void do_autoexec (void);
void do_postexec (void);
void do_zone (int);
void do_entity (int);
void check_map_change (void);

#endif
