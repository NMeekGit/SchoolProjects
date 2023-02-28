#include <stdio.h>
#include <pthread.h>

#include "shared.h"
#include "populatetree.h"
#include "readprefix.h"
#include "countprefix.h"

#define MIN_NUM_OF_ARGS 3
#define VOCAB_FILE_ARG_POS 1
#define TEST_FILE_ARG_POS 2

using namespace std;

int main (int argc, char** argv) {
    bool verbose = false;
    int option;
    int idx;
    int count;
    int hashInterval = DEFAULT_HASH_MARK_INTERVAL;
    int numOfMarks = DEFAULT_NUM_OF_MARKS;
    int minNumForPrint = DEFAULT_MIN_NUM_OF_WORDS_WITH_A_PREFIX;
    string vocabFile;
    string testFile;

    if (argc < MIN_NUM_OF_ARGS) {

        printf("Not Enough Arguements\n");
        printf("i.e. ./crunch <<vocab file.txt>> <<test file.txt>> [[--args]]\n");
        return 1;

    }

    while ((option = getopt(argc, argv, "p:h:n:")) != -1) {

        switch (option) {
            case 'p':
                numOfMarks = optarg;
                break;
            case 'h':
                hashInterval = optarg;
                break;
            case 'n':
                minNumForPrint = optarg;
                break;
            default:
                cout << "no args" << endl;
                exit(2);
        }
    }

    idx = optind;

    if (idx < argc) {

        /* Process positional arguments */
        
        if (idx == VOCAB_FILE_ARG_POS) {
            vocabFile = argv[idx];
        }

        idx ++;

        if (idx == TEST_FILE_ARG_POS) {
            testfile = argv[idx];
        }

        idx ++;
    }

    SHARED_DATA *sharedData = (SHARED_DATA*)malloc(sizeof(SHARED_DATA));

    sharedData->root = createDictNode();
    sharedData->numOfProgressMarks = numOfMarks;
    sharedData->hashmarkInterval = hashInterval;
    sharedData->minNumOfWordsWithAPrefixForPrinting = minNumForPrint;
    sharedData->taskCompleted[DICTSRCFILEINDEX] = verbose;
    sharedData->taskCompleted[TESTFILEINDEX] = verbose;


    /*! TODO: initializez the sharedData, maybe with a helper
     *  \todo 
     */

    /* populatetree thread
     *  - thread process to read in a vocab file to create a dictionary tree
     */
    pthread_t populateTreeThread;

    /* readprefix thread
     *  - thread process to read in a test file and fill a queue of prefixes
     */
    pthread_t readPrefixThread;
    
    /* countprefix thread
     *  - thread process to dequeue the prefixes from the readprefix thread
     *    and count the number of words that contain the prefix
     */
    pthread_t countPrefixThread;

    if (pthread_create(&populateTreeThread, NULL, &populateTree, &sharedData)) {
        /*! TODO: Error handling
         *  \todo 
         */
    }
    /*if (pthread_create(&readPrefixThread, NULL, &readprefix, &sharedData)) { */
    /*    /1*! TODO: Error handling */
    /*     *  \todo */ 
    /*     *1/ */
    /*} */
    /*if (pthread_create(&countPrefixThread, NULL, &countprefix, &sharedData)) { */
    /*    /1*! TODO: Error handling */
    /*     *  \todo */ 
    /*     *1/ */
    /*} */

    /*! TODO: Create progress bar for populate and count
     *  \todo 
     */

    /* populatetree thread progress bar */
    count = 0;
    double step = 1/sharedData->numOfProgressMarks;
    double currentStep = step;

    
    while (!sharedData->taskCompleted[DICTSRCFILEINDEX]) {

        double percent = ((double)sharedData->numOfCharsReadFromFile[DICTSRCFILEINDEX] / (double)sharedData->totalNumOfCharsInFile[DICTSRCFILEINDEX]) * 100;
        
        if (percent >= currentStep) {
            if (count == sharedData->hashmarkInterval) {
                cout << "#";
            } else {
                cout << "-";
            }
            count++;
            currentStep += step;
        }
    }

    double percent = ((double)sharedData->numOfCharsReadFromFile[DICTSRCFILEINDEX] / (double)sharedData->totalNumOfCharsInFile[DICTSRCFILEINDEX]) * 100;
    }
    cout << "\n There are " << sharedData->wordCountInFile[DICTSRCFILEINDEX] << " words in vocabulary.txt." << endl;


}
