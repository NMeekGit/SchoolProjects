# Makefile for Program 2
# @author: Noah Meeker
# Class: CS530

CC = g++
CFLAGS = -Wall -std=c++11 -g3 -c
OBJS = Disassembler.o Driver.o

disassem: $(OBJS)
	$(CC) -o disassem $^

driver.o: Driver.cpp
	$(CC) $(CFLAGS) Driver.cpp

disassembler.o: Disassembler.cpp Disassembler.h
	$(CC) $(CFLAGS) Disassembler.cpp

clean:
	rm -f *.o disassem *.txt 