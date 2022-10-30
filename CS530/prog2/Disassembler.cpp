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

    txtRecord = new string[MAX_SIZE];
    symTable = new string*[MAX_SIZE];
    
    for (int i = 0; i < MAX_SIZE; i++) {
        symTable[i] = new string[2];
    }

};

Disassembler::~Disassembler() {
    delete[] inputOBJ;
    delete[] inputSYM;
    delete[] output;
};

void Disassembler::OpenFile(int index, string file, string symbol){

    string line;
    inputOBJ = new string[MAX_SIZE];
    inputSizeOBJ = 0;

    ifstream fileOBJ(file);

    if (fileOBJ.is_open()) {
        
        cout << file << " is open" << endl; // Confirm File is Open

        int i = 0;
        while (getline(fileOBJ,line)) {

            inputOBJ[i] = line;
            inputSizeOBJ++;
            i++;

        }

        fileOBJ.close();

    }

    inputSYM = new string[MAX_SIZE];
    inputSizeSYM = 0;

    ifstream fileSYM(symbol);

    if (fileSYM.is_open()) {

        cout << symbol << " is open" << endl;

        int i = 0;
        while (getline(fileSYM,line)) {

            inputSYM[i] = line;
            inputSizeSYM++;
            i++;

        }
    }

    // Check current Input[]
    cout << "Current InputOBJ:\n" << endl;
    for (int i = 0; i <= inputSizeOBJ; i++) {
        cout << inputOBJ[i] << endl;
    }
    cout << "\nSize: " << inputSizeOBJ << "\n" << endl;

    // Check current symbol table input
    cout << "Current Symbol Table:\n" << endl;
    for (int i = 0; i < inputSizeSYM; i++) {
        cout << inputSYM[i] << endl;
    }
    cout << "\nSize: " << inputSizeSYM << "\n" << endl;

    GrabTXT();
    GrabSYM();
};

void Disassembler::PrintFile() {

    ofstream fileTXT("out.lst");

    if (fileTXT.is_open()) {

        fileTXT << "Hello World" << endl;

    }

    fileTXT.close();

    cout << "File Printed" << endl;

};

void Disassembler::GrabTXT() {

    cout << "Grabbing Text Record" << endl;

    for (int i = 0; i <= inputSizeOBJ; i++) {

        if (inputOBJ[i][0] == 'T') {

            txtRecord[i] = inputOBJ[i];

            cout << "Text Record Recieved: " << setw(12) << txtRecord[i] << endl;
        }
    }
};

void Disassembler::GrabSYM() {

    cout << "Grabbing Symbol Information" << endl;

    int row = 0;
    symTableSize = 0;
    for (int i = 0; i < inputSizeSYM; i++) {

        if (inputSYM[i][0] != 'S' && inputSYM[i][0] != '-') {

            istringstream ss(inputSYM[i]);
            string sym, addr, flag;
            while (ss >> sym >> addr >> flag) {

                symTable[row][0] = sym;
                symTable[row][1] = addr;
                symTableSize++;
                row++;

            }
        }
    }

    cout << "Current Symbol Table:" << endl;
    for (int i = 0; i < symTableSize; ++i) {
        cout << symTable[i][0] << setw(8) << symTable[i][1] << endl;
    }
    cout << "Symbol Table Size: " << symTableSize << endl;
};

