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

#ifndef __SHOPMENU_H
#define __SHOPMENU_H



#define NUMSHOPS      50
#define SHOPITEMS     12

int shop (int);                 /*  only in intrface.c  */
void inn (char *, int, int);    /*  only in intrface.c  */



extern unsigned short shops[NUMSHOPS][SHOPITEMS][3];   /*  sgame.c  */
extern unsigned short shopq[NUMSHOPS][SHOPITEMS];      /*  sgame.c  */

#endif /*  __SHOPMENU_H  */
