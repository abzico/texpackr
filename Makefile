SRCDIR := src
INCLDIR := include/texpackr
BUILDDIR := build
TESTDIR := test
EXTSDIR := $(SRCDIR)/externals
OUT_STATIC_LIBNAME := libtexpackr.a

CC := gcc
override CFLAGS += -std=c99 -g -Wall -Iinclude -I$(EXTSDIR)/hashmap_c/include
LFLAGS = -lpng -lz

DEPS :=	\
	$(BUILDDIR)/lib.o	\
	$(BUILDDIR)/png_util.o		\
	$(BUILDDIR)/cli.o	\
	$(BUILDDIR)/texpackr_test.o	\
	$(BUILDDIR)/treetrv.o		\
	$(BUILDDIR)/meta.o	\
	$(BUILDDIR)/sprite.o	\
	$(EXTSDIR)/hashmap_c/build/libhashmapc.a

LIB_DEPS := \
	$(BUILDDIR)/lib.o	\
	$(BUILDDIR)/png_util.o		\
	$(BUILDDIR)/treetrv.o		\
	$(BUILDDIR)/meta.o	\
	$(BUILDDIR)/sprite.o	\
	$(EXTSDIR)/hashmap_c/build/libhashmapc.a

.PHONY: all mkbuilddir clean static-lib

all: mkbuilddir $(DEPS) texpackr

mkbuilddir: 
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/lib.o: $(SRCDIR)/lib.c $(INCLDIR)/texpackr.h $(INCLDIR)/types.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/png_util.o: $(SRCDIR)/png_util.c $(INCLDIR)/png_util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/cli.o: $(SRCDIR)/cli.c $(INCLDIR)/texpackr.h $(INCLDIR)/types.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/texpackr_test.o: $(TESTDIR)/texpackr_test.c $(INCLDIR)/texpackr.h $(INCLDIR)/types.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/treetrv.o: $(SRCDIR)/treetrv.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/meta.o: $(SRCDIR)/meta.c $(INCLDIR)/meta.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/sprite.o: $(SRCDIR)/sprite.c $(INCLDIR)/sprite.h
	$(CC) $(CFLAGS) -c $< -o $@

$(EXTSDIR)/hashmap_c/build/libhashmapc.a:
	# to produce .a library file
	make lib -C $(EXTSDIR)/hashmap_c

# cli output
texpackr: $(BUILDDIR)/lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/cli.o $(BUILDDIR)/treetrv.o $(BUILDDIR)/meta.o $(BUILDDIR)/sprite.o $(EXTSDIR)/hashmap_c/build/libhashmapc.a
	$(CC) $(LFLAGS) $^ -o $@

# test program output
test: mkbuilddir $(BUILDDIR)/texpackr_test.o $(BUILDDIR)/lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/treetrv.o $(BUILDDIR)/meta.o $(BUILDDIR)/sprite.o $(EXTSDIR)/hashmap_c/build/libhashmapc.a
	$(CC) $(BUILDDIR)/texpackr_test.o $(BUILDDIR)/lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/treetrv.o $(BUILDDIR)/meta.o $(BUILDDIR)/sprite.o $(LFLAGS) $(EXTSDIR)/hashmap_c/build/libhashmapc.a -o texpackr_test

# create static library
static-lib: mkbuilddir $(LIB_DEPS)
	ar rcs $(BUILDDIR)/$(OUT_STATIC_LIBNAME) $(LIB_DEPS)

clean-deps:
	make clean -C $(EXTSDIR)/hashmap_c

clean:
	rm -rf $(BUILDDIR)
	rm -f texpackr
	rm -f texpackr_test
