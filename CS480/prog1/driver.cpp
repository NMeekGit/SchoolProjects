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
    /* Check proper amount of arguements passed */
    if (argc != 3) {
        return EXIT_FAILURE;
    }

    string DICTIONARY_FILE = argv[1];
    string line;
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";
    Dict dictionary;

    /* Opening Vocabulary File */
    ifstream dictStream (DICTIONARY_FILE);
    if (dictStream.is_open()) {
        while (getline(dictStream, line)) {

            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char *word = strtok(line_c, delimiters);
            while (word) {
                if (!dictionary.add(word)) {

                    // If word from vocabulary file could not be added to the dictionary
                    return EXIT_FAILURE;
                }
                word = nullptr;
            }
            delete[] line_c;
        }
    }

    dictStream.close();

    string TEST_FILE = argv[2];
    dictNode* preffix;

    /* Opening Document for Searching and counting */
    ifstream testStream (TEST_FILE);
    if (testStream.is_open()) {
        while (getline(testStream, line)) {
            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char* word = strtok(line_c, delimiters);
            while (word) {
                int count = 0;
                preffix = dictionary.findEndingNodeOfStr(word);
                dictionary.countWordsStartingFromNode(preffix, count);
                string str = word;
                cout << str << " " << count << endl;

                // Move on to next word
                word = strtok(NULL, delimiters);
            }
        }
    }

    testStream.close();

    /* Program Successful */
    return EXIT_SUCCESS;
};

