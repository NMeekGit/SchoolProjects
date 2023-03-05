#ifndef SHARED_H
#define SHARED_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <cstring>
#include <array>
#include <bits/stdc++.h>

#define NUMOFFILES 2
#define DICTSRCFILEINDEX 0
#define TESTFILEINDEX 1

#define DEFAULT_NUM_OF_MARKS 50

#define MIN_NUM_OF_MARKS 10

#define DEFAULT_HASH_MARK_INTERVAL 10

#define DEFAULT_MIN_NUM_OF_WORDS_WITH_A_PREFIX 1

#define NULL_CHAR '\0'
#define NULL_POS 29
#define NCHILD 30

using namespace std;

typedef struct dictNode {
    struct dictNode* next[NCHILD]; // a-z,',-,_,NULL_CHAR
};

typedef struct SHARED_DATA {

    /* character map for dictionary tree */
    static const map<char, int> validCharacters;

    /* Separator for parsing */
    static const char *separators;

    /* Root Dictionary Node */
    dictNode *root;

    /* parameters for printing progress bar */
    int numOfProgressMarks;
    int hashmarkInterval;

    /* print the prefix and its count to the output file only if the
     * number of dictionary words starting from the prefix is equal to 
     * or greater than this number
     */
    int minNumOfWordsWithAPrefixForPrinting;

    /* filePath[0]
     *  - file path for the dictionary vocabulary file providing words
     *    to populate the dictionary tree
     * filePath[1]
     *  - file path for the test source file providing prefixes
     */
    const char *filePath[NUMOFFILES];

    /* store total number of characters in files
     *
     * totalNumOfCharsInFile[DICTSRCFILEINDEX]
     *  - number of total chars in the dictionary vocab file
     *
     * totalNumOfCharsInFile[TESTFILEINDEX]
     *  - number of total chars in the test file
     */
    long totalNumOfCharsInFile[NUMOFFILES];

    /* numOfCharsProcessedFromFile
     *  - used to track ongoing progress of number of characters 
     *    read in from files and the subsequent thread logic
     *
     * numOfCharsReadFromFile[DICTSRCFILEINDEX]
     *  - number of chars read in and processed from the dictionary vocab file
     *
     * numOfCharsReadFromFile[TESTFILEINDEX]
     *  - number of chars read in and processed from the test file
     */
    long numOfCharsReadFromFile[NUMOFFILES];

    /* wordCountInFile[DICTSRCFILEINDEX]
     *  - number of total words in the dictionary vocab file
     *
     * wordCountInFile[TESTFILEINDEX]
     *  - number of total words in the test file
     */
    long wordCountInFile[NUMOFFILES];

    /* number of prefixes that have been read from the prefix queue */
    long numOfProcessedPrefixes;

    /* buffer prefixes read from test file */
    queue<string> prefixQueue;

    /* pthread mutex for accessing the prefixQueue */
    pthread_mutex_t queue_mutex;

    /* completed flags indicate the completion of the thread logic
     *
     * taskCompleted[DICTSRCFILEINDEX]
     *  - boolean flag to indicate whether the tree population thread
     *    has completed the task
     *
     * taskCompleted[TESTFILEINDEX]
     *  - boolean flag to indicate whether the readprefix thread has completed the task
     */
    bool taskCompleted[NUMOFFILES];
};

#endif
