/*
 * @author Noah Meeker
 * Red ID: 821272563
 * Date: Sept. 27
 * Class: CS 530
 *
 * Disassembler.h
 * 
 */

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <cstdlib>
#include <string>
#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cmath>


using namespace std;

class Disassembler {

	static const string ops[];
	static const string mnemonics[];
	static const string mnemonics2[];
	static const string formats[];
	static const string oams[];
	static const string taams[];

	static const int PRIME;

	int inputSize;
	int txtSize;
	int rowSize;

	bool form2;

	string filename;
	string txtRecord;
	string* input;
	string* instr;
	string* format;
	string* oam;
	string* taam;
	string* obj;
	ifstream fileOBJ;
	ofstream fileTXT;

	public:	
		
		static int MAX_SIZE;

		Disassembler(); // Constructer
		~Disassembler(); // Deconstructor

		void openFile(int, string);  // Read in a file
		void printFile();	// Output new file
								
		void grabTXT(); // Grab TEXT Record from input
		void grabTXTInfo(); // Grab TEXT Record Info
		void initColumns(); // Initializa Columns

		void Solve(); // Begin parsing
		void findFlags(); // Find nixbpe
		void grabInstruction(); // Grab Instructions
		
		bool checkFormat2(); // Checking for Format 2 Instructions
		
		int HexToDec(string);

		string HexToBin(string);
		string MaskOP(string);
		string MaskFlag(string);
};

#endif
