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

#ifndef __DRAW_H
#define __DRAW_H

#include "allegro.h"

#define GREY1      4
#define GREY2      8
#define GREY3     13
#define WHITE     15
#define DBLUE      3
#define DRED       6
#define FNORMAL    0
#define FRED       1
#define FYELLOW    2
#define FGREEN     3
#define FDARK      4
#define FGOLD      5
#define FBIG       6
#define FDECIDE    7
#define B_TEXT     0
#define B_THOUGHT  1

void blit2screen (int, int);
void color_scale (BITMAP *, BITMAP *, int, int);
void convert_cframes (int, int, int, int);
void revert_cframes (int, int);
void draw_icon (BITMAP *, int, int, int);
void draw_stsicon (BITMAP *, int, int, int, int, int);
void drawchar (int, int);
int is_forestsquare (int, int);
void drawmap (void);
void draw_backlayer (void);
void draw_midlayer (void);
void draw_forelayer (void);
void draw_shadows (void);
void border (BITMAP *, int, int, int, int);
void menubox (BITMAP *, int, int, int, int, int);
void print_font (BITMAP *, int, int, char *, int);
void print_num (BITMAP *, int, int, char *, int);
void set_textpos (int);
void draw_textbox (int);
void thought_text (int, char *, char *, char *, char *);
void bubble_text (int, char *, char *, char *, char *);
void generic_text (int, int);
char *parse_string (char *);
int prompt (int, int, int, char *, char *, char *, char *);
void message (char *, int, int, int, int);
void set_view (int, int, int, int, int);

extern unsigned char BLUE, DARKBLUE, DARKRED;

#endif
