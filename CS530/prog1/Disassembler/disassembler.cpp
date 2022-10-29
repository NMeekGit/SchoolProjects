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

const string Disassembler::formats[] = {
	"2","3","4"
};

const string Disassembler::oams[] = {
	"simple","immediate","indirect"
};

const string Disassembler::taams[] = {
	"pc","base","absolute","pc_indexed","base_indexed"
};

const int Disassembler::PRIME = 47;

int Disassembler::MAX_SIZE = 100;

Disassembler::Disassembler() {
	initColumns();
};

Disassembler::~Disassembler() {
	delete[] input;
	delete[] instr;
	delete[] format;
	delete[] oam;
	delete[] taam;
	delete[] obj;
};

void Disassembler::openFile(int index, string file) {
	this->filename = file;
	inputSize = 0;
	input = new string[MAX_SIZE];
	string line;
	ifstream fileOBJ (filename);
	if (fileOBJ.is_open()) {
		//cout << "fileOBJ is open" << endl;
		int i = 0;
		while (getline(fileOBJ,line)) {
			input[i] = line;
			i++;
			inputSize++;
		}
		fileOBJ.close();
	}
	//cout << "input[] filed\n" << endl;
	//cout << "Current Input[]\n" << endl;
	for (int i = 0; i <= inputSize; i++) {
		cout << input[i] << endl;
	}	
	//cout << "Size: " << inputSize << "\n" << endl;
	grabTXT();
};

void Disassembler::printFile() {
	this->filename = this->filename.substr(0,filename.find_last_of('.')) + ".txt";
	ofstream fileTXT (filename);
	if (fileTXT.is_open()) {
		for (int i = 0; i < rowSize; i++) {
			fileTXT << left << setw(16) << instr[i] << setw(16) << format[i] << setw(16) << oam[i] << setw(16) << taam[i] << setw(16) << obj[i] << endl;
		}
		fileTXT.close();
		//cout << "File Printed" << endl;
	}
};

void Disassembler::grabTXT() {
	//cout << "Grabbing TEXT Record" << endl;
	for (int i = 0; i <= inputSize; i++) {
		string line = input[i];
		if (line[0] == 'T') {
			txtRecord = input[i];
			//cout << "TEXT Record Recieved:" << endl;
			//cout << txtRecord << endl;
		}
	}
	grabTXTInfo();
};

void Disassembler::grabTXTInfo() {
	//cout << "Grabbing TEXT Info" << endl;
	txtSize = HexToDec(txtRecord.substr(7,2));
	//cout << "Size of " << txtRecord.substr(7,2) << ": " << txtSize << endl;
	txtRecord = txtRecord.erase(0,9);
	//cout << "New Line: " << txtRecord << endl;

	//cout << "Disassemble" << endl;
	Solve();
};

void Disassembler::Solve() {
	while (txtRecord.length() != 1) {	
		// Check if end of TEXT Record
		if (txtRecord.empty()) { break; }
		// Grab Instruction
		grabInstruction();
		// Find flags
		findFlags();

		rowSize++;
	}
};

void Disassembler::findFlags() {
	string hex = HexToBin(txtRecord.substr(1,2));
	string flagValue = MaskFlag(hex);

	//cout << "HEX: " << hex << " VALUE: " << flagValue << endl;
	bool n = false;
	bool i = false;
	bool x = false;
	bool b = false;
	bool p = false;
	bool e = false;
	bool form2 = false;
	bool form4 = false;

	if(flagValue == "00110000") { n = true; i = true; }
	else if(flagValue == "00110001") { n = true; i = true; e = true; }
	else if(flagValue == "00110010") { n = true; i = true; p = true; }
	else if(flagValue == "00110100") { n = true; i = true; b = true; }
	else if(flagValue == "00111000") { n = true; i = true; x = true; }
	else if(flagValue == "00111001") { n = true; i = true; x = true; e = true; }
	else if(flagValue == "00111010") { n = true; i = true; x = true; p = true; }
	else if(flagValue == "00111100") { n = true; i = true; x = true; b = true; }
	else if(flagValue == "00100000") { n = true; }
	else if(flagValue == "00100001") { n = true; e = true; }
	else if(flagValue == "00100010") { n = true; p = true; }
	else if(flagValue == "00100100") { n = true; b = true; }
	else if(flagValue == "00010000") { i = true; }
	else if(flagValue == "00010001") { i = true; e = true; }
	else if(flagValue == "00010010") { i = true; p = true; }
	else if(flagValue == "00010100") { i = true; b = true; }

	string mode;
	string tMode;
	string oMode;

	// Checking OAM
	// "simple","immediate","indirect"
	if (n && !(i))	  					{ mode = oams[2]; }
	else if (!(n) && i)					{ mode = oams[1]; }
	else if ((n && i) || !(n && i))  	{ mode = oams[0]; }

	// Checking TAAM
	// "pc","base","absolute","pc_indexed","base_indexed"
	if (p) 				{ tMode = taams[0]; }
	else if (b) 		{ tMode = taams[1]; }
	else if (!b && !p)	{ tMode = taams[2]; }

	// Checking Indexed
	if (p && x) 		{ tMode = taams[3]; }
	else if (b && x) 	{ tMode = taams[4]; }

	// Checking Format 4
	if (e && !b && !p)	{ form4 = true; }

	/****** Push modes into their respected Columns ******/
	
	// Formats
	// Check Format 2: If form2 = FALSE, then it is Format 3/4
	form2 = checkFormat2();

	if (form2) {
		format[rowSize] = formats[0];
	}
	else if (form4) {
		format[rowSize] = formats[2];
	}
	else {
		format[rowSize] = formats[1];
	}	

	// OAMS
	oam[rowSize] = mode;

	// TAAM || Indexed
	taam[rowSize] = tMode;

	// OBJ
	if (form2) {
		oam[rowSize] = " ";
		taam[rowSize] = " ";
		obj[rowSize] = txtRecord.substr(0,4);
		txtRecord = txtRecord.erase(0,4);
		form2 = false;
	}
	else if (form4) {
		obj[rowSize] = txtRecord.substr(0,8);
		txtRecord = txtRecord.erase(0,8);
		form4 = false;
	}
	else {
		obj[rowSize] = txtRecord.substr(0,6);
		txtRecord = txtRecord.erase(0,6);
	}

	//cout << "Row " << rowSize << ":" << setw(16) << instr[rowSize] << setw(16) << format[rowSize] << setw(16) << oam[rowSize] << setw(16) << taam[rowSize] << setw(16) << obj[rowSize] << endl;
};

void Disassembler::grabInstruction() {
	string hex = HexToBin(txtRecord.substr(0,2));
	string instrValue = MaskOP(hex);
	//cout << "instrHEX: " << hex << " instrVALUE: " << instrValue << endl;
	int i = 0;
	while (!ops[i].empty()) {
		if (instrValue == HexToBin(ops[i].c_str())) {
			instr[rowSize] = mnemonics[i];
			break;
		}
		i++;
	}
};

void Disassembler::initColumns() {
	rowSize = 0;
	instr = new string[MAX_SIZE];
	format = new string[MAX_SIZE];
	oam = new string[MAX_SIZE];
	taam = new string[MAX_SIZE];
	obj = new string[MAX_SIZE];
	instr[rowSize] = "INSTR";
	format[rowSize] = "FORMAT";
	oam[rowSize] = "OAM";
	taam[rowSize] = "TAAM";
	obj[rowSize] = "OBJ";
	rowSize++;
};

bool Disassembler::checkFormat2() {
	int i = 0;
	while (!mnemonics2[i].empty()) {
		if (instr[rowSize] == mnemonics2[i]) {
			return true;
		}
		i++;
	}
	return false;
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

string Disassembler::MaskOP(string value) {
	string mask = "11111100";
	string newValue = "";
	for(int i = 0; i < (int)value.length(); i++) {
		newValue += (char)(((mask[i] - '0') & (value[i] - '0')) + '0');
	}
	return newValue;
};

string Disassembler::MaskFlag(string value) {
	string mask = "00111111";
	string newValue = "";
	for(int i = 0; i < (int)value.length(); i++) {
		newValue += (char)(((mask[i] - '0') & (value[i] - '0')) + '0');
	}
	return newValue;
};

