/*
 * driver.cpp | Assignment 1
 *
 * @class  CS 480
 * @author Noah Meeker
 * @REDID  821272563
 */

#include "dictionary.h"

int main(int argc, char **argv)
{
    if (argc != 3) {
        cout << "[ERROR] Code requires two parameters" << endl;
        cout << "   ex: ./{program} {vocab list}.txt {test file}.txt\n" << endl;
        cout << "***SYSTEM EXIT FAILURE***" << endl;
        return EXIT_FAILURE;
    }

    string DICTIONARY_FILE = argv[1];
    ifstream stream (DICTIONARY_FILE);
    ofstream output ("testoutput.txt");
    string line;
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";
    Dict dictionary;
    if (stream.is_open()) {
        while (getline(stream, line)) {

            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char *word = strtok(line_c, delimiters);
            /* for (int i = 0; i < line.length() +1; ++i) { */
            /*     cout << word[i] << ","; */
            /* } */
            /* cout << "" << endl; */
            while (word != nullptr) {
                if (!dictionary.add(word)) {
                    cout << "[ERROR] Word Not Entered" << endl;
                    cout << "***SYSTEM EXIT FAILURE***" << endl;
                    return EXIT_FAILURE;
                }
                word = nullptr;
            }
            cout << "[Dictionary] Word Inserted" << endl;
            delete[] line_c;
        }
            cout << "[Dictionary] Filling of Dictionary Completed" << endl;
    }

    stream.close();

    cout << "***SYSTEM EXIT SUCCESS***" << endl;
    return EXIT_SUCCESS;
}
