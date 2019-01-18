#
# Makefile for thread tests
#

CC = gcc
CFLAGS = -g -Wall -std=c99
EXES = mutex error1 

all: $(EXES)

mutex: mutex.o thread.o thr_asm.o
	$(CC) $(CFLAGS) mutex.o thread.o thr_asm.o -o mutex

error1: error1.o thread.o thr_asm.o
	$(CC) $(CFLAGS) error1.o thread.o thr_asm.o -o error1

thread.o: thread.c
	$(CC) $(CFLAGS) -c thread.c

thr_asm.o: thr_asm.s
	$(CC) $(CFLAGS) -c thr_asm.s

clean:
	-rm -f *.o $(EXES)

