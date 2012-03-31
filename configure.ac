#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([mathlib], [0.1], [ryan.lucchese@write-c.com])
AM_INIT_AUTOMAKE([mathlib], [0.1])
AC_CONFIG_SRCDIR([src/mathlib.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
LT_INIT
LT_OUTPUT

CFLAGS="-O0 -g3 -Wall -Wextra -Werror"
AC_ARG_ENABLE([nodebug], [AC_HELP_STRING([--enable-nodebug],
	          [build without debugging information (default NO)])],
		      [test "$enableval" = "yes" && CFLAGS="-O3 -Wall -Wextra -Werror"])


# Checks for header files.
AC_CHECK_HEADERS([float.h stdlib.h])
AC_CONFIG_FILES([mathlib.pc])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC

AC_OUTPUT(Makefile src/Makefile)
