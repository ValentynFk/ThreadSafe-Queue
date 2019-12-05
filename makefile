# This makefile is intended to build the ThreadSafe-Queue solution
# Written by Valentyn Faychuk at SoftServe on 05.12.2019
# email:
# 		faitchouk.valentyn@gmail.com

SHELL = /bin/sh
$(shell mkdir -p build)

ODIR = build
# Add here all the include directories
IDIR = .
# Add here all the sources in the project
SOURCES = main threadsafe-queue
#A Add here all the libs, needed by the project
CLIBS = -lpthread

CC = gcc
CFLAGS = -g -I$(IDIR)

.PHONY: all clean threadsafequeue

all: threadsafequeue

threadsafequeue: setup
	$(CC) $(CFLAGS) -o $(ODIR)/thread-safe-queue $(shell find $(ODIR)/ -type f -name \*.o) $(CLIBS)

setup:
	for SOURCE in $(SOURCES) ; do \
		$(CC) -c $$SOURCE.c $(CFLAGS) -o $(ODIR)/$$SOURCE.o ; \
	done

clean:
	rm -f $(ODIR)/*.o $(ODIR)/thread-safe-queue core

