# This file will build the ThreadSafe-Queue solution
SHELL = /bin/sh

$(shell mkdir -p build)

ODIR = build
IDIR = .

CLIBS = -lpthread
CFLAGS = -g -I$(IDIR)
CC = gcc

.PHONY: all clean threadsafequeue

all: threadsafequeue

threadsafequeue: setup
	$(CC) -o $(ODIR)/thread-safe-queue $(shell find $(ODIR)/ -type f -name \*.o) $(CLIBS)

setup:
	$(CC) -c main.c $(CFLAGS) -o $(ODIR)/main.o

clean:
	rm -f $(ODIR)/*.o $(ODIR)/thread-safe-queue core

