/*! \page License
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
 * \brief Main include file for KQ
 * \author JB
 * \date ??????
 */

#ifndef __KQ_H
#define __KQ_H



#include <allegro.h>
#include "entity.h"

#define MAX_TILES        1024
#define MAXE               40
#define MAX_ANIM            5
#define MAXCHRS             8
#define PSIZE               2
#define MAXFRAMES          12
#define MAXEFRAMES         12
#define MAXCFRAMES          8
#define NUM_FIGHTERS        7
#define MISS             9999
#define NODISPLAY        9998
#define SEL_ALL_ALLIES   9997
#define SEL_ALL_ENEMIES  9996
#define SCANALL          9995
#define NO_STS_CHECK     9994
#define CURE_CHECK       9993
#define MAX_SHADOWS        12
#define MAX_INV            64
#define STEPS_NEEDED       15

#define DATA_DIR       "data/"
#define MAP_DIR        "maps/"
#define SAVE_DIR       "saves/"
#define MUSIC_DIR      "music/"
#define SCRIPT_DIR     "scripts/"
#define PCX_DATAFILE   "data/mpcx.dat"
#define SPELL_DATAFILE "data/ssprites.dat"
#define SOUND_DATAFILE "data/kqsnd.dat"

/*! \name Map layer setters */
/*\{*/
#define set_btile(tx,ty,t) map_seg[ty*g_map.xsize+tx] = t
#define set_mtile(tx,ty,t) b_seg[ty*g_map.xsize+tx] = t
#define set_ftile(tx,ty,t) f_seg[ty*g_map.xsize+tx] = t
#define set_zone(tx,ty,t) z_seg[ty*g_map.xsize+tx] = t
#define set_obs(tx,ty,t) o_seg[ty*g_map.xsize+tx] = t
#define set_shadow(tx,ty,t) s_seg[ty*g_map.xsize+tx] = t
/*\}*/

/*! \name Attributes of characters */
/*\{*/
#define A_STR 0
#define A_AGI 1
#define A_VIT 2
#define A_INT 3
#define A_SAG 4
#define A_SPD 5
#define A_AUR 6
#define A_SPI 7
#define A_ATT 8
#define A_HIT 9
#define A_DEF 10
#define A_EVD 11
#define A_MAG 12
/*\}*/

/*! \name Spells */
/*\{*/
#define S_POISON   0
#define S_BLIND    1
#define S_CHARM    2
#define S_STOP     3
#define S_STONE    4
#define S_MUTE     5
#define S_SLEEP    6
#define S_DEAD     7
#define S_MALISON  8
#define S_RESIST   9
#define S_TIME     10
#define S_SHIELD   11
#define S_BLESS    12
#define S_STRENGTH 13
#define S_ETHER    14
#define S_TRUESHOT 15
#define S_REGEN    16
#define S_INFUSE   17
/*\}*/

/*! \name Special combat skills */
/*\{*/
#define C_ATTACK   1
#define C_COMBO    2
#define C_SPELL    3
#define C_SKILL    4
#define C_DEFEND   5
#define C_INVOKE   6
#define C_ITEM     7
#define C_RUN      8
/*\}*/

/*! \name Runes */
/*\{*/
#define R_EARTH     0
#define R_BLACK     1
#define R_FIRE      2
#define R_THUNDER   3
#define R_AIR       4
#define R_WHITE     5
#define R_WATER     6
#define R_ICE       7
#define R_POISON    8
#define R_BLIND     9
#define R_CHARM     10
#define R_PARALYZE  11
#define R_PETRIFY   12
#define R_SILENCE   13
#define R_SLEEP     14
#define R_TIME      15
#define R_NONE      16
/*\}*/

/*! \name Weapons */
/*\{*/
#define W_BASH         0
#define W_MACE         1
#define W_HAMMER       2
#define W_SWORD        3
#define W_AXE          4
#define W_KNIFE        5
#define W_SPEAR        6
#define W_ROD          7
#define W_STAFF        8
#define W_SBOOK        27
#define W_ABOOK        28
#define W_CHENDIGAL    29
/*\}*/

/*!\name Use modes 
 * Specify how an item can be used.  
 */
/*\{*/
#define USE_NOT          0
#define USE_ANY_ONCE     1
#define USE_ANY_INF      2
#define USE_CAMP_ONCE    3
#define USE_CAMP_INF     4
#define USE_COMBAT_ONCE  5
#define USE_COMBAT_INF   6
#define USE_ATTACK       7
#define USE_IMBUED       8
/*\}*/

/*! \name Weapon/Spell targetting modes */
/*\{*/
#define TGT_CASTER        -1
#define TGT_NONE          0
#define TGT_ALLY_ONE      1
#define TGT_ALLY_ONEALL   2
#define TGT_ALLY_ALL      3
#define TGT_ENEMY_ONE     4
#define TGT_ENEMY_ONEALL  5
#define TGT_ENEMY_ALL     6
/*\}*/

/*! \name Movement directions */
/*\{*/
#define MOVE_NOT     0
#define MOVE_DOWN    1
#define MOVE_UP      2
#define MOVE_LEFT    3
#define MOVE_RIGHT   4
/*\}*/

/*! \name Facing directions */
/*\{*/
#define FACE_DOWN    0
#define FACE_UP      1
#define FACE_LEFT    2
#define FACE_RIGHT   3
/*\}*/

/*! \name Map identifiers */
/*\{*/
#define MAP_MAIN           0
#define MAP_MANOR          1
#define MAP_TOWN1          2
#define MAP_CAVE1          3
#define MAP_TOWN2          4
#define MAP_BRIDGE         5
#define MAP_TOWN3          6
#define MAP_GROTTO         7
#define MAP_CAVE2          8
#define MAP_FORT           9
#define MAP_CAVE3A         10
#define MAP_CAVE3B         11
#define MAP_TEMPLE1        12
#define MAP_TEMPLE2        13
#define MAP_STARTING       14
#define MAP_TOWER          15
#define MAP_GROTTO2        16
#define MAP_TOWN4          17
#define MAP_CAMP           18
#define MAP_ESTATE         19
#define MAP_TOWN5          20
#define MAP_GUILD          21
#define MAP_DVILLE         22
#define MAP_COLISEUM       23
#define MAP_CAVE4          24
#define MAP_TOWN6          25
#define MAP_PASS           26
#define MAP_TOWN7          27
#define MAP_CULT           28
#define MAP_CAVE5          30   /*sic */
/*\}*/

/*! \brief Animation specifier 
 *
 * Marks a block of tiles that are interchanged to give
 * an animation effect. Used in check_animation()
*/
typedef struct
{
   unsigned short start;        /*!< First tile in sequence  */
   unsigned short end;          /*!< Last tile in sequence */
   unsigned short delay;        /*!< Frames to wait between tile changes */
}
s_anim;


/*! \brief Map definition */
typedef struct
{
   unsigned char map_no;        /*!< Map identifier */
   unsigned char zero_zone;     /*!< Non-zero if zone 0 triggers an event */
   unsigned char map_mode;      /*!< Map mode (see draw_map()) */
   unsigned char can_save;      /*!< Non-zero if Save is allowed in this map */
   unsigned char tileset;       /*!< Which tile-set to use */
   unsigned char use_sstone;    /*!< Non-zero if sunstone works on this map */
   unsigned char can_warp;      /*!< Non-zero if Warp is allowed in this map */
   unsigned char extra_byte;    /*!< Currently unused */
   int xsize;                   /*!< Map width */
   int ysize;                   /*!< Map height */
   int pmult;                   /*!< Parallax multiplier */
   int pdiv;                    /*!< Parallax divider */
   int stx;                     /*!< Default start x-coord */
   int sty;                     /*!< Default start y-coord */
   int warpx;                   /*!< x-coord where warp spell takes you to (see special_spells()) */
   int warpy;                   /*!< y-coord where warp spell takes you to (see special_spells()) */
   int extra_sdword1;           /*!< Not used */
   int extra_sdword2;           /*!< Not used */
   char song_file[16];          /*!< Base file name for map song */
   char map_desc[40];           /*!< Map name (shown when map first appears) */
}
s_map;


/*! \brief Player */
typedef struct
{
   char name[9];                /*!< Entity name */
   int xp;                      /*!< Entity experience */
   int next;                    /*!< Experience needed for level-up */
   int lvl;                     /*!< Entity's level */
   int mrp;                     /*!<  */
   int hp;                      /*!< Hit points */
   int mhp;                     /*!< Maximum hit points */
   int mp;                      /*!< Magic points */
   int mmp;                     /*!< Maximum magic points */
   int stats[13];
   char res[16];
   unsigned char sts[24];
   unsigned char eqp[6];        /*!< Weapons, armor, etc. equipped */
   unsigned char spells[60];    /*!< Known spells */
}
s_player;


/*! \brief Fighter 
 *
 * s_player is transformed into a s_fighter during combat
 */
typedef struct
{
   char name[25];
   int xp;
   int gp;
   int lvl;
   int cx;
   int cy;
   int cw;
   int cl;
   int hp;
   int mhp;
   int mp;
   int mmp;
   int dip;
   int ditmc;
   int ditmr;
   int sitmc;
   int sitmr;
   int stats[13];
   char res[16];
   unsigned char facing;
   unsigned char aframe;
   unsigned char crit;
   unsigned char sts[24];
   unsigned char defend;
   unsigned char ai[8];
   unsigned char aip[8];
   unsigned char atrack[8];
   int csmem;
   int ctmem;
   int cwt;
   int welem;                    /*!< has the ability of 'White' spell element */
   int unl;
   int aux;
   int bonus;
   int bstat;
   int mrp;
   int imb_s;
   int imb_a;
   int imb[2];
}
s_fighter;



void change_map (char *, int, int, int, int);   /*  intrface.c, magic.c  */
void readcontrols (void);       /*  everywhere ;)  */
void calc_viewport (int);       /*  entity.c, intrface.c  */
void zone_check (void);         /*  entity.c  */
void warp (int, int, int);      /*  only in intrface.c  */
void check_animation (void);    /*  draw.c, intrface.c  */
void activate (void);           /*  only in entity.c  */
void unpress (void);            /*  everywhere ;)  */
void wait_enter (void);         /*  everywhere ;)  */
void klog (char *);             /*  draw.c, intrface.c, magic.c, setup.c  */
void init_players (void);       /*  sgame.c  */
void kwait (int);               /*  intrface.c  */
void program_death (char *);    /*  everywhere ;)  */
int in_party (int);             /*  combat.c, intrface.c  */
void wait_for_entity (int, int);        /*  intrface.c  */



extern char curmap[16];         /*  sgame.c  */

extern int right;               /*  intrface.c, (eqp|item|mas|shop)menu.c,  */
                        /*  entity.c, menu.c, selector.c, setup.c,  */
                        /*  heroc.c                                 */

extern int left;                /*  intrface.c, (eqp|item|mas|shop)menu.c,  */
                        /*  entity.c, menu.c, selector.c, setup.c,  */
                        /*  heroc.c                                 */

extern int up;                  /*  selector.c, (eqp|item|mas|shop)menu.c,  */
                        /*  heroc.c, intrface.c, menu.c, entity.c,  */
                        /*  setup.c, sgame.c, draw.c                */

extern int down;                /*  selector.c, (eqp|item|mas|shop)menu.c,  */
                        /*  heroc.c, intrface.c, menu.c, entity.c,  */
                        /*  setup.c, sgame.c, draw.c                */

extern int besc;                /*  intrface.c, setup.c, sgame.c            */

extern int benter;              /*  entity.c, intrface.c, setup.c, sgame.c  */

extern int balt;                /*  selector.c, (eqp|item|mas|shop)menu.c,  */
                        /*  heroc.c, intrface.c, menu.c, setup.c,   */
                        /*  draw.c, sgame.c                         */

extern int bctrl;               /*  selector.c, (eqp|item|mas|shop)menu.c,  */
                        /*  sgame.c, heroc.c, intrface.c, setup.c,  */
                        /*  menu.c                                  */

extern int kright;              /*  setup.c, sgame.c  */
extern int kleft;               /*  setup.c, sgame.c  */
extern int kup;                 /*  setup.c, sgame.c  */
extern int kdown;               /*  setup.c, sgame.c  */
extern int kesc;                /*  setup.c, sgame.c  */
extern int kenter;              /*  setup.c, sgame.c  */
extern int kalt;                /*  setup.c, sgame.c  */
extern int kctrl;               /*  setup.c, sgame.c, entity.c  */
extern int jbalt;               /*  setup.c, sgame.c  */
extern int jbctrl;              /*  setup.c, sgame.c  */
extern int jbenter;             /*  setup.c, sgame.c  */
extern int jbesc;               /*  setup.c, sgame.c  */
extern int vx, vy, mx, my, steps, lastm[PSIZE];;

extern BITMAP *double_buffer;
extern BITMAP *map_icons[MAX_TILES];


extern BITMAP *back, *tc, *tc2, *bub[8], *b_shield, *b_shell, *b_repulse, *b_mp;
extern BITMAP *cframes[NUM_FIGHTERS][MAXCFRAMES],
   *tcframes[NUM_FIGHTERS][MAXCFRAMES], *frames[MAXCHRS][MAXFRAMES];
extern BITMAP *eframes[MAXE][MAXEFRAMES], *pgb[9], *sfonts[5], *bord[8];
extern BITMAP *menuptr, *mptr, *sptr, *stspics, *sicons, *bptr, *missbmp,
   *noway, *upptr, *dnptr;
extern BITMAP *shadow[MAX_SHADOWS];     /*  draw.c  */
extern unsigned short *map_seg;
extern unsigned short *b_seg, *f_seg;
extern unsigned char *progress, *z_seg, *s_seg, *o_seg, *treasure;
extern BITMAP *kfonts, *portrait[MAXCHRS];
extern s_map g_map;
extern s_entity g_ent[MAX_ENT + PSIZE];
extern s_anim tanim[6][MAX_ANIM];
extern s_anim adata[MAX_ANIM];
extern int noe, pidx[PSIZE], numchrs, gp, xofs, yofs, gsvol, gmvol;
extern unsigned char autoparty, autofollow, alldead, is_sound, deadeffect,
   vfollow, use_sstone;
extern unsigned char kq_version, hold_fade, cansave, skip_intro, wait_retrace,
   windowed, stretch_view;
extern unsigned short tilex[MAX_TILES], adelay[MAX_ANIM];
extern char *strbuf;
extern s_player party[MAXCHRS], players[MAXCHRS];
extern s_fighter fighter[NUM_FIGHTERS];
extern s_fighter tempa, tempd;
extern int noi, shin[12], dct;
extern char sname[39], ctext[39];
extern volatile int timer, ksec, kmin, khr, timer_count;
extern unsigned short lup[MAXCHRS][20];
extern COLOR_MAP cmap;
extern unsigned char can_run, display_desc;
extern unsigned char draw_background, draw_middle, draw_foreground, draw_shadow;
extern unsigned short g_inv[MAX_INV][2];
extern int view_x1, view_y1, view_x2, view_y2, view_on, in_combat;
extern int frate, mfrate, show_frate, use_joy, cheat_loaded;

extern int cheat;
extern int warx, wary;



#if 0
void my_counter (void);         /*  staticed  */
void data_dump (void);          /*  staticed  */
void startup (void);            /*  staticed  */
void load_data (void);          /*  staticed  */
void allocate_stuff (void);     /*  staticed  */
void deallocate_stuff (void);   /*  staticed  */
BITMAP *alloc_bmp (int, int, char *);   /*  staticed  */
void load_portraits (void);     /*  did not exist  */
#endif /*  0  */



#endif /*  __KQ_H  */
