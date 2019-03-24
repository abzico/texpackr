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
# FIXME: Replace `main' with a function in `-lpng':
AC_CHECK_LIB([png], [png_create_read_struct \
                     png_create_info_struct \
                     png_destroy_read_struct \
                     png_jmpbuf \
                     png_init_io \
                     png_set_sig_bytes \
                     png_read_info \
                     png_get_image_width \
                     png_get_image_height \
                     png_get_bit_depth \
                     png_get_color_type \
                     png_get_interlace_type \
                     png_get_channels \
                     png_get_rowbytes \
                     png_read_image \
                     png_create_write_struct \
                     png_destroy_write_struct \
                     png_set_IHDR \
                     png_write_info \
                     png_write_image \
                     png_write_end ])
# FIXME: Replace `main' with a function in `-lz':
#AC_CHECK_LIB([z], [])

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
