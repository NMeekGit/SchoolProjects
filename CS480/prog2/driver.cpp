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
                /*! TODO: Logic for -p arguement
                    *  \todo 
                    */
                break;
            case 'h':
                /*! TODO: Logic for -h arguement
                 *  \todo 
                 */
                break;
            case 'n':
                /*! TODO: Logic for -n arguement
                 *  \todo 
                 */
                break;
            default:
                /*! TODO: Print something
                 *  \todo 
                 */
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

    SHARED_DATA sharedData;

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
    while ((double)numOfCharsProcessedFromFile[DICTSRCFILEINDEX] / 
            (double)totalNumOfCharsInFile[DICTSRCFILEINDEX] != 1) {
        
        double percent = (double)numOfCharsProcessedFromFile[DICTSRCFILEINDEX] / 
            (double)totalNumOfCharsInFile[DICTSRCFILEINDEX];

        if (percent / shared-> == 0)
           if (count == hashmarkInterval) {
               cout << "#";
           } else {
               cout << "-";
           }
    }
    cout << "\n There are " << wordCountInFile[DICTSRCFILEINDEX] << " words in vocabulary.txt." << endl;


}
