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
    ifstream dictStream (DICTIONARY_FILE);
    string line;
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";
    Dict dictionary;
    if (dictStream.is_open()) {
        cout << "[Dictionary] Opening Vocab File\n" << endl;
        while (getline(dictStream, line)) {

            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char *word = strtok(line_c, delimiters);
            while (word) {
                if (!dictionary.add(word)) {
                    cout << "[ERROR] Word Not Entered\n" << endl;
                    cout << "***SYSTEM EXIT FAILURE***" << endl;
                    return EXIT_FAILURE;
                }
                word = nullptr;
            }
            delete[] line_c;
        }
            cout << "[Dictionary] Filling of Dictionary Completed\n" << endl;
    }

    dictStream.close();

    string TEST_FILE = argv[2];
    ifstream testStream (TEST_FILE);
    ofstream output ("testoutput.txt");
    dictNode* preffix;

    if (testStream.is_open()) {
        cout << "[Test File] Opening Test File\n" << endl;
        while (getline(testStream, line)) {
            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char* word = strtok(line_c, delimiters);
            while (word) {
                for (int i = 0; i < strlen(word); i++) {
                    cout << word[i] << ",";
                }
                cout << "" << endl;
                preffix = dictionary.findEndingNodeOfStr(word);
                int count = 0;
                dictionary.countWordsStartingFromNode(preffix, count);
                string str = word;
                if (preffix != NULL) {
                    cout << "[Search] " + str + " Found" << endl;
                } else {
                    cout << "[Search] " + str + " Not Found" << endl;
                }
                cout << "[Count] " << count << endl;
                word = strtok(NULL, delimiters);
            }
        }
    }

    testStream.close();
    output.close();

    cout << "***SYSTEM EXIT SUCCESS***" << endl;
    return EXIT_SUCCESS;
};

