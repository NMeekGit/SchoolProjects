#include "disassembler.h"

int main(int argc, char** argv) {
	Disassembler g;
	for(int i = 1; i < argc; i++) {

		cout << "File Entered: " << argv[i] << endl;

		string file = argv[i];

		cout << "Attempt Open File" << endl;
		g.openFile(i, file);

		cout << "Attempt Print File" << endl;
		g.printFile();
	}
	return 0;
}
