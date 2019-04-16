ACLOCAL_AMFLAGS = -I m4

# cli program
bin_PROGRAMS = texpackr
texpackr_CFLAGS = -Wall \
									-std=c99 \
									-O2 \
									-pedantic \
									-I./include
texpackr_LDFLAGS = -lhashmap_c \
									 -lpng \
									 -lz \
									 -lm
texpackr_SOURCES = src/lib.c \
									 src/cli.c \
									 src/meta.c \
									 src/png_util.c \
									 src/sprite.c \
									 src/treetrv.c

EXTRA_DIST = README.md LICENSE autogen.sh

# lib
lib_LTLIBRARIES = libtexpackr.la
libtexpackr_la_CFLAGS = -Wall \
												-std=c99 \
												-O2 \
												-pedantic \
												-I./include
libtexpackr_la_LDFLAGS = -no-defined -version-info 0:1:0 \
												 -lhashmap_c \
												 -lpng \
												 -lz \
												 -lm
libtexpackr_la_SOURCES = src/lib.c \
												 src/meta.c \
												 src/png_util.c \
												 src/sprite.c \
												 src/treetrv.c

libtexpackrdir=$(includedir)/texpackr
libtexpackr_HEADERS = include/texpackr/internal.h \
									    include/texpackr/log.h \
									    include/texpackr/meta.h \
									    include/texpackr/png_util.h \
									    include/texpackr/sprite.h \
									    include/texpackr/texpackr.h \
									    include/texpackr/types.h

# tests program
test_common_cflags = -Wall \
							-std=c99 \
							-O2 \
							-pedantic \
							-I. \
							-I./include
test_common_ldflags = -ltexpackr \
							 -lhashmap_c \
							 -lpng \
							 -lz \
							 -lm

check_PROGRAMS = test/test test/test_readfrommem
TESTS = $(check_PROGRAMS)
test_test_SOURCES = test/test.c
test_test_CFLAGS = $(test_common_cflags)
test_test_LDFLAGS = $(test_common_ldflags)

test_test_readfrommem_SOURCES = test/test_readfrommem.c
test_test_readfrommem_CFLAGS = $(test_common_cflags)
test_test_readfrommem_LDFLAGS = $(test_common_ldflags)
