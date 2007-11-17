#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)
AC_INIT(KQ, 0.99, [peterhull90@users.sourceforge.net])
AC_CONFIG_SRCDIR([src/kq.c])
AC_CONFIG_HEADER([config.h])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.16.1)

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AM_PROG_OBJC

# Check for allegro
AC_CHECK_PROG([HAVE_ALLEGRO], [allegro-config], [yes])
if test "${HAVE_ALLEGRO}" == "yes" ; then
  ALLEGRO_LIBS=`allegro-config --libs`
else
  # there is no allegro-config under windows
  AC_CHECK_LIB(alleg, create_bitmap, [HAVE_ALLEGRO=yes])
  ALLEGRO_LIBS='-lalleg'
fi
if test "${HAVE_ALLEGRO}" != "yes" ; then
  AC_MSG_ERROR([Allegro not found])
fi
AC_SUBST(ALLEGRO_LIBS)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h sys/select.h])
AC_CHECK_HEADERS(
  [lua50/lua.h], , [AC_CHECK_HEADERS(
    [lua5.1/lua.h], , [AC_CHECK_HEADERS(
      [lua.h], , [AC_MSG_ERROR(
        [Lua not found]
      )]
    )]
  )]
)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE

WARNING_CFLAGS="-Wall -W -Wno-unused-parameter -Wshadow -Wmissing-prototypes \
-Wredundant-decls -Wendif-labels -Wpointer-arith \
-Wcast-qual -Wcast-align -Wwrite-strings"
WARNING_CFLAGS="-Wall -Wextra -Wfloat-equal -Wredundant-decls \
-Wmissing-noreturn -Wno-shadow -Wendif-labels -Wlarger-than-65500 \
-Wpointer-arith -Wno-cast-qual -Wcast-align -Wwrite-strings \
-Winline -Wformat=2 -Winit-self -Wno-switch-default \
-Wno-switch-enum -Wno-unused-parameter -Wundef -Wno-error \
-Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes \
-Wold-style-definition -Wdeclaration-after-statement \
-Wbad-function-cast"
kq_save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $WARNING_CFLAGS"
AC_COMPILE_IFELSE("", [], [WARNING_CFLAGS=""])
CFLAGS="$kq_save_CFLAGS"
AC_SUBST(WARNING_CFLAGS)

# Checks for libraries and library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit gettimeofday memmove memset mkdir pow strchr strstr])
AC_CHECK_LIB(dumb, dumb_exit, [ALLEGRO_LIBS="-ldumb $ALLEGRO_LIBS"], [AC_MSG_ERROR([Dumb not found])])
AX_CHECK_ALDUMB([ALLEGRO_LIBS="-laldmb $ALLEGRO_LIBS"], [AC_MSG_ERROR([Aldumb not found])], [$ALLEGRO_LIBS])
AC_SEARCH_LIBS(lua_gettop, lua50 lua5.1 lua, [], [AC_MSG_ERROR([Lua not found])])

PKG_CHECK_MODULES(GTK, [gtk+-2.0 >= 2.8.0 libglade-2.0 >= 2.4.0],
  [AC_SUBST(GTK_LIBS)
  AC_SUBST(GTK_CFLAGS)
  MAPDRAW2="mapdraw2${EXEEXT}"],
  [MAPDRAW2=""])
AC_SUBST(MAPDRAW2)

m4_ifdef([AS_HELP_STRING], , [m4_define([AS_HELP_STRING], m4_defn([AC_HELP_STRING]))])
AC_ARG_WITH(2xsai, AS_HELP_STRING([--with-2xsai], [Use the 2xSAI library (default NO)]), AC_SUBST(kq_use_2xsai,1))
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [Enable debug mode]),
  [AC_DEFINE(DEBUGMODE,1,[Some debug options])])
AC_ARG_ENABLE(profile, AS_HELP_STRING([--enable-profile], [Enable profile mode]), AC_SUBST(kq_use_profile,1))
AC_ARG_ENABLE(cheats, AS_HELP_STRING([--enable-cheats], [Enable cheats]), AC_DEFINE(KQ_CHEATS,1, [Add some code to allow the player to cheat]))

# host stuff
case ${host} in
     *-*-linux*)
     kq_extra_files='unix.$(OBJEXT)'
     AC_SUBST([kq_extra_files])
     AC_SUBST(kq_target_dir,unix)
     bundle=no
     ;;
     *-*-darwin*)
     kq_extra_files='osx.$(OBJEXT)'
     AC_SUBST([kq_extra_files])
     AC_SUBST(kq_target_dir,macosx)
     bundle=yes
     ;;
     *-*-mingw32* | *-*-cygwin* | *-*-windows*)
     kq_extra_files='win.$(OBJEXT)'
     AC_SUBST([kq_extra_files])
     bundle=no
	 #LIBS="-lwinmm ${LIBS}"
     ;;
esac
AM_CONDITIONAL(BUNDLE, test x$bundle = xyes)
AC_CONFIG_FILES([Makefile scripts/Makefile po/Makefile.in])
AC_OUTPUT

