# Makefile for Program 1
# @author: Noah Meeker
# Class: CS 530

CC=g++
CFLAGS=-Wall -std=c++11 -g3 -c
OBJS = disassembler.o driver.o

disassem: $(OBJS)
	$(CC) -o disassem $^

driver.o: driver.cpp
	$(CC) $(CFLAGS) driver.cpp

disassembler.o: disassembler.cpp disassembler.h
	$(CC) $(CFLAGS) disassembler.cpp

clean:
		rm -f *.o disassem *.txt

