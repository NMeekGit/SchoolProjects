#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <queue>
#include <array>
#include <map>
#include <iostream>
#include <fstream>
#include <pthread.h>

#define NUMOFFILES 2
#define DICTSRCFILEINDEX 0
#define TESTFILEINDEX 1

#define DEFAULT_NUM_OF_MARKS 50
#define MIN_NUM_OF_MARKS 10
#define DEFAULT_HASH_MARK_INTERVAL 10
#define DEFAULT_MIN_NUM_OF_WORDS_WITH_A_PREFIX 1

#define NCHILD 30
#define NULL_CHAR '\0'

using namespace std;

typedef struct dictNode {
    struct dictNode* next[NCHILD];
};

typedef struct SHARED_DATA {

    /* root node of dictionary tree */
    dictNode* root;

    /* parameters for printing progress bar */
    int numOfProgressMarks;
    int hashmarkInterval;

    /* print prefix and its count to the output file only if the number
     * of dictionary words starting from the prefix is equal to or greater than this number
     */
    int minNumOfWordsWithAPrefixForPrinting;

    /*
     * filePath[0] - file path for the dictionary vocab file
     *
     * filePath[1] - file path for the test source file providing prefixes
     */
    const char* filePath[NUMOFFILES];

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
     *  - used to track ongoing progress of number of characters read in files and the subsequent thread logic.
     *
     * This progress indicator is updated by worker threads, and used by the main thread
     * to display the progress for tracking the execution of the populatetree thread
     *
     * numOfCharsReadFromFile[DICTSRCFILEINDEX]
     *  - number of chars read in and processed from the dictioanry vocab file
     *
     * numOfCharsReadFromFile[TESTFILEINDEX]
     *  - number of chars read in and processed form the test file
     */
    long numOfCharsReadFromFile[NUMOFFILES];

    /* number of prefixes that have been read from the prefix queue,
     * searched and printed
     * this is used for tracking the progress of the countprefix thread
     */
    long numOfProcessedPrefixes;

    /* buffer prefixes read from the test file */
    queue<string> prefixQueue;

    /* pthread mutex for accessing the prefixQueue */
    pthread_mutex_t queue_mutex;

    /* completed flags indicate the completion of the thread logic,
     * one for the populatetree thread, one for countprefix thread
     *
     * taskCompleted[DICTSRCFILEINDEX]
     *  - boolean flag to indicate whether the tree population thread has completed the task:
     *    read words from the dictionary source file, and popualte the tree
     *
     * taskCompleted[TESTFILEINDEX]
     *  - boolean flag to indicate whether the readprefix thread has completed the task:
     *    read prefixes from the test file, and insert each prefix to a queue.
     *  - main thread progress display would use this flag to start to show the countprefix progress bar
     */
    bool taskCompleted[NUMOFFILES];
};


#endif
