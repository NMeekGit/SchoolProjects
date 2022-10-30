#include "disassembler.h"

int main(int argc, char** argv) {

    Disassembler g;

    for (int i = 1; i < argc; i + 2) {

        string fileOBJ = argv[i];
        string fileSYM = argv[i+1];

        cout << "Files Entered:" << setw(12) << fileOBJ << setw(12) << fileSYM << endl;

        cout << "Attempt Open" << endl;
        g.OpenFile(i, fileOBJ);

        cout << "Attempt Print" << endl;
        g.PrintFile();
    }
}