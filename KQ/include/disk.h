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

#ifndef __DISK_H
#define __DISK_H
#include "kq.h"
#include "entity.h"
int load_s_map (s_map * sm, PACKFILE * f);
int save_s_map (s_map * sm, PACKFILE * f);
int load_s_entity (s_entity * s, PACKFILE * f);
int save_s_entity (s_entity * s, PACKFILE * f);
int load_s_player (s_player * s, PACKFILE * f);
int save_s_player (s_player * s, PACKFILE * f);
#endif/*__DISK_H*/
