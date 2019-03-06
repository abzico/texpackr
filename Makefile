SRCDIR := ./src
INCDIR := ./include
BUILDDIR := ./build
OUT_OBJ_DIR := $(BUILDDIR)/obj
OUT_DIR := ./

CC := gcc
override CFLAGS += -std=c99 -g -Wall -I$(INCDIR)
LFLAGS =

all: $(OUT_DIR)/texpackr

$(OUT_OBJ_DIR)/texpackr.o: $(SRCDIR)/texpackr.c $(INCDIR)/texpackr_lib.h
	@mkdir -p $(OUT_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/texpackr: $(OUT_OBJ_DIR)/texpackr.o
	@mkdir -p $(OUT_DIR)
	$(CC) $(LFLAGS) $^ -o $@

clean:
	rm -rf $(BUILDDIR)
	rm -f $(OUT_DIR)/texpackr
