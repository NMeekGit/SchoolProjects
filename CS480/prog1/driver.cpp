/*
 * driver.cpp | Assignment 1
 *
 * @class  CS 480
 * @author Noah Meeker
 * @REDID  821272563
 */

#include "dictionary.h"

char** grabWords(char* line) {
    const int BUFFER_SIZE = 100;
    char** array;
    char* word;
    char NULL_TERMINATOR = '\0';
    
    word = new char[BUFFER_SIZE];
    array = new char*[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++) {
        *(array + i) = new char[BUFFER_SIZE];
    }

    int indexLine = 0;
    for (int indexArray = 0; indexArray < BUFFER_SIZE; indexArray++) {
        for (int indexWord = 0; indexWord < BUFFER_SIZE; indexWord++) {
            if (*(line + indexLine) == NULL_TERMINATOR) {
                *(word + indexWord) = *(line + indexLine);
                indexLine++;
                break;
            }
            *(word + indexWord) = *(line + indexLine);
            indexLine++;
        }
        *(array + indexArray) = word;
    }

    return array;
};

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
                word = strtok(NULL, delimiters);
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
        while (getline(testStream, line)) {
            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char* word = strtok(line_c, delimiters);
            while (word) {
                preffix = dictionary.findEndingNodeOfStr(word);
                if (preffix != nullptr) {
                    string str = word;
                    cout << "[Search] " + str + " Found" << endl;
                }
                word = strtok(NULL, delimiters);
            }
        }
    }

    testStream.close();
    output.close();

    cout << "***SYSTEM EXIT SUCCESS***" << endl;
    return EXIT_SUCCESS;
};

