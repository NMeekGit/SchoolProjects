/*
 * Disassembler.cpp
 *
 * @author Noah Meeker
 * Date: 11/2/2022
 * RED ID: 821272563
 *
 * Couldn't tell you what I did. Got lost myself on a time crunch and my code turned into spagetti 
 * real fast to try and grab as much points as possible. Welp always next time.
 */

#include "Disassembler.h"

map<char, int> hex_dec(void) {
    map<char, int> m{ { '0', 0 }, { '1', 1 },
                      { '2', 2 }, { '3', 3 },
                      { '4', 4 }, { '5', 5 },
                      { '6', 6 }, { '7', 7 },
                      { '8', 8 }, { '9', 9 },
                      { 'A', 10 }, { 'B', 11 },
                      { 'C', 12 }, { 'D', 13 },
                      { 'E', 14 }, { 'F', 15 } };

    return m;
};

map<int, char> dec_hex(void) {
    map<int, char> m{ { 0, '0' }, { 1, '1' },
                      { 2, '2' }, { 3, '3' },
                      { 4, '4' }, { 5, '5' },
                      { 6, '6' }, { 7, '7' },
                      { 8, '8' }, { 9, '9' },
                      { 10, 'A' }, { 11, 'B' },
                      { 12, 'C' }, { 13, 'D' },
                      { 14, 'E' }, { 15, 'F' } };

    return m;
};

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
    "TD","TIO","TIX","TIXR","WD"
};

const string Disassembler::mnemonics2[] = {
	"ADDR","COMPR","DIVR","MULR","RMO","SHIFTL",
	"SHIFTR","SUBR","SVC","TIXR","CLEAR"
};

const string Disassembler::directives[] = {
    "START", "END", "BYTE", "WORD", "RESB", "RESW"
};

const string Disassembler::specials[] = {
    "A","X","L","B","S","T","F","PC","SW"
};

int Disassembler::MAX_SIZE = 100;

/* 
 * Disassembler() : Constructor
 */
Disassembler::Disassembler() {

    txtSize = new int[MAX_SIZE];
    txtStart = new string[MAX_SIZE];
    txtRecord = new string[MAX_SIZE];
    symTable = new string*[MAX_SIZE];
    litTable = new string*[MAX_SIZE];
	output = new string*[MAX_SIZE];
    finalOutput = new string*[MAX_SIZE];
    
    for (int i = 0; i < MAX_SIZE; i++) {
        symTable[i] = new string[2];
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        litTable[i] = new string[4];
    }
	for (int i = 0; i < MAX_SIZE; i++) {
		output[i] = new string[5];
	}
    for (int i = 0; i < MAX_SIZE; i++) {
        finalOutput[i] = new string[5];
    }

    moveIndex = false;
    checkBase = false;
    literal = false;
    litorg = false;
    currSymLoc = 0;
    baseAddr = "";
};

/*
 * ~Disassembler() : Deconstructor
 */
Disassembler::~Disassembler() {
    delete[] inputOBJ;
    delete[] inputSYM;
    delete[] output;
    delete[] symTable;
    delete[] litTable;
    delete[] finalOutput;
    delete[] txtSize;
    delete[] txtStart;
    delete[] txtRecord;
};

/*
 * OpenFile() : Opens .obj file and .sym table
 */
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

/* 
 * PrintFile() : Prints final output
 */
void Disassembler::PrintFile() {

    ofstream fileTXT("out.lst");

    if (fileTXT.is_open()) {
		
		int l = 12;
		for (int i = 0; i <= finalSize; i++) {

			fileTXT << left << setw(l) <<  finalOutput[i][0] << setw(l) << finalOutput[i][1] << setw(l) << finalOutput[i][2] << setw(l) << finalOutput[i][3] << setw(l) << finalOutput[i][4] << endl;

		}
    }

    fileTXT.close();

    cout << "File Printed" << endl;

};

/* 
 * Solve() : Begins main program
 */
void Disassembler::Solve() {

	GrabHead();
    GrabTXT();
    GrabSYM();

    GrabTXTInfo();

    for (int currentTXT = 0; currentTXT < txtRecordSize; currentTXT++) {
        FindFlags(currentTXT);
    }

    FinishOutput();
    
    FinalOutput();
};

/*
 * GrabHead() : Reads Header Record
 */
void Disassembler::GrabHead() {

	cout << "\nGrabbing Header Record\n" << endl;

	programName = inputOBJ[0].substr(1,6);
	string startAddr = inputOBJ[0].substr(9,4);

	output[0][0] = startAddr;
	output[0][1] = programName;
	output[0][2] = "START";
	output[0][3] = "0";
	output[0][4] = " ";
	outputSize = 1;

};

/*
 * GrabTXT() : Reads Text Records
 */
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

/*
 * GrabSYM() : Grabs Symbol Table information
 */
void Disassembler::GrabSYM() {

    cout << "\nGrabbing Symbol Information\n" << endl;

    symTableSize = 0;
    litTableSize = 0;
    for (int i = 0; i < inputSizeSYM; i++) {

        if (inputSYM[i].substr(0,6) == "Symbol") {

            for (int j = i+2; j < inputSizeSYM && (inputSYM[j].substr(0,4) != "Name"); j++) {

                FillSYMTable(j);

            }
        }

        if (inputSYM[i].substr(0,4) == "Name") {

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

/*
 * GrabTXTInfo() : Grabs needed text record info for parseing
 */
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

/*
 * FillSYMTable(int) : Fills 2D array to create a symbol table
 */
void Disassembler::FillSYMTable(int i) {

    istringstream ss(inputSYM[i]);
    string sym, addr, flag;
    while (ss >> sym >> addr >> flag) {

        symTable[symTableSize][0] = sym;
        symTable[symTableSize][1] = addr.substr(2,4);
        symTableSize++;

    }
};

/*
 * FillLITTable(int) : Fills 2D array to create a literal table
 */
void Disassembler::FillLITTable(int i) {

    istringstream ss(inputSYM[i]);
    string name, lit, len, addr;
    while (ss >> name >> lit >> len >> addr) {

        litTable[litTableSize][0] = name;
        litTable[litTableSize][1] = lit;
        litTable[litTableSize][2] = len;
        litTable[litTableSize][3] = addr.substr(2,4);
        litTableSize++;

    }

    istringstream tt(inputSYM[i]);
    while (tt >> lit >> len >> addr) {
        if (lit[0] == '=') {
            litTable[litTableSize][0] = "";
            litTable[litTableSize][1] = lit;
            litTable[litTableSize][2] = len;
            litTable[litTableSize][3] = addr.substr(2,4);
            litTableSize++;
        }
    }

};

/*
 * FindFlags(int) : Parses txt record to grab needed member flags
 */
void Disassembler::FindFlags(int current) {
	
	for (int i = 0; (txtRecord[current].length() != 1) && !(txtRecord[current].empty()); i++) {

        ResetFlags();

		string hex = HexToBin(txtRecord[current].substr(1,2));
		string flagValue = MaskFlag(hex);

		if      (flagValue == "00110000") { simple = true; }
		else if (flagValue == "00110001") { simple = true; format4 = true; }
		else if (flagValue == "00110010") { simple = true; pc = true; }
		else if (flagValue == "00110100") { simple = true; base = true; }
		else if (flagValue == "00111000") { simple = true; indexed = true; }
		else if (flagValue == "00111001") { simple = true; indexed = true; format4 = true; }
		else if (flagValue == "00111010") { simple = true; indexed = true; pc = true; }
		else if (flagValue == "00111100") { simple = true; indexed = true; base = true; }
		else if (flagValue == "00100000") { indirect = true; }
		else if (flagValue == "00100001") { indirect = true; format4 = true; }
		else if (flagValue == "00100010") { indirect = true; pc = true; }
		else if (flagValue == "00100100") { indirect = true; base = true; }
		else if (flagValue == "00010000") { immediate = true; }
		else if (flagValue == "00010001") { immediate = true; format4 = true; }
		else if (flagValue == "00010010") { immediate = true; pc = true; }
		else if (flagValue == "00010100") { immediate = true; base = true; }

		LoadOutput(i, current);
	}
};

/*
 * LoadOutput(int, int) : Begins initial Output
 */
void Disassembler::LoadOutput(int i, int current) {

    cout << "\nLoad Output\n" << endl;

    if (checkBase) {
        string symbol;
        output[outputSize][0] = " ";
        output[outputSize][1] = " ";
        output[outputSize][2] = "BASE";
        if (output[outputSize-1][3].front() == '#' || output[outputSize-1][3].front() == '@')
           symbol = output[outputSize-1][3].substr(1,output[outputSize-1][3].length()-1);
        output[outputSize][3] = symbol;
        output[outputSize][4] = " ";
        outputSize++;
        checkBase = false;
        return;
    }

    if (literal && !litorg) {
        string symbol;
        output[outputSize][0] = " ";
        output[outputSize][1] = " ";
        output[outputSize][2] = "LTORG";
        output[outputSize][3] = " ";
        output[outputSize][4] = " ";
        literal = false;
        litorg = true;
        outputSize++;
    }

    // Check Format 2
    for (int k = 0; !mnemonics2[k].empty(); k++) {
        if (GrabInstruction(current) == mnemonics2[k]) {
            cout << "\t\tFormat 2" << endl;
            format2 = true;
        }
    }

    LoadMemory(i, current);

    LoadSymbol(current);

    if (lit) { 
        outputSize++;
        return; 
    }

    LoadOperand(current);
    
    LoadTAS(current);

    LoadOBJ(current);

    cout << "\tCurrent Ouput At Row: " << outputSize << endl;
    cout << left << setw(12) << output[outputSize][0] << setw(12) << output[outputSize][1] << setw(12) << output[outputSize][2] << setw(12) << output[outputSize][3] << output[outputSize][4] << endl;
    cout << "\tNew TXTRecord: " << txtRecord[current] << endl;

    outputSize++;

    // Check For Symbol Input

};

/*
 * FinishOutput() : Adds END line to output
 */
void Disassembler::FinishOutput() {

    output[outputSize][0] = " ";
    output[outputSize][1] = " ";
    output[outputSize][2] = "END";
    output[outputSize][3] = programName;
    output[outputSize][4] = " ";
};

/*
 * FinalOutput() : Completes final output
 */
void Disassembler::FinalOutput() {
    
    cout << outputSize << " " << symTableSize << " " << endl;
    finalSize = 0; 
    int i = 0, j = 0;
    
    finalOutput[finalSize] = output[i];
    finalSize++;
    i++;

    while (true) {
        
        cout << "FinalSize: " << finalSize << endl;
        string outAddr, symAddr, litAddr;

        if (output[i][0] <= symTable[j][1] || output[i][0] == " " || symTable[j][0].empty()) {
            finalOutput[finalSize] = output[i];
            for (int m = 0; m < symTableSize; m++) {
                if (finalOutput[finalSize][1] == symTable[m][0]) {
                    j++;
                    break;
                }
            }
            i++;
        }
        else if (output[i][0] >= symTable[j][1]) {
            finalOutput[finalSize][0] = symTable[j][1];
            finalOutput[finalSize][1] = symTable[j][0];
            finalOutput[finalSize][2] = "RESB";
            if (symTable[j+1][1] < output[i][0] || output[i][0].empty()) {
                finalOutput[finalSize][3] = to_string(HexToDec(symTable[j+1][1]) - HexToDec(symTable[j][1]));
            } else {
                finalOutput[finalSize][3] = to_string(HexToDec(output[i][0]) - HexToDec(symTable[j][1]));
            }
            finalOutput[finalSize][4] = " ";
            j++;
        }

        cout << "Symbol" << finalOutput[finalSize][2] <<  endl;
        if (finalOutput[finalSize][2] == "END") { 

            if (j < symTableSize) {
                
                for (int m = j; m < symTableSize; m++, finalSize++) {
                    finalOutput[finalSize][0] = symTable[m][1];
                    finalOutput[finalSize][1] = symTable[m][0];
                    finalOutput[finalSize][2] = "RESB";
                    if (m + 1 < symTableSize) {
                        finalOutput[finalSize][3] = to_string(HexToDec(symTable[m+1][1]) - HexToDec(symTable[m][1]));
                    } else {
                        finalOutput[finalSize][3] = "3";
                    } 
                    finalOutput[finalSize][4] = " ";
                }

                
                finalOutput[finalSize][0] = " ";
                finalOutput[finalSize][1] = " ";
                finalOutput[finalSize][2] = "END";
                finalOutput[finalSize][3] = programName;
                finalOutput[finalSize][4] = " ";
            }
            break;
        }

        finalSize++;
    }
};

/*
 * LoadMemory(int, int) : Fills Memory column of output
 */
void Disassembler::LoadMemory(int i, int j) {
    // Load Memory Location
    cout << "\tLoad Memory" << setw(8) << "Row: " << outputSize << endl;
    if (i == 0) { output[outputSize][0] = txtStart[j].substr(2,4); }
    else        { output[outputSize][0] = nextAddr; }
    
    if      (format2) { nextAddr = AddHex(output[outputSize][0], "0002");}
    else if (format4) { nextAddr = AddHex(output[outputSize][0], "0004");}
    else { nextAddr = AddHex(output[outputSize][0], "0003");}
};

/*
 * LoadSymbol(int) : Fills Symbol column of output
 */
void Disassembler::LoadSymbol(int current) {
    // Load Symbol
    cout << "\tLoad Symbol" << endl;
    for (int k = 0; k < symTableSize; k++) {

        if (output[outputSize][0] == symTable[k][1]) {
            output[outputSize][1] = symTable[k][0];
            currSymLoc++;
            break;
        }
        else { output[outputSize][1] = " "; }
    }
    for (int k = 0; k < litTableSize; k++) {
        
        if (output[outputSize][0] == litTable[k][3]) {
            output[outputSize][1] = litTable[k][0];

            if (output[outputSize][1] == "") {
                output[outputSize][2] = "*";
            } else {
                output[outputSize][2] = "BYTE";
            }

            output[outputSize][3] = litTable[k][1];

            if (output[outputSize][2] == "*") {
                output[outputSize][4] = litTable[k][1].substr(3, stoi(litTable[k][2]));
            } else {
                output[outputSize][4] = litTable[k][1].substr(2, stoi(litTable[k][2]));
            }

            txtRecord[current].erase(0, stoi(litTable[k][2]));
            nextAddr = AddHex(output[outputSize][0], "000" + to_string(stoi(litTable[k][2]) / 2));
            lit = true;
            break;
        }
    }
};

/*
 * LoadOperand(int) : Fills Operand column of output
 */
void Disassembler::LoadOperand(int current) {
    // Load Operand
    cout << "\tLoad Operand" << endl;
    
    output[outputSize][2] = GrabInstruction(current);

    if (output[outputSize][2] == "LDX") { indexAddr = DecToHex(output[outputSize][3]); }
    if (output[outputSize][2] == "LDB") { baseAddr = GrabBase(current);
                                          checkBase = true; }


    if (format4) { output[outputSize][2].insert(0, "+"); }
};

/*
 * LoadTAS(int) : Fills TAS column of output
 */
void Disassembler::LoadTAS(int current) {
    // Load TAS
    cout << "\tLoad TAS" << endl;
    output[outputSize][3] = "0";

    if (format2) { output[outputSize][3] = specials[txtRecord[current].at(2) - '0']; }

        // Search For Symbol
    else {output[outputSize][3] = GrabSymbol(current);}
    
    if (!indexed && !base && !pc && !format4 && immediate) {
        output[outputSize][3] = to_string(HexToDec(txtRecord[current].substr(3,3)));
    }

    if (indirect) { output[outputSize][3].insert(0, "@"); }
    else if (immediate) { output[outputSize][3].insert(0, "#"); }
    else if (indexed) { output[outputSize][3].append(",X"); }
};

/*
 * LoadOBJ(int) : Fills OBJ column of output
 */
void Disassembler::LoadOBJ(int current) {
    // Load OPJ Code
    cout << "\tLoad OPJ Code" << endl;
    if(format2) { output[outputSize][4] = txtRecord[current].substr(0,4); }
    else if (format4) { output[outputSize][4] = txtRecord[current].substr(0,8); }
    else { output[outputSize][4] = txtRecord[current].substr(0,6); }

    if(format2) { txtRecord[current].erase(0,4); }
    else if (format4) { txtRecord[current].erase(0,8); }
    else { txtRecord[current].erase(0,6); }
};

/*
 * GrabInstructions(int) : Returns the instruction
 */
string Disassembler::GrabInstruction(int current) {

    cout << "\t\tGrab Instructions" << endl;
    
    string hex = HexToBin(txtRecord[current].substr(0,2));
    string instrValue = MaskOP(hex);

    int i = 0;
    while (!ops[i].empty()) {
        if (instrValue == HexToBin(ops[i].c_str())) {
            cout << "\t\t\t\tFound" << endl;
            return mnemonics[i];
        }
        i++;

    }
    return "ERROR";
};

/*
 * GrabSymbal(int) : Returns the symbol
 */
string Disassembler::GrabSymbol(int current) {

    string record = "", pcAddr = "";
    if (format4) { record = txtRecord[current].substr(4,4); }
    else if (!base && !pc) { return "0"; }
    else { 

        record = txtRecord[current].substr(3,3);
        if (pc) {
            pcAddr = AddHex(output[outputSize][0], "0003");
            record = AddHex(record, pcAddr);
            record = record.replace(0, 1, "0");
            cout << "\t\t\trecord: " << record << endl;
        }
        else if (base) {
            record = AddHex(record, baseAddr);
        }

        if (indexed) { 
            record = AddHex(record, indexAddr); 
            /* moveIndex = true; */
            /* for (int i = 0; i < symTableSize; i++) { */
            /*     if (record == symTable[i][1] && symTable[i][0].substr(0,5) == "TABLE") { */
            /*         indexAddr = AddHex(record, "1770"); */
            /*     } */
            /* } */
        }

        
    }
    
    for (int i = 0; i < symTableSize; i++) {
        if (record == symTable[i][1]) { 
            cout << "Returned Symbol" << endl;
            return symTable[i][0];
        }
    }
    for (int i = 0; i < litTableSize; i++) {
        if (record == litTable[i][3]) { 
            cout << "Returned Literal" << endl;
            if (litTable[i][0] != "") {
                return litTable[i][0]; 
            }
            else {
                literal = true;
                return litTable[i][1];
            }
        }
    }

};

string Disassembler::GrabBase(int current) {

    string record = "";

    if (pc) {
        record = txtRecord[current].substr(3,3);
        if (pc) {
            string pcAddr = AddHex(output[outputSize][0], "0003");
            record = AddHex(record, pcAddr);
            record = record.replace(0, 1, "0");
        }
    }
    else if (base) {
        record = AddHex(record, baseAddr);
    }
    else if (format4) {
        record = txtRecord[current].substr(4,4);
    }

    cout << "\t\tCurrent Base: " << record << endl;

    return record;
};

void Disassembler::ResetFlags() {
    
	simple = false;
	indirect = false;
	immediate = false;
	indexed = false;
	pc = false;
	base = false;
	format2 = false;
	format4 = false;
    lit = false;

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

void Disassembler::binMap(unordered_map<string, char> *um) {
    (*um)["0000"] = '0';
    (*um)["0001"] = '1';
    (*um)["0010"] = '2';
    (*um)["0011"] = '3';
    (*um)["0100"] = '4';
    (*um)["0101"] = '5';
    (*um)["0110"] = '6';
    (*um)["0111"] = '7';
    (*um)["1000"] = '8';
    (*um)["1001"] = '9';
    (*um)["1010"] = 'A';
    (*um)["1011"] = 'B';
    (*um)["1100"] = 'C';
    (*um)["1101"] = 'D';
    (*um)["1110"] = 'E';
    (*um)["1111"] = 'F';
};

string Disassembler::BinToHex(string bin) {

    int l = bin.size();
    int t = bin.find_first_of('.');

    for (int i = 1; i <= (4 - l % 4) % 4; i++) {
        bin = '0' + bin;
    }

    unordered_map<string, char> binhexMap;
    binMap(&binhexMap);

    int i = 0;
    string hex = "";

    while (i != bin.size()) {
        hex += binhexMap[bin.substr(i,4)];
        i += 4;
    }

    return hex;

}

string Disassembler::HexToBin(string hex) {
	int i = 0;
	string bin = "";
	while (hex[i]) {
		switch (hex[i]) {
			case '0':
				bin += "0000";
				break;
			case '1':
				bin += "0001";
				break;
			case '2':
				bin += "0010";
				break;
			case '3':
				bin += "0011";
				break;
			case '4':
				bin += "0100";
				break;
			case '5':
				bin += "0101";
				break;
			case '6':
				bin += "0110";
				break;
			case '7':
				bin += "0111";
				break;
			case '8':
				bin += "1000";
				break;
			case '9':
				bin += "1001";
				break;
			case 'A':
			case 'a':
				bin += "1010";
				break;
			case 'B':
			case 'b':
				bin += "1011";
				break;
			case 'C':
			case 'c':
				bin += "1100";
				break;
			case 'D':
			case 'd':
				bin += "1101";
				break;
			case 'E':
			case 'e':
				bin += "1110";
				break;
			case 'F':
			case 'f':
				bin += "1111";
				break;
		}
		i++;
	}
	return bin;
};

string Disassembler::MaskFlag(string value) {
	string mask = "00111111";
	string newValue = "";
	for(int i = 0; i < (int)value.length(); i++) {
		newValue += (char)(((mask[i] - '0') & (value[i] - '0')) + '0');
	}
	return newValue;
};

string Disassembler::MaskOP(string value) {
	string mask = "11111100";
	string newValue = "";
	for(int i = 0; i < (int)value.length(); i++) {
		newValue += (char)(((mask[i] - '0') & (value[i] - '0')) + '0');
	}
	return newValue;
};

string Disassembler::MaskREC(string value) {
	string mask = "00000111";
	string newValue = "";
	for(int i = 0; i < (int)value.length(); i++) {
		newValue += (char)(((mask[i] - '0') & (value[i] - '0')) + '0');
	}
	return newValue;
};

string Disassembler::DecToHex(string a) {

    map<int, char> k = dec_hex();

    int aLen = a.length();
    int i,j;

    string ans = "";

    for (i = aLen - 1; i >= 0; i--) {

        int num = a[i] - '0';
        int addBit = k[num % 16];

        ans.push_back(addBit);
    }
    
    reverse(ans.begin(), ans.end());

    return ans;
};

string Disassembler::AddHex(string a, string b) {

    map<char, int> m = hex_dec();
    map<int, char> k = dec_hex();

    if (a.length() < b.length()) { swap(a,b); }

    int aLen = a.length(), bLen = b.length();

    string ans = "";

    int carry = 0, i, j;

    for (i = aLen - 1, j = bLen - 1; j >= 0; i--, j--) {

        int sum = m[a[i]] + m[b[j]] + carry;
        int addBit = k[sum % 16];

        ans.push_back(addBit);
        carry = sum / 16;

    }

    while (i >= 0) {

        int sum = m[a[i]] + carry;
        int addBit = k[sum % 16];

        ans.push_back(addBit);
        carry = sum / 16;
        i--;
    }

    if (carry) { ans.push_back(k[carry]); }

    reverse(ans.begin(), ans.end());

    return ans;
}
