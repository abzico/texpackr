AC_PREREQ([2.69])
AC_INIT([texpackr], [0.1], [wasin@abzi.co])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_PROG_AR

AC_ENABLE_SHARED
AC_ENABLE_STATIC

LT_INIT

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AC_SEARCH_LIBS([sin], [m], [], [AC_MSG_ERROR("Unusual error. Not found libm. Make sure you have installed basic development / build package.")])
AC_SEARCH_LIBS([zlibVersion], [z], [], [AC_MSG_ERROR("Not found libz. Please install it on your system first")])
AC_SEARCH_LIBS([png_read_image], [png], [], [AC_MSG_ERROR("Not found libpng. Please install it on your system first")])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strtol])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
