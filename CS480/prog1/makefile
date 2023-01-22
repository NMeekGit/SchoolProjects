# Assignment 1 : CountPrefix
# Makefile
#
# @class: CS 480
# @author: Noah Meeker
# @REDID: 821272563

CC=g++
CFLAGS=-Wall -std=c++11 -g3 -c

countprefix: driver.o dictionary.o
	$(CC) -o countprefix $^

dictionary.o: dictionary.cpp
	$(CC) $(CFLAGS) $^

driver.0: driver.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o countprefix
