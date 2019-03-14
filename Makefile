SRCDIR := src
BUILDDIR := build
TESTDIR := test
EXTSDIR := $(SRCDIR)/externals

CC := gcc
override CFLAGS += -std=c99 -g -Wall -I$(SRCDIR)
LFLAGS = -lpng -lz

DEPS :=	\
	$(BUILDDIR)/texpackr_lib.o	\
	$(BUILDDIR)/png_util.o		\
	$(BUILDDIR)/texpackr_cli.o	\
	$(BUILDDIR)/texpackr_test.o	\
	$(BUILDDIR)/treetrv.o		\
	$(EXTSDIR)/hashmap_c/build/libhashmapc.a

.PHONY: all mkbuilddir clean lib

all: mkbuilddir $(DEPS) texpackr test

mkbuilddir: 
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/texpackr_lib.o: $(SRCDIR)/texpackr_lib.c $(SRCDIR)/texpackr.h $(SRCDIR)/types.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/png_util.o: $(SRCDIR)/png_util.c $(SRCDIR)/png_util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/texpackr_cli.o: $(SRCDIR)/texpackr_cli.c $(SRCDIR)/texpackr.h $(SRCDIR)/types.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/texpackr_test.o: $(TESTDIR)/texpackr_test.c $(SRCDIR)/texpackr.h $(SRCDIR)/types.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/treetrv.o: $(SRCDIR)/treetrv.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXTSDIR)/hashmap_c/build/libhashmapc.a:
	# to produce .a library file
	make lib -C $(EXTSDIR)/hashmap_c

# cli output
texpackr: $(BUILDDIR)/texpackr_lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/texpackr_cli.o $(BUILDDIR)/treetrv.o $(EXTSDIR)/hashmap_c/build/libhashmapc.a
	$(CC) $(LFLAGS) $^ -o $@

# test program output
test: mkbuilddir $(BUILDDIR)/texpackr_test.o $(BUILDDIR)/texpackr_lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/treetrv.o $(EXTSDIR)/hashmap_c/build/libhashmapc.a
	$(CC) $(BUILDDIR)/texpackr_test.o $(BUILDDIR)/texpackr_lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/treetrv.o $(LFLAGS) $(EXTSDIR)/hashmap_c/build/libhashmapc.a -o texpackr_test

clean:
	rm -rf $(BUILDDIR)
	rm -f texpackr
	rm -f texpackr_test
