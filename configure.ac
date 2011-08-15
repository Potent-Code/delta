#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([mathlib], [0.1], [ryan.lucchese@gmail.com])
AM_INIT_AUTOMAKE([mathlib], [0.1])
AC_CONFIG_SRCDIR([float_cmp.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
LT_INIT
LT_OUTPUT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([float.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC

AC_OUTPUT(Makefile)
