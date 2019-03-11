SRCDIR := src
INCDIR := include
BUILDDIR := build
TESTDIR := test

CC := gcc
override CFLAGS += -std=c99 -g -Wall -I$(INCDIR)
LFLAGS =

DEPS :=	\
	$(BUILDDIR)/texpackr_lib.o	\
	$(BUILDDIR)/texpackr_cli.o	\
	$(BUILDDIR)/texpackr_test.o

.PHONY: all mkbuilddir clean lib

all: mkbuilddir $(DEPS) texpackr test

mkbuilddir: 
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/texpackr_lib.o: $(SRCDIR)/texpackr_lib.c $(INCDIR)/texpackr.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/texpackr_cli.o: $(SRCDIR)/texpackr_cli.c $(INCDIR)/texpackr.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/texpackr_test.o: $(TESTDIR)/texpackr_test.c $(INCDIR)/texpackr.h
	$(CC) $(CFLAGS) -c $< -o $@

texpackr: $(BUILDDIR)/texpackr_lib.o $(BUILDDIR)/texpackr_cli.o
	$(CC) $(LFLAGS) $^ -o $@

test: mkbuilddir $(BUILDDIR)/texpackr_test.o $(BUILDDIR)/texpackr_lib.o
	$(CC) $(LFLAGS) $(BUILDDIR)/texpackr_test.o $(BUILDDIR)/texpackr_lib.o -o texpackr_test

clean:
	rm -rf $(BUILDDIR)
	rm -f texpackr
	rm -f texpackr_test
