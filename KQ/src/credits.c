#include <string.h>
#include "kq.h"
#include "draw.h"
#include "credits.h"
/*! file
 * Stuff relating to credits display:
 * The original authors, new team etc are name-checked
 * \author PH
 * \date 20030526
 */

/*! Array of strings */
static char *credits[] = {
   "(C) 2001 DoubleEdge Software",
   "(C) 2002,3 KQ Lives Team",
   "http://kqlives.sourceforge.net/",
   "Peter Hull",
   "TeamTerradactyl",
   "Chris Barry",
   "Eduardo Dudaskank",
   "Troy D Patterson",
   "Master Goodbytes",
   "Rey Brujo",
   "Matthew Leverton",
   "Sam Hocevar",
   "Steven Fullmer",
   NULL
};
static char pressf1[] = "Press F1 for help";

static char **cc = NULL;
static short int etab[32];
static BITMAP *wk = NULL;

static volatile int ticks = 0;
static void ticker (void)
{
   ticks++;
}

END_OF_FUNCTION (ticker);

/*! \brief An S-shaped curve
 * 
 * Returns values from an 'ease' curve,
 * generally =3*x^2-2*x^3
 * 
 * Here modified to return a value 0..32
 * \param x Where to evaluate the function
 * \returns 0 if x<0, 32 if x>=32, otherwise a number between 0 and 32
 */
static int ease (int x)
{
   if (x <= 0)
      return 0;
   else if (x >= 32)
      return 32;
   else
      return etab[x];
}

void allocate_credits (void)
{
   unsigned int tlen = 0;
   int i;
   for (cc = credits; *cc; ++cc)
     {
        if (strlen (*cc) > tlen)
           tlen = strlen (*cc);
     }
   wk = create_bitmap (8 * tlen, 64);
   for (i = 0; i < 32; ++i)
      etab[i] = i * i * (3 * 32 - 2 * i) / 32 / 32;
   cc = credits;
   LOCK_FUNCTION (ticker);
   LOCK_VARIABLE (ticks);
   install_int_ex (ticker, BPS_TO_TIMER (60));
}
void deallocate_credits (void)
{
   destroy_bitmap (wk);
   wk = NULL;
   remove_int (ticker);
}
void display_credits (void)
{
   int i, x0, e;
   static int last_e = 999;
   if (wk == NULL)
     {
        allocate_credits ();
     }
   if (ticks > 640)
     {
        clear_bitmap (wk);
        print_font (wk, (wk->w - 8 * strlen (*cc)) / 2, 42, *cc, FNORMAL);
        if (*(++cc) == NULL)
           cc = credits;
        print_font (wk, (wk->w - 8 * strlen (*cc)) / 2, 10, *cc, FNORMAL);
        print_font (double_buffer, (320 - 8 * strlen (pressf1)) / 2, 218,
                    pressf1, FNORMAL);
        ticks = 0;
     }
   e = 320 - ticks;
   if (e != last_e)
     {
        x0 = (320 - wk->w) / 2;
        for (i = 0; i < wk->w; ++i)
          {
             blit (wk, double_buffer, i, ease (i + e), i + x0, 185, 1, 32);
          }
        last_e = e;
     }
}
