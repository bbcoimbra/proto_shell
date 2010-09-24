#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([proto-shell], [0.1.0], [bbcoimbra@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/shell.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
# Checks for programs.
AC_PROG_CC
CFLAGS="-O0 -Wall"

# Checks for libraries.
VL_LIB_READLINE
# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T

# Checks for library functions.
AC_CHECK_FUNCS([strdup])
AC_FUNC_FORK
AC_FUNC_MALLOC


AC_OUTPUT
