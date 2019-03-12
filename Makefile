SRCDIR := src
BUILDDIR := build
TESTDIR := test

CC := gcc
override CFLAGS += -std=c99 -g -Wall -I$(SRCDIR)
LFLAGS = -lpng -lz

DEPS :=	\
	$(BUILDDIR)/texpackr_lib.o	\
	$(BUILDDIR)/png_util.o		\
	$(BUILDDIR)/texpackr_cli.o	\
	$(BUILDDIR)/texpackr_test.o	\
	$(BUILDDIR)/treetrv.o

.PHONY: all mkbuilddir clean lib

all: mkbuilddir $(DEPS) texpackr test

mkbuilddir: 
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/texpackr_lib.o: $(SRCDIR)/texpackr_lib.c $(SRCDIR)/texpackr.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/png_util.o: $(SRCDIR)/png_util.c $(SRCDIR)/png_util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/texpackr_cli.o: $(SRCDIR)/texpackr_cli.c $(SRCDIR)/texpackr.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/texpackr_test.o: $(TESTDIR)/texpackr_test.c $(SRCDIR)/texpackr.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/treetrv.o: $(SRCDIR)/treetrv.c
	$(CC) $(CFLAGS) -c $< -o $@

texpackr: $(BUILDDIR)/texpackr_lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/texpackr_cli.o $(BUILDDIR)/treetrv.o
	$(CC) $(LFLAGS) $^ -o $@

test: mkbuilddir $(BUILDDIR)/texpackr_test.o $(BUILDDIR)/texpackr_lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/treetrv.o
	$(CC) $(LFLAGS) $(BUILDDIR)/texpackr_test.o $(BUILDDIR)/texpackr_lib.o $(BUILDDIR)/png_util.o $(BUILDDIR)/treetrv.o -o texpackr_test

clean:
	rm -rf $(BUILDDIR)
	rm -f texpackr
	rm -f texpackr_test
