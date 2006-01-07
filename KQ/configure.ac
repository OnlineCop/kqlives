#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)
AC_INIT(KQ, 0.98, [peterhull90@users.sourceforge.net])
AC_CONFIG_SRCDIR([src/kq.c])
AC_CONFIG_HEADER([config.h])

# What are we running on
AC_CANONICAL_SYSTEM

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_CHECK_PROG([HAVE_ALLEGRO], [allegro-config], [yes])
if test "${HAVE_ALLEGRO}" != "yes" ; then
 AC_ERROR([Allegro not found])
fi

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h])
AC_CHECK_HEADERS([lua50/lua.h],,[
	AC_CHECK_HEADERS([lua.h],,[
	AC_ERROR([Lua not found])
	])
	])

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
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [Enable debug mode]), AC_SUBST(kq_use_debug,1))
AC_ARG_ENABLE(profile, AS_HELP_STRING([--enable-profile], [Enable profile mode]), AC_SUBST(kq_use_profile,1))
AC_ARG_ENABLE(cheats, AS_HELP_STRING([--enable-cheats], [Enable cheats]), AC_SUBST(kq_use_cheats,1))

# host stuff
case ${host} in
     *-*-linux*)
     AC_SUBST(kq_extra_src,[unix.c])
     AC_SUBST(kq_target_dir,unix)
     ;;

     *-*-darwin*)
     AC_SUBST(kq_extra_src, [macosx.m])
     AC_SUBST(kq_target_dir,macosx)
     ;;
esac

AC_OUTPUT

