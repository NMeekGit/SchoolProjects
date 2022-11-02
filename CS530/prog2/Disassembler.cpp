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

    moveIndex = false;
    checkBase = false;
    currSymLoc = 0;
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
		for (int i = 0; i <= outputSize; i++) {

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

    for (int currentTXT = 0; currentTXT < txtRecordSize; currentTXT++) {
        FindFlags(currentTXT);
    }

    FinishOutput();
};

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
        symTable[symTableSize][1] = addr.substr(2,4);
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

void Disassembler::LoadOutput(int i, int current) {

    cout << "\nLoad Output\n" << endl;

    int j = current;
    bool lit = false;

    if (checkBase) {
        string symbol;
        output[outputSize][0] = " ";
        output[outputSize][1] = " ";
        output[outputSize][2] = "BASE";
        if (output[outputSize-1][3].front() == '#' || output[outputSize-1][3].front() == '@')
           symbol = output[outputSize-1][3].erase(0,1);
        output[outputSize][3] = symbol;
        output[outputSize][4] = " ";
        outputSize++;
        checkBase = false;
        return;
    }

    // Load Memory Location
    cout << "\tLoad Memory" << setw(8) << "Row: " << outputSize << endl;
    if (i == 0) { output[outputSize][0] = txtStart[j].substr(2,4); }
    else        { output[outputSize][0] = nextAddr; }
    
    if      (format2) { nextAddr = AddHex(output[outputSize][0], "0002");}
    else if (format4) { nextAddr = AddHex(output[outputSize][0], "0004");}
    else { nextAddr = AddHex(output[outputSize][0], "0003");}
    j++;

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
            lit = true;
            break;
        }
        else { output[outputSize][1] = " "; }
    }

    // Load Operand
    cout << "\tLoad Operand" << endl;
    if (lit) {
        output[outputSize][2] = "BYTE";
    }
    else {
        output[outputSize][2] = GrabInstruction(current);
    }

    if (output[outputSize][2] == "LDX") { indexAddr = output[outputSize][0]; }
    else if (output[outputSize][2] == "LDB") { 
        baseAddr = output[outputSize][0]; 
        checkBase = true;
    }

    if (format4) { output[outputSize][2].insert(0, "+"); }
    
    // Check Format 2
    for (int k = 0; !mnemonics2[k].empty(); k++) {
        if (output[outputSize][2] == mnemonics2[k]) {
            cout << "\t\tFormat 2" << endl;
            format2 = true;
        }
    }

    // Load TAS
    cout << "\tLoad TAS" << endl;
    output[outputSize][3] = "0";
    
    // Search For Symbol
    output[outputSize][3] = GrabSymbol(current);
    if (indirect) { output[outputSize][3].insert(0, "@"); }
    else if (immediate) { output[outputSize][3].insert(0, "#"); }
    else if (indexed) { output[outputSize][3].append(",X"); }

    // Load OPJ Code
    cout << "\tLoad OPJ Code" << endl;
    if(format2) { output[outputSize][4] = txtRecord[current].substr(0,4); }
    else if (format4) { output[outputSize][4] = txtRecord[current].substr(0,8); }
    else { output[outputSize][4] = txtRecord[current].substr(0,6); }

    if(format2) { txtRecord[current].erase(0,4); }
    else if (format4) { txtRecord[current].erase(0,8); }
    else { txtRecord[current].erase(0,6); }

    cout << "\tCurrent Ouput At Row: " << outputSize << endl;
    cout << left << setw(12) << output[outputSize][0] << setw(12) << output[outputSize][1] << setw(12) << output[outputSize][2] << setw(12) << output[outputSize][3] << output[outputSize][4] << endl;
    cout << "\tNew TXTRecord: " << txtRecord[current] << endl;

    outputSize++;
};

void Disassembler::FinishOutput() {

    for (int i = currSymLoc; i < symTableSize; i++, outputSize++) {
        output[outputSize][0] = symTable[i][1];
        output[outputSize][1] = symTable[i][0];
        output[outputSize][2] = "RESB";
        output[outputSize][3] = "0";
        output[outputSize][4] = " ";
    }

    output[outputSize][0] = " ";
    output[outputSize][1] = " ";
    output[outputSize][2] = "END";
    output[outputSize][3] = programName;
    output[outputSize][4] = " ";
};

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
        }
        else if (base) {
            record = AddHex(record, baseAddr);
        }

        if (moveIndex && indexed) {
            record = indexAddr;
        }
        else if (indexed) { 
            record = AddHex(record, indexAddr); 
            moveIndex = true;
            for (int i = 0; i < symTableSize; i++) {
                if (record == symTable[i][1] && symTable[i][0].substr(0,5) == "TABLE") {
                    indexAddr = AddHex(record, "1770");
                }
            }
        }

        
    }
    
    for (int i = 0; i < symTableSize; i++) {
        if (record == symTable[i][1]) { return symTable[i][0]; }
    }
    for (int i = 0; i < litTableSize; i++) {
        if (record == litTable[i][3]) { return litTable[i][1]; }
    }

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
