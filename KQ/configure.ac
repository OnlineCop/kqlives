#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(KQ, 0.98, peterhull90@users.sourceforge.net)
AC_CONFIG_SRCDIR([src/kq.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h unistd.h lua.h lua50/lua.h])

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
AC_CHECK_LIB(dumb, load_duh, [])
AC_SEARCH_LIBS(lua_open,  lua50 lua)

AC_CONFIG_FILES([Makefile
                 maps/Makefile
                 other/Makefile
                 scripts/Makefile])
AC_ARG_WITH(jgmod, AS_HELP_STRING([--with-jgmod], [Use the JGMod library (default NO)]), AC_SUBST(kq_use_jgmod,1))
AC_ARG_WITH(2xsai, AS_HELP_STRING([--with-2xsai], [Use the 2xSAI library (default NO)]), AC_SUBST(kq_use_2xsai,1))
AC_OUTPUT
