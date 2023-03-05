#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>

#include "shared.h"
#include "populatetree.h"
#include "readprefix.h"
#include "countprefix.h"

#define MIN_NUM_OF_ARGS 3
#define VOCAB_FILE_ARG_POS 1
#define TEST_FILE_ARG_POS 2

using namespace std;

void progressBar(int, SHARED_DATA *);

int main (int argc, char** argv) {
    bool verbose = false;
    int option;
    int idx;
    int hashInterval = DEFAULT_HASH_MARK_INTERVAL;
    int numOfMarks = DEFAULT_NUM_OF_MARKS;
    int minNumForPrint = DEFAULT_MIN_NUM_OF_WORDS_WITH_A_PREFIX;
    char* vocabFile;
    char* testFile;

    struct stat buf;

    if (argc < MIN_NUM_OF_ARGS) {

        printf("Not Enough Arguements\n");
        printf("i.e. ./crunch <<vocab file.txt>> <<test file.txt>> [[--args]]\n");
        return 1;

    }

    while ((option = getopt(argc, argv, "p:h:n:")) != -1) {

        switch (option) {
            case 'p':
                if (atoi(optarg) < MIN_NUM_OF_MARKS) {
                    cout << "Number of progress marks must be a number and at least 10" << endl;
                    exit(2);
                }
                numOfMarks = atoi(optarg);
                break;
            case 'h':
                if (atoi(optarg) < 0 || atoi(optarg) > DEFAULT_HASH_MARK_INTERVAL) {
                    cout << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << endl;
                    exit(2);
                }
                hashInterval = atoi(optarg);
                break;
            case 'n':
                minNumForPrint = atoi(optarg);
                break;
            default:
                cout << "" << endl;
                exit(2);
        }
    }

    idx = optind;

    if (idx < argc) {

        /* Process positional arguments */
        
        vocabFile = argv[idx];

        ifstream checkFile (vocabFile);
        if (!checkFile.is_open()) {
            cout << "Unable to open <<" << vocabFile << ">>" << endl;
            exit(2);
        }
        checkFile.close();

        idx ++;

        testFile = argv[idx];

        checkFile.open(testFile);
        if (!checkFile.is_open()) {
            cout << "Unable to open <<" << testFile << ">>" << endl;
            exit(2);
        }
        checkFile.close();

        idx ++;
    }


    SHARED_DATA *sharedData = (SHARED_DATA*)malloc(sizeof(SHARED_DATA));

    /* Initialize sharedData variable */
    sharedData->root = createDictNode();
    sharedData->numOfProgressMarks = numOfMarks;
    sharedData->hashmarkInterval = hashInterval;
    sharedData->minNumOfWordsWithAPrefixForPrinting = minNumForPrint;
    sharedData->taskCompleted[DICTSRCFILEINDEX] = verbose;
    sharedData->taskCompleted[TESTFILEINDEX] = verbose;
    sharedData->filePath[DICTSRCFILEINDEX] = vocabFile;
    sharedData->filePath[TESTFILEINDEX] = testFile;
    sharedData->prefixQueue = std::queue<std::string>();
    sharedData->queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    /* grab total number of characters from file */
    stat(vocabFile, &buf);
    sharedData->totalNumOfCharsInFile[DICTSRCFILEINDEX] = buf.st_size;

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

    if (pthread_create(&populateTreeThread, NULL, &populatetree, sharedData)) {
        perror("[ERROR] populateTree");
        exit(1);
    }

    if (pthread_create(&readPrefixThread, NULL, &readPrefix, sharedData)) {
        perror("[ERROR] readprefix");
        exit(1);
    }

    if (pthread_create(&countPrefixThread, NULL, &countPrefix, sharedData)) {
        perror("[ERROR] countprefix");
        exit(1);
    }

    /* Print progress bar for PopulateTree Thread */
    progressBar(DICTSRCFILEINDEX, sharedData);


    /* wait for ReadPrefix Thread to finish */
    pthread_join(readPrefixThread, NULL);

    /* Print progress bar for CountPrefix Thread */
    progressBar(TESTFILEINDEX, sharedData);

    return 0;

}

void progressBar(int fileIdx, SHARED_DATA *shared) {
    /* populatetree thread progress bar */
    double percent = 0.0;
    int pos;
    int lastPos = 0;
    int intervalCount = 1;
    fflush(stdout);

    while (percent < 1.0) {
        int width = shared->numOfProgressMarks;
        pos = (width * percent);

        /* Check if there is progress */
        if (pos > lastPos) {
            if (intervalCount % shared->hashmarkInterval == 0) {
                cout << "#";
            } else {
                cout << "-";
            }
            intervalCount++;
        }

        /* Flush print buffer */
        fflush(stdout);

        /* Check if printing for populate tree or countprefix */
        if (fileIdx == DICTSRCFILEINDEX) {
            percent = (double)shared->numOfCharsReadFromFile[fileIdx] / (double)shared->totalNumOfCharsInFile[fileIdx];
        } else if (fileIdx == TESTFILEINDEX) {
            percent = (double)shared->numOfProcessedPrefixes / (double)shared->wordCountInFile[fileIdx];
        }
        lastPos = pos;
    }

    /* Finish out progress bar */
    if (intervalCount % shared->hashmarkInterval == 0) {
        cout << "#";
    } else {
        cout << "-";
    }

    cout << "\nThere are " << shared->wordCountInFile[fileIdx] << " words in " << shared->filePath[fileIdx] << "." << endl;
}
