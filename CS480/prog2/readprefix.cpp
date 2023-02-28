#include "shared.h"
#include "readprefix.h"

void *readPrefix(void *voidPtr) {

    SHARED_DATA *shared = (SHARED_DATA *)voidptr;

    if (shared == NULL)
        return NULL;

    ifstream textstream(shared->filePath[TESTFILEINDEX]);
    string line;

    while (getline(textstream, line)) {
    
        char *token = strtok(line, shared->separators);
        while (token) {
            string token_s = token;
            pthread_mutex_lock(&(shared->queue_mutex));

            shared->prefixQueue.push(token_s);

            shared->wordCountInFile[TESTFILEINDEX]++;
            
            pthread_mutex_unlock(&(shared->queue_mutex));
            
            token = strtok(NULL, shared->separators);
        }
    }

    exit(0);
}
