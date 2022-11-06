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
#include <unordered_map>
#include <algorithm>

using namespace std;

class Disassembler {

    static const string ops[];
    static const string mnemonics[];
    static const string mnemonics2[];
    static const string directives[];
    static const string specials[];

    bool format2;
    bool format3;
    bool format4;
    bool simple;
    bool indirect;
    bool immediate;
    bool indexed;
    bool pc;
    bool base;
    bool moveIndex;
    bool checkBase;

    int inputSizeOBJ;
    int inputSizeSYM;
    int symTableSize;
    int litTableSize;
    int txtRecordSize;
	int outputSize;
    int currSymLoc;
    int* txtSize;
    int rowSize;

    string programName;
    string nextAddr;
    string baseAddr;
    string indexAddr;
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

        void OpenFile(int, string, string); // Read in File
        void PrintFile();                   // Output new file

		void GrabHead();                    // Grab Head Record from input
        void GrabTXT();                     // Grab TXT Record from input
        void GrabSYM();                     // Grab SYM Table from input
        void GrabTXTInfo();                 // Grab TEXT Record Info
        
        void Solve();                       // Begin parsing
        void LoadOutput(int, int);          // Fills in outout[][]
        void FinishOutput();                // Closes off output[][]
        void FindFlags(int);                // Find nixbpe
        string GrabInstruction(int);        // Grab Instructions
        string GrabSymbol(int);             // Grab Symbols

        void FillSYMTable(int);             // Fills a Symbol Table
        void FillLITTable(int);             // Fills a Literal Table
        void ResetFlags();                  // Resets Flags
        void binMap(unordered_map<string,char>*); // Unordered Map of Binary to Hex

        int HexToDec(string);               // Convert string hexadecimal to decimal

        string BinToHex(string);            // Convert Binary to Hex
        string HexToBin(string);            // Convert string hex to uint
        string MaskOP(string);              // Mask for Operand
        string MaskFlag(string);            // Mask for Flags
        string MaskREC(string);             // Mask for PC Record
        string AddHex(string, string);      // Add Two Hex Numbers
};

#endif
