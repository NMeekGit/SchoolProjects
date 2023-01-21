/*
 * driver.cpp | Assignment 1
 *
 * @class  CS 480
 * @author Noah Meeker
 * @REDID  821272563
 */

#include "countprefix.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "[ERROR] Code requires two parameters" << endl;
        cout << "   ex: ./{program} {vocab list}.txt {test file}.txt\n" << endl;
        cout << "***SYSTEM EXIT FAILURE***" << endl;
        return EXIT_FAILURE;
    }

    string DICTIONARY_FILE = argv[1];
    ifstream stream (DICTIONARY_FILE);
    ofstream output ("testoutput.txt");
    string line;
    if (stream.is_open()) {
        while (getline(stream, line)) {
            output << line << endl;
        }
    }

    stream.close();
    output.close();

    cout << "***SYSTEM EXIT SUCCESS***" << endl;
    return EXIT_SUCCESS;
}
