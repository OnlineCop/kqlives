#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)
AC_INIT(KQ, 0.99, [peterhull90@users.sourceforge.net])
AC_CONFIG_SRCDIR([src/kq.c])
AM_CONFIG_HEADER([config.h])
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CC
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
LIBS="${ALLEGRO_LIBS} ${LIBS}"

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h sys/select.h])
AC_CHECK_HEADERS([lua50/lua.h], ,
  [AC_CHECK_HEADERS([lua.h], ,
    [AC_MSG_ERROR([Lua not found])])
  ])

AC_CHECK_HEADER([aldumb.h], , [AC_MSG_ERROR([aldumb.h from libaldmb not found])])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit gettimeofday memmove memset mkdir pow strchr strstr])
AC_CHECK_LIB(dumb, dumb_exit, [], [AC_MSG_ERROR([Dumb not found])])
AC_CHECK_LIB(aldmb, al_duh_set_volume, [], [AC_MSG_ERROR([Aldumb not found])])
AC_SEARCH_LIBS(lua_open, lua50 lua, [], [AC_MSG_ERROR([Lua not found])])

m4_ifdef([AS_HELP_STRING], , [m4_define([AS_HELP_STRING], m4_defn([AC_HELP_STRING]))])
AC_ARG_WITH(jgmod, AS_HELP_STRING([--with-jgmod], [Use the JGMod library (default NO)]), AC_SUBST(kq_use_jgmod,1))
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
AC_CONFIG_FILES([Makefile scripts/Makefile])
AC_OUTPUT

