#include "disassembler.h"

const string Disassembler::ops[] = {
    "18","58","90","40","B4","28",
    "88","A0","24","64","9C","C4",
    "C0","F4","3C","30","34","38",
    "48","00","68","50","70","08",
    "6C","74","04","D0","20","60",
    "98","C8","44","D8","AC","4C",
    "A4","A8","F0","EC","0C","78",
    "54","80","D4","14","7C","E8",
    "84","10","1C","5C","94","B0",
    "E0","F8","2C","B8","DC"
};

const string Disassembler::mnemonics[] = {
    "ADD","ADDF","ADDR","AND","CLEAR","COMP",
    "COMPF","COMPR","DIV","DIVF","DIVR","FIX",
    "FLOAT","HIO","J","JEQ","JGT","JLT",
    "JSUB","LDA","LDB","LDCH","LDF","LDL",
    "LDS","LDT","LDX","LPS","MUL","MULF",
    "MULR","NORM","OR","RD","RMO","RSUB",
    "SHIFTL","SHIFTR","SIO","SSK","STA","STB",
    "STCH","STF","STI","STL","STS","STSW",
    "STT","STX","SUB","SUBF","SUBR","SVC",
    "TD","TIO","TIX","TIXR","WB"
};

const string Disassembler::mnemonics2[] = {
	"ADDR","COMPR","DIVR","MULR","RMO","SHIFTL",
	"SHIFTR","SUBR","SVC","TIXR"
};

int Disassembler::MAX_SIZE = 100;

Disassembler::Disassembler() {

};

Disassembler::~Disassembler() {
    delete[] input;
    delete[] output;
};

void Disassembler::OpenFile(int index, string file){

    string line;
    input = new string[MAX_SIZE];
    inputSize = 0;

    ifstream fileOBJ(file);

    if (fileOBJ.is_open()) {
        
        cout << file << " is open" << endl; // Confirm File is Open

        int i = 0;
        while (getline(fileOBJ,line)) {

            input[i] = line;
            inputSize++;
            i++;

        }

        fileOBJ.close();

    }

    // Check current Input[]
    cout << "Current Input:\n" << endl;
    for (int i = 0; i <= inputSize; i++) {
        cout << input[i] << endl;
    }
    cout << "\nSize: " << inputSize << "\n" << endl;

    // grabTXT();
};

void Disassembler::PrintFile() {

    ofstream fileTXT("out.lst");

    if (fileTXT.is_open()) {

        fileTXT << "Hello World" << endl;

    }

    fileTXT.close();

    cout << "File Printed" << endl;

};