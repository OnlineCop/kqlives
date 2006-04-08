# Check for Objective-C compiler.
# For now we only handle the GNU compiler.

# Copyright (C) 2004 Sir Raorn <raorn@altlinux.ru>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

AC_DEFUN([AM_PROG_OBJC],[
AC_CHECK_PROGS(OBJC, $CC gcc, gcc)
test -z "$OBJC" && AC_MSG_ERROR([no acceptable gcc found in \$PATH])
if test "x${OBJCFLAGS-unset}" = xunset; then
   OBJCFLAGS="-g -O2"
fi
AC_SUBST(OBJCFLAGS)
_AM_IF_OPTION([no-dependencies],, [_AM_DEPENDENCIES(OBJC)])
])


# AX_CHECK_ALDUMB([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND],
#              [OTHER-LIBRARIES])
# On some platforms, aldumb needs some variable the END_OF_MAIN
# macro declares, so AC_CHECK_LIB cannot be used. Instead,
# this macro does what AC_CHECK_LIB does, only with a different
# sourcecode used for compilation.

# Copyright (C) 1992, 1993, 1994, 1995, 1996, 1998, 1999, 2000, 2001,
# 2002 Free Software Foundation, Inc.
# Copyright (C) 2006 Günther Brammer

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

AC_DEFUN([AX_CHECK_ALDUMB],
[m4_ifval([$1], , [AH_CHECK_LIB([aldmb])])dnl
AS_VAR_PUSHDEF([ax_Lib], [ax_cv_lib_aldmb])dnl
AC_CACHE_CHECK([for al_duh_set_volume in -laldmb], ax_Lib,
[ax_check_lib_save_LIBS=$LIBS
LIBS="-laldmb $3 $LIBS"
AC_LINK_IFELSE([AC_LANG_SOURCE([[#include <allegro.h>
#include <aldumb.h>
int main(int argc, char * argv[]) { al_duh_set_volume(NULL, 0); return 0; }
END_OF_MAIN();]])],
	       [AS_VAR_SET(ax_Lib, yes)],
	       [AS_VAR_SET(ax_Lib, no)])
LIBS=$ax_check_lib_save_LIBS])
AS_IF([test AS_VAR_GET(ax_Lib) = yes],
      [m4_default([$1], [AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_LIBaldmb))
  LIBS="-laldmb $LIBS"
])],
      [$2])dnl
AS_VAR_POPDEF([ax_Lib])dnl
])# AX_CHECK_ALDUMB