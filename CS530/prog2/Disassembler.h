/*
 * @author Noah Meeker
 * Red ID: 821272563
 * Date: Oct. 29
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
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

class Disassembler {

    static const string ops[];
    static const string mnemonics[];
    static const string mnemonics2[];

    bool format2;
    bool format3;
    bool format4;
    bool simple;
    bool indirect;
    bool immediate;
    bool indexed;
    bool pc;
    bool base;

    int inputSizeOBJ;
    int inputSizeSYM;
    int symTableSize;
    int litTableSize;
    int txtRecordSize;
	int outputSize;
    int* txtSize;
    int rowSize;

    string* txtRecord;
    string* txtStart;
    string* inputOBJ;
    string* inputSYM;
    string** symTable;
    string** litTable;
    string** output;

    ifstream fileOBJ;
    ofstream fileTXT;

    public:

        static int MAX_SIZE;

        Disassembler();                     // Constructor
        ~Disassembler();                    // Deconstructor

        void OpenFile(int, string, string);         // Read in File
        void PrintFile();                   // Output new file

		void GrabHead();
        void GrabTXT();                     // Grab TXT Record from input
        void GrabSYM();                     // Grab SYM Table from input
        void GrabTXTInfo();                 // Grab TEXT Record Info
        
        void Solve();                       // Begin parsing
        void LoadOutput(int, int);
        void FindFlags(int);                   // Find nixbpe
        string GrabInstruction(int);             // Grab Instructions

        void FillSYMTable(int);
        void FillLITTable(int);
        void ResetFlags();

        int HexToDec(string);               // Convert string hexadecimal to decimal

        string HexToBin(string);       // Convert string hex to uint
        string MaskOP(string);             // Mask for Operand
        string MaskFlag(string);           // Mask for Flags
        string AddHex(string, string);
};

#endif
