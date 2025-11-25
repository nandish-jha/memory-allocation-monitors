# Nandish Jha NAJ474 11282001

# Makefile for Lab 6 - Best Fit Memory Allocation Simulator
# Replace student info as necessary

CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
OBJS = BestFit.o BestFitMonitor.o Monitor.o

LIB_PATH = /student/cmpt332/pthreads/lib/Linuxx86_64/
LIBS = -lpthreads
CPPFLAGS += -I/student/cmpt332/pthreads -I.

.PHONY: all clean

all: lab6

lab6: $(OBJS) liblist.a
	$(CC) $(CFLAGS) $(CPPFLAGS) -o lab6 $(OBJS) liblist.a -L$(LIB_PATH) $(LIBS) 

BestFit.o: BestFit.c Monitor.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c BestFit.c

BestFitMonitor.o: BestFitMonitor.c Monitor.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c BestFitMonitor.c

Monitor.o: Monitor.c Monitor.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c Monitor.c

liblist.a: list_movers.o list_adders.o list_removers.o
	ar rcs liblist.a list_movers.o list_adders.o list_removers.o

list_movers.o: list_movers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_movers.c

list_adders.o: list_adders.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_adders.c

list_removers.o: list_removers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_removers.c

clean:
	clear
	rm -f lab6 *.o output.txt liblist.a
