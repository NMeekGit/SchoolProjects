# Makefile for lab 4 OOP
#
# @author: Noah Meeker
#

CC= g++

CCFLAGS= -std=c++11 -Wall -g3 -c

OBJS= driver.o

PROGRAM= lab4

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

driver.o: driver.cpp
	$(CC) $(CCFLAGS) driver.cpp

clean:
	rm -f *.o $(PROGRAM)
