#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)
AC_INIT(KQ, 0.98, peterhull90@users.sourceforge.net)
AC_CONFIG_SRCDIR([src/kq.c])
AC_CONFIG_HEADER([config.h])

# What are we running on
AC_CANONICAL_SYSTEM

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_CHECK_PROG([HAVE_ALLEGRO], [allegro-config], [yes])
if test \( -z "${HAVE_ALLEGRO}" \) ; then
 AC_ERROR([Allegro not found])
fi


# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h])
AC_CHECK_HEADERS([lua.h lua50/lua.h], [LUA=1])
if test \( -z "${LUA}" \) ; then
 AC_ERROR([Lua not found])
fi


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
AC_CHECK_LIB(dumb, dumb_exit, [])
AC_CHECK_LIB(aldmb, al_duh_set_volume, [], [], [`allegro-config --libs`])
AC_SEARCH_LIBS(lua_open,  lua50 lua)

AC_CONFIG_FILES([Makefile])
AC_ARG_WITH(jgmod, AS_HELP_STRING([--with-jgmod], [Use the JGMod library (default NO)]), AC_SUBST(kq_use_jgmod,1))
AC_ARG_WITH(2xsai, AS_HELP_STRING([--with-2xsai], [Use the 2xSAI library (default NO)]), AC_SUBST(kq_use_2xsai,1))
AC_ARG_WITH(debug, AS_HELP_STRING([--with-debug], [Enable debug mode]), AC_SUBST(kq_use_debug,1))
AC_ARG_WITH(cheats, AS_HELP_STRING([--with-cheats], [Enable cheats]), AC_SUBST(kq_use_cheats,1))
AC_OUTPUT
