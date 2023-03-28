# Assignment 1 : CountPrefix
# Makefile
#
# @class: CS 480
# @author: Noah Meeker
# @REDID: 821272563

CC=g++
CFLAGS=-Wall -std=c++11 -g3 -c

countprefix: driver.o PageTable.o TLB.o vaddr_tracereader.o print_helpers.o
	$(CC) -o help $^

PageTable.o: PageTable.cpp
	$(CC) $(CFLAGS) $^

TLB.o: TLB.cpp
	$(CC) $(CFLAGS) $^

vaddr_tracereader.o: vaddr_tracereader.c
	$(CC) $(CFLAGS) $^

print_helpers.o: print_helpers.c
	$(CC) $(CFLAGS) $^

driver.o: driver.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o help
