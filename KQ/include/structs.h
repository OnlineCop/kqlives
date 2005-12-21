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
 * \brief Structures common to mapedit and the game (s_map and s_entity)
 * \author PH
 * \date 20030805
 */

#ifndef __STRUCTS_H
#define __STRUCTS_H

/*! \brief Position marker
 *
 * A marker is a named position on the map.
 * These are set up in the mapeditor and accessed via scripts.
 * They are to make it easier to pinpoint locations in the maps,
 * for example, the locations that doors lead to.
 * \author PH
 * \date 20050126
 */


typedef struct
{
   char name[32];               /*!< The name of this marker */
   short x;                     /*! <The X position it refers to */
   short y;                     /*! <The Y position it refers to */
} s_marker;


/*! \brief Map definition 
 *
 * This is the structure of each map on disk
 * \note 20050126 PH add extensions for markers (rev1 map)
 */
typedef struct
{
   signed char map_no;          /*!< Map identifier */
   unsigned char zero_zone;     /*!< Non-zero if zone 0 triggers an event */
   unsigned char map_mode;      /*!< Map's parallax mode (see draw_map()) */
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
   int revision;                /*!< Internal revision number for the map file */
   int extra_sdword2;           /*!< Not used */
   char song_file[16];          /*!< Base file name for map song */
   char map_desc[40];           /*!< Map name (shown when map first appears) */
   short num_markers;           /*!< How many markers defined on the map */
   s_marker *markers;           /*!< Pointer to the array markers */
}
s_map;


/*! \brief Entity
 *
 * Contains info on an entities appearance,  position  and behaviour */
typedef struct
{
   unsigned char chrx;          //!< Entity's identity (what s/he looks like)
   unsigned short x;            //!< x-coord on map
   unsigned short y;            //!< y-coord on map
   unsigned short tilex;        //!< x-coord tile that entity is standing on
   unsigned short tiley;        //!< y-coord tile that entity is standing on
   unsigned char eid;           //!< Entity type (fighter, enemy, normal)
   unsigned char active;        //!< "Alive" or not
   unsigned char facing;        //!< Direction
   unsigned char moving;        //!< In the middle of a move
   unsigned char movcnt;        //!< How far along the move entity is
   unsigned char framectr;      //!< Counter for determining animation frame
   unsigned char movemode;      //!< Stand, wander, script or chasing
   unsigned char obsmode;       //!< Determine if affected by obstacles or not
   unsigned char delay;         //!< Movement delay (between steps)
   unsigned char delayctr;      //!< Counter for movement delay
   unsigned char speed;         //!< How hyperactive the entity is
   unsigned char scount;
   unsigned char cmd;           //!< Move/Wait/Facing command
   unsigned char sidx;          //!< Script ID number
   unsigned char extra;
   unsigned char chasing;       //!< Entity is following another
   int cmdnum;                  //!< Number of times we need to repeat 'cmd'
   unsigned char atype;
   unsigned char snapback;      //!< Snaps back to direction previously facing
   unsigned char facehero;      //!< Look at player when talked to
   unsigned char transl;        //!< Entity is see-through or not
   char script[60];             //!< Movement/action script (pacing, etc.)
   unsigned short target_x;     //!< x-coord the ent is moving to
   unsigned short target_y;     //!< y-coord the ent is moving to
}
s_entity;


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


/* PH eek! hack alert! */
#ifndef MAX_ANIM
#define MAX_ANIM 5
#endif


/*! \brief Tileset definition 
 *
 * This encapulates a tile set: graphics and animation.
 * \author PH
 * \date 20031222
 */
typedef struct
{
   char icon_set[16];
   s_anim tanim[MAX_ANIM];
} s_tileset;

#endif /*__STRUCTS_H*/
