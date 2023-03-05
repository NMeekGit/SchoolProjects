/*
 * READPREFIX.CPP
 *
 * @author: Noah Meeker
 * @Red ID: 821272563
 * @Class: CS 480
 *
 * Implementation file for the ReadPrefix Thread in the program countprefix
 */

#include "shared.h"
#include "readprefix.h"
#include <pthread.h>
#include <stdio.h>

void * readPrefix (void *voidPtr) {

    /* Set the shared data to a variable for later use */
    SHARED_DATA *shared = (SHARED_DATA *)voidPtr;

    /* Checked if there is shared data */
    if (shared == NULL)
        return NULL;

    /* Busy wait function
     *  - forces current thread to wait for the populate Thread to finish before further execution
     */
    while (!shared->taskCompleted[DICTSRCFILEINDEX]) {
    }
    
    /* Open Test File */
    ifstream textstream (shared->filePath[TESTFILEINDEX]);

    string line;

    /* Check if file is open */
    if (textstream.is_open()) {
        while (getline(textstream, line)) {
        
            char*line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char *token = strtok(line_c, shared->separators);
            while (token) {
                string token_s = token;

                /* give current thread ownership of shared data */
                pthread_mutex_lock(&(shared->queue_mutex));

                shared->prefixQueue.push(token_s);

                /* increment word count for progress bar */
                shared->wordCountInFile[TESTFILEINDEX]++;
                
                /* give up current thread's ownership of shared data */
                pthread_mutex_unlock(&(shared->queue_mutex));
                
                token = strtok(NULL, shared->separators);
            }
            delete[] line_c;
        }
    }

    textstream.close();
    
    /* mark completion of current task */
    shared->taskCompleted[TESTFILEINDEX] = true;

    pthread_exit(0);
}
