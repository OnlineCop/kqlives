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
/** \file
 * \brief Constants that are included in the C and Lua scripts 
 */
#ifndef __PROGRESS_H
#define __PROGRESS_H

/*! \name Progress indicators */
/* PLEASE ADD TO THIS LIST IN ORDER!!! */
/*\{*/

#define P_START           0
#define P_ODDWALL         1
#define P_DARKIMPBOSS     2
#define P_DYINGDUDE       3
#define P_BUYCURE         4
#define P_GETPARTNER      5
#define P_PARTNER1        6
#define P_PARTNER2        7
#define P_SHOWBRIDGE      8
#define P_TALKDERIG       9
#define P_FIGHTONBRIDGE   10
#define P_FELLINPIT       11
#define P_EKLAWELCOME     12
#define P_LOSERONBRIDGE   13
#define P_ASLEEPONBRIDGE  14
#define P_ALTARSWITCH     15
#define P_KILLBLORD       16
#define P_GOBLINITEM      17
#define P_ORACLE          18
#define P_FTOTAL          19
#define P_FLOOR1          20
#define P_FLOOR2          21
#define P_FLOOR3          22
#define P_FLOOR4          23
#define P_WSTONES         24
#define P_BSTONES         25
#define P_WALL1           26
#define P_WALL2           27
#define P_WALL3           28
#define P_WALL4           29
#define P_DOOROPEN        30
#define P_DOOROPEN2       31
#define P_TOWEROPEN       32
#define P_DRAGONDOWN      33
#define P_TREASUREROOM    34
#define P_UNDEADJEWEL     35
#define P_UCOIN           36
#define P_CANCELROD       37
#define P_PORTALGONE      38
#define P_WARPEDTOT4      39
#define P_OLDPARTNER      40
#define P_BOUGHTHOUSE     41
#define P_TALKGELIK       42
#define P_OPALHELMET      43
#define P_FOUNDMAYOR      44
#define P_PARTNER3        45
#define P_EMBERSKEY       46
#define P_FOUGHTGUILD     47
#define P_GUILDSECRET     48
#define P_SEECOLISEUM     49
#define P_OPALSHIELD      50
#define P_STONE1          51
#define P_STONE2          52
#define P_STONE3          53
#define P_STONE4          54
#define P_DENORIAN        55
#define P_C4DOORSOPEN     56
#define P_DEMNASDEAD      57
#define P_FIRSTTIME       58
#define P_ROUNDNUM        59
#define P_BATTLESTATUS    60
#define P_USEITEMINCOMBAT 61
#define P_FINALPARTNER    62
#define P_TALKGRAMPA      63
#define P_SAVEBREANNE     64
#define P_PASSGUARDS      65
#define P_IRONKEY         66
#define P_AVATARDEAD      67
#define P_GIANTDEAD       68
#define P_OPALBAND        69
#define P_BRONZEKEY       70
#define P_CAVEKEY         71
#define P_TOWN6INN        72
#define P_WARPSTONE       73
#define P_DOINTRO         74
#define P_GOTOFORT        75
#define P_GOTOESTATE      76
#define P_TALKBUTLER      77
#define P_PASSDOOR1       78
#define P_PASSDOOR2       79
#define P_PASSDOOR3       80

/* New indicators */
#define P_BOMB1           81
#define P_BOMB2           82
#define P_BOMB3           83
#define P_BOMB4           84
#define P_BOMB5           85
#define P_DYNAMITE        86
#define P_TALKRUFUS       87
#define P_EARLYPROGRESS   88
#define P_OPALDRAGONOUT   89
#define P_OPALARMOUR      90

/* These are to store who's waiting in the manor */
#define P_MANORPARTY      91
#define P_MANORPARTY1     92
#define P_MANORPARTY2     93
#define P_MANORPARTY3     94
#define P_MANORPARTY4     95
#define P_MANORPARTY5     96
#define P_MANORPARTY6     97
#define P_MANORPARTY7     98

#define P_MANOR           99
#define P_PLAYERS         100
#define P_TALK_AJATHAR    101
#define P_BLADE           102
#define P_AYLA_QUEST	  103

/* 'specials' */
#define P_SHOPSTART       1750
#define P_SKIPINTRO       1997
#define P_REPULSE         1998
#define P_TEMP            1999
/*\}*/

/*! \name Special identifiers for bubble() */
/*\{*/
#define HERO1             200
#define HERO2             201
/*\}*/

/*!\name Hero identifiers*/
/*\{*/
#define SENSAR            0
#define SARINA            1
#define CORIN             2
#define AJATHAR           3
#define CASANDRA          4
#define TEMMIN            5
#define AYLA              6
#define NOSLOM            7
/*\}*/

#endif /*  __PROGRESS_H  */
