#include "Disassembler.h"

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

    txtSize = new int[MAX_SIZE];
    txtStart = new string[MAX_SIZE];
    txtRecord = new string[MAX_SIZE];
    symTable = new string*[MAX_SIZE];
    litTable = new string*[MAX_SIZE];
	output = new string*[MAX_SIZE];
    
    for (int i = 0; i < MAX_SIZE; i++) {
        symTable[i] = new string[2];
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        litTable[i] = new string[4];
    }
	for (int i = 0; i < MAX_SIZE; i++) {
		output[i] = new string[5];
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

    Solve();
};

void Disassembler::PrintFile() {

    ofstream fileTXT("out.lst");

    if (fileTXT.is_open()) {
		
		int l = 12;
		for (int i = 0; i < outputSize; i++) {

			fileTXT << left << setw(l) <<  output[i][0] << setw(l) << output[i][1] << setw(l) << output[i][2] << setw(l) << output[i][3] << setw(l) << output[i][4] << endl;

		}
    }

    fileTXT.close();

    cout << "File Printed" << endl;

};

void Disassembler::Solve() {

	GrabHead();
    GrabTXT();
    GrabSYM();

    GrabTXTInfo();
};

void Disassembler::GrabHead() {

	cout << "\nGrabbing Header Record\n" << endl;

	string programName = inputOBJ[0].substr(1,6);
	string startAddr = inputOBJ[0].substr(7,6);

	output[0][0] = startAddr;
	output[0][1] = programName;
	output[0][2] = "START";
	output[0][3] = "0";
	output[0][4] = "";
	outputSize = 1;

};

void Disassembler::GrabTXT() {

    cout << "\nGrabbing Text Record\n" << endl;
    txtRecordSize = 0;
    for (int i = 0; i <= inputSizeOBJ; i++) {

        if (inputOBJ[i][0] == 'T') {

            txtRecord[txtRecordSize] = inputOBJ[i];
            txtRecordSize++;
        }
    }

    cout << "\tText Record Recieved:" << endl;
    for (int i = 0; i < txtRecordSize ; i++) {
        cout << "\t\t" << txtRecord[i] << endl;
    }

    cout << "\tText Record Size: " << txtRecordSize << endl;
};

void Disassembler::GrabSYM() {

    cout << "\nGrabbing Symbol Information\n" << endl;

    symTableSize = 0;
    litTableSize = 0;
    for (int i = 0; i < inputSizeSYM; i++) {

        if (inputSYM[i][0] == 'S') {

            for (int j = i+2; j < inputSizeSYM && (inputSYM[j][0] != 'N'); j++) {

                FillSYMTable(j);

            }
        }

        if (inputSYM[i][0] == 'N') {

            for (int j = i+2; j < inputSizeSYM; j++) {

                FillLITTable(j);

            }
        }

    }

    cout << "\tCurrent Symbol Table:" << endl;
    for (int i = 0; i < symTableSize; ++i) {
        cout << "\t\t" << symTable[i][0] << setw(8) << symTable[i][1] << endl;
    }
    cout << "\tSymbol Table Size: " << symTableSize << "\n" << endl;

    cout << "\tCurrent Litteral Table:" << endl;
    for (int i = 0; i < litTableSize; ++i) {
        cout << "\t\t" << litTable[i][0] << setw(12) << litTable[i][1] << setw(12) << litTable[i][2] << setw(12) << litTable[i][3] << endl;
    }
    cout << "\tLitteral Table Size: " << litTableSize << endl;
};

void Disassembler::GrabTXTInfo() {

    cout << "\nGrabing Text Info\n" << endl;

    int i;
    for (i = 0; i < txtRecordSize; i++) {

        txtSize[i] = HexToDec(txtRecord[i].substr(7,2)); // Retrieve Text Record actual size

        txtStart[i] = txtRecord[i].substr(1,6); // Grab Start Address
        txtRecord[i] = txtRecord[i].erase(0,9); // Remove the first 9 hex digits.

    }

    cout << "\tText Record Recieved:" << endl;
    for (i = 0; i < txtRecordSize; i++) {
        cout << "\t\tSize: " << txtSize[i] << setw(12) << "Start: " << txtStart[i] << setw(18) << "Record: " << txtRecord[i] << endl;
    }

};

void Disassembler::FillSYMTable(int i) {

    istringstream ss(inputSYM[i]);
    string sym, addr, flag;
    while (ss >> sym >> addr >> flag) {

        symTable[symTableSize][0] = sym;
        symTable[symTableSize][1] = addr;
        symTableSize++;

    }
};

void Disassembler::FillLITTable(int i) {

    istringstream ss(inputSYM[i]);
    string name, lit, len, addr;
    while (ss >> name >> lit >> len >> addr) {

        litTable[litTableSize][0] = name;
        litTable[litTableSize][1] = lit;
        litTable[litTableSize][2] = len;
        litTable[litTableSize][3] = addr;
        litTableSize++;

    }

    istringstream tt(inputSYM[i]);
    while (tt >> lit >> len >> addr) {
        if (lit[0] == '=') {
            litTable[litTableSize][0] = "";
            litTable[litTableSize][1] = lit;
            litTable[litTableSize][2] = len;
            litTable[litTableSize][3] = addr;
            litTableSize++;
        }
    }

};

void Disassembler::FindFlags() {
	
	simple = false;
	indirect = false;
	immediate = false;
	indexed = false;
	pc = false;
	base = false;
	format2 = false;
	format3 = false;
	format4 = false;
	
	for (int i = 0; i < txtRecordSize; i++) {

		string hex = HexToBin(txtRecord[i].substr(1,2));
		string flagValue = MaskFlag(hex);

		if (flagValue == "00110000") { simple = true };
		else if (flagValue == "00110001") { simple = true; format4 = true; };
		else if (flagValue == "00110010") { simple = true; pc = true; };
		else if (flagValue == "00110100") { simple = true; base = true; };
		else if (flagValue == "00111000") { simple = true; indexed = true; };
		else if (flagValue == "00111001") { simple = true; indexed = true; format4 = true; };
		else if (flagValue == "00111010") { simple = true; indexed = true; pc = true; };
		else if (flagValue == "00111100") { simple = true; indexed = true; base = true; };
		else if (flagValue == "00100000") { indirect = true; };
		else if (flagValue == "00100001") { indirect = true; format4 = true; };
		else if (flagValue == "00100010") { indirect = true; pc = true; };
		else if (flagValue == "00100100") { indirect = true; base = true };
		else if (flagValue == "00010000") { immediate = true; };
		else if (flagValue == "00010001") { immediate = true; format4 = true; };
		else if (flagValue == "00010010") { immediate = true; pc = true; };
		else if (flagValue == "00010100") { immediate = true; base = true; };

		
	}
};

int Disassembler::HexToDec(string hex){
	int length = hex.size();
	int base = 1;
	int dec = 0;

	for (int i = length-1; i >= 0; i--) {
		if (hex[i] >= '0' && hex[i] <= '9') {
			dec += (int(hex[i])-48) * base;
			base *= 16;
		}
		else if (hex[i] >= 'A' && hex[i] <= 'F') {
			dec += (int(hex[i])-55) * base;
			base *= 16;
		}
	}
	return dec;
};

