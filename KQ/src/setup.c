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
 * \brief Setup and menu code
 *
 * \author JB
 * \date ??????
 * \remark Updated  ML Oct-2002
 */

#include <stdio.h>
#include <string.h>

#include "kq.h"
#include "setup.h"

#include "combat.h"
#include "draw.h"
#include "music.h"
#include "res.h"
#include "timing.h"
/*! \name Globals */
/*\{*/
/*! Debug level 0..3 */
char debugging = 0;
/*! Speed-up for slower machines */
char slow_computer = 0;
/*! Look up table of names for keys */
char *keynames[115] = {
   "",
   "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
   "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
   "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3",
   "4", "5", "6", "7", "8", "9", "0_PAD", "1_PAD", "2_PAD", "3_PAD",
   "4_PAD", "5_PAD", "6_PAD", "7_PAD", "8_PAD", "9_PAD", "F1", "F2", "F3",
   "F4",
   "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "ESC", "TILDE",
   "MINUS", "EQUALS", "BACKSPACE", "TAB", "OPENBRACE", "CLOSEBRACE", "ENTER",
   "COLON", "QUOTE", "BACKSLASH",
   "BACKSLASH2", "COMMA", "STOP", "SLASH", "SPACE", "INSERT", "DEL", "HOME",
   "END", "PGUP",
   "PGDN", "LEFT", "RIGHT", "UP", "DOWN", "SLASH_PAD", "ASTERISK",
   "MINUS_PAD", "PLUS_PAD", "DEL_PAD",
   "ENTER_PAD", "PRTSCR", "PAUSE", "ABNT_C1", "YEN", "KANA", "CONVERT",
   "NOCONVERT", "AT", "CIRCUMFLEX",
   "COLON2", "KANJI", "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "ALT",
   "ALTGR", "LWIN", "RWIN",
   "MENU", "SCRLOCK", "NUMLOCK", "CAPSLOCK"
};
/*\}*/


/*  internal variables  */
static DATAFILE *sfx[MAX_SAMPLES];



/*  internal functions  */
static int load_samples (void);
static int getavalue (char *, int, int, int, int);
static int getakey (void);



/*! \brief Parse setup.cfg
 *
 * Read settings from file
 * Parse the setup.cfg file for key configurations.
 * This file would also contain sound options, but that
 * isn't necessary right now.
 *
 * Remember that setup.cfg is found in the /saves dir!
*/
void parse_setup (void)
{
   FILE *s;
   int dab = 0;
   /* Default key assignments */
   kup = KEY_UP;
   kdown = KEY_DOWN;
   kright = KEY_RIGHT;
   kleft = KEY_LEFT;
   kalt = KEY_ALT;
   kctrl = KEY_LCONTROL;
   kenter = KEY_ENTER;
   kesc = KEY_ESC;
   jbalt = 0;
   jbctrl = 1;
   jbenter = 2;
   jbesc = 3;
   /* PH Why in the world doesn't he use Allegro cfg functions here? */
   if (!(s = fopen (kqres (SETTINGS_DIR, "setup.cfg"), "r")))
     {
        klog ("Could not open saves/setup.cfg - Using defaults.");
        return;
     }
   fscanf (s, "%s", strbuf);
   while (!feof (s))
     {
        if (strbuf[0] == '#')
           fgets (strbuf, 254, s);
#ifdef KQ_CHEATS
        if (!strcmp (strbuf, "cheat"))
          {
             fscanf (s, "%d", &dab);
             cheat = dab;
          }
#endif
        if (!strcmp (strbuf, "debug"))
          {
             fscanf (s, "%d", &dab);
             debugging = dab;
          }
        if (!strcmp (strbuf, "intro"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "no"))
                skip_intro = 1;
          }
        if (!strcmp (strbuf, "windowed"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "yes"))
                windowed = 1;
          }
        if (!strcmp (strbuf, "stretch"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "yes"))
                stretch_view = 1;
          }
        if (!strcmp (strbuf, "framerate"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "on"))
                show_frate = 1;
          }
        if (!strcmp (strbuf, "sound"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "off"))
                is_sound = 0;
          }
        if (!strcmp (strbuf, "joystick"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "no"))
                use_joy = 0;
          }
        if (!strcmp (strbuf, "slow_computer"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "yes"))
                slow_computer = 1;
          }
        if (!strcmp (strbuf, "rightkey"))
          {
             fscanf (s, "%s", strbuf);
             kright = atoi (strbuf);
          }
        if (!strcmp (strbuf, "leftkey"))
          {
             fscanf (s, "%s", strbuf);
             kleft = atoi (strbuf);
          }
        if (!strcmp (strbuf, "upkey"))
          {
             fscanf (s, "%s", strbuf);
             kup = atoi (strbuf);
          }
        if (!strcmp (strbuf, "downkey"))
          {
             fscanf (s, "%s", strbuf);
             /* TT: huh?! */
             // kup = atoi (strbuf);
             kdown = atoi (strbuf);
          }
        if (!strcmp (strbuf, "sysmenukey"))
          {
             fscanf (s, "%s", strbuf);
             kesc = atoi (strbuf);
          }
        if (!strcmp (strbuf, "cancelkey"))
          {
             fscanf (s, "%s", strbuf);
             kctrl = atoi (strbuf);
          }
        if (!strcmp (strbuf, "confirmkey"))
          {
             fscanf (s, "%s", strbuf);
             kalt = atoi (strbuf);
          }
        if (!strcmp (strbuf, "chrmenukey"))
          {
             fscanf (s, "%s", strbuf);
             kenter = atoi (strbuf);
          }
        fscanf (s, "%s", strbuf);
     }
   fclose (s);
}


/*! \brief Draw a setting and its title
 *
 * Helper function for the config menu.
 * The setting title is drawn, then its value right-aligned.
 * \author PH
 * \date 20030527
 * \param y y-coord of line
 * \param caption title of the setting (e.g. "Windowed mode:")
 * \param value the setting (e.g. "Yes")
 */
static void citem (int y, char *caption, char *value)
{
   print_font (double_buffer, 48 + xofs, y + yofs, caption, FNORMAL);
   print_font (double_buffer, 280 - 8 * strlen (value) + xofs, y + yofs, value,
               FNORMAL);
}


/*! \brief Display configuration menu
 *
 * This is the config menu that is called from the system
 * menu.  Here you can adjust the music or sound volume, or
 * the speed that the battle gauge moves at.
*/
void config_menu (void)
{
   int stop = 0, ptr = 0, rd = 1, p;
   int temp_key = 0;
   /* helper strings */
   char *dc[16] = {
      "Display KQ in a window.",
      "Stretch to fit 640x480 resolution.",
      "Display the frame rate during play.",
      "Wait for vertical retrace.",
      "Key used to move up.",
      "Key used to move down.",
      "Key used to move left.",
      "Key used to move right.",
      "Key used to call system menu.",
      "Key used to cancel action.",
      "Key used to confirm action.",
      "Key used to call character menu.",
      "Toggle sound and music on/off.",
      "Overall sound volume (affects music).",
      "Music volume.",
      "Animation speed-ups for slow machines."
   };

   unpress ();
   while (!stop)
     {
        if (rd)
          {
             drawmap ();
             menubox (double_buffer, 88 + xofs, yofs, 16, 1, BLUE);
             print_font (double_buffer, 96 + xofs, 8 + yofs,
                         "KQ Configuration", FGOLD);
             menubox (double_buffer, 32 + xofs, 24 + yofs, 30, 19, BLUE);
             citem (32, "Windowed mode:", windowed == 1 ? "YES" : "NO");
             citem (40, "Stretch Display:", stretch_view == 1 ? "YES" : "NO");
             citem (48, "Show Frame Rate:", show_frate == 1 ? "YES" : "NO");
             citem (56, "Wait for Retrace:", wait_retrace == 1 ? "YES" : "NO");
             citem (72, "Up Key:", keynames[kup]);
             citem (80, "Down Key:", keynames[kdown]);
             citem (88, "Left Key:", keynames[kleft]);
             citem (96, "Right Key:", keynames[kright]);
             citem (104, "Confirm Key:", keynames[kalt]);
             citem (112, "Cancel Key:", keynames[kctrl]);
             citem (120, "Menu Key:", keynames[kenter]);
             citem (128, "System Menu Key:", keynames[kesc]);
             citem (144, "Sound System:", is_sound ? "ON" : "OFF");

             if (is_sound == 2)
               {
                  sprintf (strbuf, "%3d%%", gsvol * 100 / 250);
                  citem (152, "Sound Volume:", strbuf);

                  sprintf (strbuf, "%3d%%", gmvol * 100 / 250);
                  citem (160, "Music Volume:", strbuf);
               }

             /* TT: This needs to check for ==0 because 1 means sound init */
             if (is_sound == 0)
               {
                  print_font (double_buffer, 48 + xofs, 152 + yofs,
                              "Sound Volume:", FDARK);
                  print_font (double_buffer, 48 + xofs, 160 + yofs,
                              "Music Volume:", FDARK);
               }

             citem (176, "Slow Computer:", slow_computer ? "YES" : "NO");

             /* this affects the VISUAL placement of the arrow */
             p = ptr;
             if (ptr > 3)
                p++;
             if (ptr > 11)
                p++;
             if (ptr > 14)
                p++;
             draw_sprite (double_buffer, menuptr, 32 + xofs, p * 8 + 32 + yofs);
             /* This is the bottom window, where the description goes */
             menubox (double_buffer, xofs, 216 + yofs, 38, 1, BLUE);
             print_font (double_buffer, 8 + xofs, 224 + yofs, dc[ptr], FNORMAL);
             blit2screen (xofs, yofs);
          }
        readcontrols ();
        if (up)
          {
             unpress ();
             // "jump" over unusable options
             if (ptr == 15 && is_sound == 0)
                ptr -= 2;
             ptr--;
             if (ptr < 0)
                ptr = 15;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (down)
          {
             unpress ();
             // "jump" over unusable options
             if (ptr == 12 && is_sound == 0)
                ptr += 2;
             ptr++;
             if (ptr > 15)
                ptr = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             switch (ptr)
               {
               case 0:
                  text_ex (B_TEXT, 255,
                           "Changing the display mode to or from windowed view could have serious ramifications. It is advised that you save first.");
                  if (windowed == 0)
                     sprintf (strbuf, "Switch to windowed mode?");
                  else
                     sprintf (strbuf, "Switch to full screen?");
                  p = prompt (255, 2, B_TEXT, strbuf, "  no", "  yes", "");
                  if (p == 1)
                    {
                       if (windowed == 0)
                          windowed = 1;
                       else
                          windowed = 0;
                       set_graphics_mode ();
                    }
                  break;
               case 1:
                  text_ex (B_TEXT, 255,
                           "Changing the stretched view option could have serious ramifications. It is advised that you save your game before trying this.");
                  if (stretch_view == 0)
                     sprintf (strbuf, "Try to stretch the display?");
                  else
                     sprintf (strbuf, "Switch to unstretched display?");
                  p = prompt (255, 2, B_TEXT, strbuf, "  no", "  yes", "");
                  if (p == 1)
                    {
                       if (stretch_view == 0)
                          stretch_view = 1;
                       else
                          stretch_view = 0;
                       set_graphics_mode ();
                    }
                  break;
               case 2:
                  if (show_frate == 0)
                     show_frate = 1;
                  else
                     show_frate = 0;
                  break;
               case 3:
                  if (wait_retrace == 0)
                     wait_retrace = 1;
                  else
                     wait_retrace = 0;
                  break;
               case 4:
                  while ((temp_key = getakey ()) == 0);
                  kup = temp_key;
                  unpress ();
                  temp_key = 0;
                  break;
               case 5:
                  while ((temp_key = getakey ()) == 0);
                  kdown = temp_key;
                  unpress ();
                  temp_key = 0;
                  break;
               case 6:
                  while ((temp_key = getakey ()) == 0);
                  kleft = temp_key;
                  unpress ();
                  temp_key = 0;
                  break;
               case 7:
                  while ((temp_key = getakey ()) == 0);
                  kright = temp_key;
                  unpress ();
                  temp_key = 0;
                  break;
               case 8:
                  while ((temp_key = getakey ()) == 0);
                  kalt = temp_key;
                  unpress ();
                  temp_key = 0;
                  break;
               case 9:
                  while ((temp_key = getakey ()) == 0);
                  kctrl = temp_key;
                  unpress ();
                  temp_key = 0;
                  break;
               case 10:
                  while ((temp_key = getakey ()) == 0);
                  kenter = temp_key;
                  unpress ();
                  temp_key = 0;
                  break;
               case 11:
                  while ((temp_key = getakey ()) == 0);
                  kesc = temp_key;
                  unpress ();
                  temp_key = 0;
                  break;
               case 12:
                  if (is_sound == 2)
                     sound_init ();
                  else
                    {
                       if (is_sound == 0)
                         {
                            is_sound = 1;
                            print_font (double_buffer, 92 + 2 + xofs,
                                        204 + yofs, "...please wait...",
                                        FNORMAL);
                            blit2screen (xofs, yofs);
                            sound_init ();
                            play_music (g_map.song_file, 0);
                         }
                    }
                  break;
               case 13:
                  if (is_sound == 2)
                    {
                       p = getavalue ("Sound Volume", 0, 25, gsvol / 10, 1);
                       if (p != -1)
                          gsvol = p * 10;

                       /* make sure to set it no matter what */
                       set_volume (gsvol, 0);
                    }
                  else
                     play_effect (SND_BAD, 128);
                  break;
               case 14:
                  if (is_sound == 2)
                    {
                       p = getavalue ("Music Volume", 0, 25, gmvol / 10, 1);
                       if (p != -1)
                          gmvol = p * 10;

                       /* make sure to set it no matter what */
                       set_music_volume (gmvol / 250.0);
                    }
                  else
                     play_effect (SND_BAD, 128);
                  break;
               case 15:
                  /* TT: toggle slow_computer */
                  slow_computer = (slow_computer == 0 ? 1 : 0);
                  break;
               }
             rd = 1;
          }
        if (bctrl)
          {
             unpress ();
             stop = 1;
          }
     }
}



/*! \brief Process keypresses when mapping new keys
 *
 * This grabs whatever key is being pressed and returns it to the caller.
 * PH 20030527 Removed call to keypressed() and added poll_music()
 *
 * \returns the key being pressed, 0 if error (or cancel?)
*/
static int getakey (void)
{
   int a;

   clear_keybuf ();
   menubox (double_buffer, 108 + xofs, 108 + yofs, 11, 1, DARKBLUE);
   print_font (double_buffer, 116 + xofs, 116 + yofs, "Press a key", FNORMAL);
   blit2screen (xofs, yofs);

   while (1)
     {
        poll_music ();
        for (a = 0; a < KEY_MAX; a++)
           if (key[a] != 0)
              return a;
     }
   return 0;
}



/*! \brief Play sound effects / music if adjusting it */
/* TT: looks like a hack to me! :-) */
/* PH: I cannot tell a lie: it was Matthew... */
#define IF_VOLUME_ALERT() \
   if (!strcmp (capt, "Sound Volume")){\
   set_volume (cv * 10, 0);\
   play_effect (1, 127);\
}\
else if (!strcmp (capt, "Music Volume"))\
set_music_volume (cv / 25.5);



/*! \brief Get value for option
 *
 * Display a bar and allow the user to adjust between fixed limits
 *
 * \param   capt Caption
 * \param   minu Minimum value of option
 * \param   maxu Maximum vlaue of option
 * \param   cv Current value (initial value)
 * \param   sp Show percent. If sp==1, show as a percentage of maxu
 * \returns the new value for option, or -1 if cancelled.
*/
static int getavalue (char *capt, int minu, int maxu, int cv, int sp)
{
   int stop = 0, a, b, rd = 1;

   if (maxu == 0)
      return -1;
   while (!stop)
     {
        if (rd == 1)
          {
             menubox (double_buffer, 148 - (maxu * 4) + xofs, 100 + yofs,
                      maxu + 1, 3, DARKBLUE);
             print_font (double_buffer, 160 - (strlen (capt) * 4) + xofs,
                         108 + yofs, capt, FGOLD);
             print_font (double_buffer, 152 - (maxu * 4) + xofs, 116 + yofs,
                         "<", FNORMAL);
             print_font (double_buffer, 160 + (maxu * 4) + xofs, 116 + yofs,
                         ">", FNORMAL);
             b = 160 - (maxu * 4) + xofs;
             for (a = 0; a < cv; a++)
               {
                  rectfill (double_buffer, a * 8 + b + 1, 117 + yofs,
                            a * 8 + b + 7, 123 + yofs, 50);
                  rectfill (double_buffer, a * 8 + b, 116 + yofs,
                            a * 8 + b + 6, 122 + yofs, 21);
               }
             if (sp == 1)
                sprintf (strbuf, "%d%%", cv * 100 / maxu);
             else
                sprintf (strbuf, "%d", cv);
             print_font (double_buffer, 160 - (strlen (strbuf) * 4) + xofs,
                         124 + yofs, strbuf, FGOLD);
             blit2screen (xofs, yofs);
          }
        readcontrols ();
        if (left)
          {
             unpress ();
             cv--;
             if (cv < minu)
                cv = minu;
             rd = 1;
             IF_VOLUME_ALERT ();
          }
        if (right)
          {
             unpress ();
             cv++;
             if (cv > maxu)
                cv = maxu;
             rd = 1;
             IF_VOLUME_ALERT ();
          }
        if (balt)
          {
             unpress ();
             stop = 1;
          }
        if (bctrl)
          {
             unpress ();
             return -1;
          }
     }
   return cv;
}


/*! \brief Show keys help
 * Show a screen with the keys listed, and other helpful info
 * \author PH
 * \date 20030527
 */
void show_help (void)
{
   menubox (double_buffer, 116 + xofs, yofs, 9, 1, BLUE);
   print_font (double_buffer, 132 + xofs, 8 + yofs, "KQ Help", FGOLD);
   menubox (double_buffer, 32 + xofs, 24 + yofs, 30, 20, BLUE);
   menubox (double_buffer, xofs, 216 + yofs, 38, 1, BLUE);
   print_font (double_buffer, 16 + xofs, 224 + yofs,
               "Press CONFIRM to exit this screen", FNORMAL);
   citem (72, "Up Key:", keynames[kup]);
   citem (80, "Down Key:", keynames[kdown]);
   citem (88, "Left Key:", keynames[kleft]);
   citem (96, "Right Key:", keynames[kright]);
   citem (104, "Confirm Key:", keynames[kalt]);
   citem (112, "Cancel Key:", keynames[kctrl]);
   citem (120, "Menu Key:", keynames[kenter]);
   citem (128, "System Menu Key:", keynames[kesc]);
   do
     {
        blit2screen (xofs, yofs);
        readcontrols ();
     }
   while (!balt);
   unpress ();
}


/*! \brief Set mode
 *
 * Set the graphics mode, taking into account the Windowed and Stretched
 * settings
*/
void set_graphics_mode (void)
{
   if (stretch_view == 1)
     {
        if (windowed == 1)
           set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
        else
           set_gfx_mode (GFX_AUTODETECT, 640, 480, 0, 0);
     }
   else
     {
        if (windowed == 1)
           set_gfx_mode (GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);
        else
           set_gfx_mode (GFX_AUTODETECT, 320, 240, 0, 0);
     }

   set_palette (pal);
}



/*! \brief Initialize sound system
 * \author JB
 * \date ????????
 * \remark On entry is_sound=1 to initialise,
 *         on exit is_sound=0 (failure) or 2 (success),
 *         is_sound=2 to shutdown,
 *         on exit is_sound=0
 * \remark 20020914 - 05:28 RB : Updated
 *  20020922 - ML : updated to use DUMB
 *  20020922 - ML : Changed to only reserving 8 voices. (32 seemed over-kill?)
*/
void sound_init (void)
{
   if (is_sound == 1)
     {
        if (install_sound (DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) == -1)

          {
             is_sound = 0;
          }
        else
          {

             reserve_voices (8, 0);
             set_volume_per_voice (2);
             init_music ();
             is_sound = load_samples ()? 0 : 2; /* load the wav files */
          }
     }
   else if (is_sound == 2)
     {
        shutdown_music ();
        free_samples ();
        remove_sound ();
        is_sound = 0;
     }
}



/*! \brief Load sample files
 * \author JB
 * \date ????????
 *
 * Load the list of samples from the data file.
 *
 * \todo RB FIXME: This must be generated from the kqsnd.h header,
 *          not hardcoded, to make it easier to maintain (a perl script?).
 * \remark Updated : 20020914 - 05:20 (RB)
 * \remark ML 2002-09-22: altered this so it returns an error on failure
 *
 * \returns 0 on success, 1 on failure.
*/
static int load_samples (void)
{
   AL_CONST char *sndfiles[MAX_SAMPLES] = {
      "WHOOSH_WAV", "MENUMOVE_WAV", "BAD_WAV", "ITEM_WAV",
      "EQUIP_WAV", "DEEQUIP_WAV", "BUYSELL_WAV", "TWINKLE_WAV",
      "SCORCH_WAV", "POISON_WAV", "CHOP_WAV", "SLASH_WAV",
      "STAB_WAV", "HIT_WAV", "ICE_WAV", "WIND_WAV",
      "QUAKE_WAV", "BLACK_WAV", "WHITE_WAV", "BOLT1_WAV",
      "FLOOD_WAV", "HURT_WAV", "BMAGIC_WAV", "SHIELD_WAV",
      "KILL_WAV", "DOOROPEN_WAV", "DOOR2_WAV", "STAIRS_WAV",
      "TELEPORT_WAV", "CURE_WAV", "RECOVER_WAV", "ARROW_WAV",
      "BOLT2_WAV", "BOLT3_WAV", "FLAME_WAV", "BLIND_WAV",
      "INN_WAV", "CONFUSE_WAV", "DISPEL_WAV", "DOOM_WAV",
      "DRAIN_WAV", "GAS_WAV", "EXPLODE_WAV"
   };
   int index;

   if (is_sound == 0)
      return 1;

   for (index = 0; index < MAX_SAMPLES; index++)
     {
        sfx[index] = load_datafile_object (SOUND_DATAFILE, sndfiles[index]);
        if (sfx[index] == NULL)
          {
             sprintf (strbuf, "Error loading .WAV file: %s.\n",
                      sndfiles[index]);
             klog (strbuf);
             return 1;
          }
     }
   return 0;
}



/*! \brief Release memory used by samples
 * \author  : Josh Bolduc
 * \date ????????
 *
 *  Duh.
*/
void free_samples (void)
{
   int index;

   if (is_sound == 0)
      return;

   for (index = 0; index < MAX_SAMPLES; index++)
      unload_datafile_object (sfx[index]);
}



/*! \brief Play sample effect
 *
 * Play an effect... if possible/necessary.  If the effect to
 * be played is the 'bad-move' effect, than do something visually
 * so that even if sound is off you know you did something bad :)
 * PH added explode effect.
 *
 * \param   efc Effect to play (index in sfx[])
 * \param   panning Left/right pan - see Allegro's play_sample()
*/
void play_effect (int efc, int panning)
{
   int a, s, xo = 1, yo = 1;
   static const int bx[8] = { -1, 0, 1, 0, -1, 0, 1, 0 };
   static const int by[8] = { -1, 0, 1, 0, 1, 0, -1, 0 };
   static const int sc[] = { 1, 2, 3, 5, 3, 3, 3, 2, 1 };
   SAMPLE *samp;
   PALETTE whiteout, old;

   /* Patch provided by mattrope: */
   /* sfx array is empty if sound is not initialized */
   if (is_sound != 0)
      samp = (SAMPLE *) sfx[efc]->dat;
   else
      samp = NULL;              /* PH not strictly needed but I added it */

   switch (efc)
     {
     default:
        if (is_sound != 0)
           play_sample (samp, gsvol, panning, 1000, 0);
        break;
     case SND_BAD:
        blit (double_buffer, fx_buffer, 0, 0, 0, 0, 352, 280);

        if (is_sound != 0)
           play_sample (samp, gsvol, panning, 1000, 0);
        clear_bitmap (double_buffer);
        blit (fx_buffer, double_buffer, xofs, yofs, xofs, yofs, 320, 240);

        if (in_combat == 0)
          {
             xo = xofs;
             yo = yofs;
          }

        /*        blit (fx_buffer, double_buffer, xo, yo, xo, yo, 320, 240); */

        for (a = 0; a < 8; a++)
          {
             blit2screen (xo + bx[a], yo + by[a]);
             wait (10);
          }
        blit (fx_buffer, double_buffer, 0, 0, 0, 0, 352, 280);
        break;
     case SND_EXPLODE:
        blit (double_buffer, fx_buffer, 0, 0, 0, 0, 352, 280);
        clear_bitmap (double_buffer);
        get_palette (old);
        for (a = 0; a < 256; ++a)
          {
             s = (old[a].r + old[a].g + old[a].b) > 40 ? 0 : 63;
             whiteout[a].r = whiteout[a].g = whiteout[a].b = s;
          }
        blit (fx_buffer, double_buffer, xofs, yofs, xofs, yofs, 320, 240);
        if (is_sound)
          {
             play_sample (samp, gsvol, panning, 1000, 0);
          }
        for (s = 0; s < (int) (sizeof (sc) / sizeof (*sc)); ++s)
          {
             if (s == 1)
                set_palette (whiteout);
             if (s == 6)
                set_palette (old);

             for (a = 0; a < 8; a++)
               {
                  blit2screen (xofs + bx[a] * sc[s], yofs + by[a] * sc[s]);
                  wait (10);
               }
          }
        blit (fx_buffer, double_buffer, 0, 0, 0, 0, 352, 280);
        break;
     }
}
