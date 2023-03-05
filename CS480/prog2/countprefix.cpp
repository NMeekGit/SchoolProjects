/*
 * READPREFIX.CPP
 *
 * @author: Noah Meeker
 * @Red ID: 821272563
 * @Class: CS 480
 *
 * Implementation file for the CountPrefix Thread in the program countprefix
 */

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "shared.h"
#include "countprefix.h"

void * countPrefix(void *voidPtr) {

    /* Set the shared data to a variable for later use */
    SHARED_DATA *shared = (SHARED_DATA *)voidPtr;

    /* Chacked if there is shared data */
    if (shared == NULL)
        return NULL;

    /* Busy wait fucntion
     *  - forces current thread to wait for the populate Thread to finish before further execution */
    while (!shared->taskCompleted[DICTSRCFILEINDEX]) {
    }

    string word;

    /* Open output file */
    ofstream output ("countprefix_output.txt");

    /* Check if output file is open */
    if (output.is_open()) {

        while (!shared->prefixQueue.empty() || !shared->taskCompleted[TESTFILEINDEX]) {
            if (!shared->prefixQueue.empty()) {
                int count = 0;
                
                /* give current thread ownership of shared data */
                pthread_mutex_lock(&(shared->queue_mutex));

                /* Grab first word in queue */
                word = shared->prefixQueue.front();
                char* word_c = new char[word.length() + 1];
                strcpy(word_c, word.c_str());

                /* give up current thread's ownership of shared data */
                pthread_mutex_unlock(&(shared->queue_mutex));

                dictNode *node = findEndingNodeOfStr(word_c, shared);

                countWordsStartingFromNode(node, count, shared);

                /* Check if preffix's count is large enough for print */
                if (count >= shared->minNumOfWordsWithAPrefixForPrinting)
                    output << word << " " << count << endl;

                pthread_mutex_lock(&(shared->queue_mutex));

                /* preffix check complete. Remove word from queue */
                shared->prefixQueue.pop();
                /* increment number of preffixes processed for progress bar */
                shared->numOfProcessedPrefixes++;

                pthread_mutex_unlock(&(shared->queue_mutex));
            }

        }


    }

    output.close();

    pthread_exit(0);
}

/* Search Method 
 * ---
 *  Search for word's last character
 *  returns last node if all characters in word are found
 *  returns NULL if any of the characters in word does not exist
 */
dictNode* findEndingNodeOfStr(const char *strBeingSearched, SHARED_DATA *shared) {
    dictNode* temp = shared->root;

    for (int index = 0; *(strBeingSearched + index) != NULL_CHAR; ++index) {
        char c = *(strBeingSearched + index);
        int pos = shared->validCharacters.at(c);

        if (!temp->next[pos]) {

            // Word is not in dictionary
            return NULL;
        }
        temp = temp->next[pos];
    }

    if (temp) {

        // Word is in Dictionary
        return temp;
    }

    // If all else fails
    return NULL;
}

/* Count Method 
 * ---
 *  count the amount of words starting with the preffix passed in
 *  return void
 */
void countWordsStartingFromNode(dictNode* node, int& count, SHARED_DATA *shared) {

    if (!node) {

        // Word doesn't exist in dictionary
        // Returns initial value of count
        return;
    }

    if (node->next[NULL_POS]) {
        
        // Increments count if current node has a null character node attached
        count++;
    }

    for (int index = 0; index < NCHILD; index++) {
        
        if (node->next[index]) {
            
            // Check for child node
            // if exists, move to node
            countWordsStartingFromNode(node->next[index], count, shared);
        }
    }

}
