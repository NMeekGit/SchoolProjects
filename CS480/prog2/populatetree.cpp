/*
 * POPULATETREE.CPP
 *
 * @author: Noah Meeker
 * @Red ID: 821272563
 * @Class: CS 480
 *
 * Implementation file for the PopulateTree Thread in the program countprefix
 */
#include <stdio.h>
#include <pthread.h>
#include "shared.h"
#include "populatetree.h"

void * populatetree (void *voidPtr) {

    /* Set the shared data to a variable for late use */
    SHARED_DATA *shared = (SHARED_DATA *)voidPtr;

    /* Checked if there is shared data */
    if (shared == NULL)
        return NULL;

    /* Open Vocab File */
    ifstream dictStream (shared->filePath[DICTSRCFILEINDEX]);
    string line;

    /* Check if file is open */
    if (dictStream.is_open()) {
        while (getline(dictStream, line)) {
            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char *word = strtok(line_c, shared->separators);
            while (word) {
                if (!add(word, shared)) {

                    // If word from vocabulary file could not be added to the dictionary
                    return NULL;
                }
                word = nullptr;
            }
            delete[] line_c;
        }
    }

    dictStream.close();
    
    /* mark completion of current task */
    shared->taskCompleted[DICTSRCFILEINDEX] = true;

    pthread_exit(0);
}

/* Helper Method 
 * ---
 *  Creates a new node and sets its children to NULL
 */
dictNode* createDictNode() {
    
    // creat a new node
    dictNode* node = (dictNode*) calloc (1, sizeof(dictNode));
    for (int index = 0; index < NCHILD; index++) {
        node->next[index] = NULL;
    }
    return node;
}

/* Insert Method 
 * ---
 *  Insertes word into dictionary
 *  return true if word is successfully inserted
 *  return false if unsuccessful
 */
bool add(const char *wordBeingInserted = nullptr, SHARED_DATA *shared = NULL) {
    dictNode* temp = shared->root;

    if (wordBeingInserted == nullptr) {

        // No word was inserted
        return false;
    }

    for (unsigned int index = 0; *(wordBeingInserted + index) != NULL_CHAR; ++index) {

        shared->numOfCharsReadFromFile[DICTSRCFILEINDEX]++;
        char c = *(wordBeingInserted + index);
        int pos = shared->validCharacters.at(c);
        if (!temp->next[pos]) {

            // Node doesn't exist
            temp->next[pos] = createDictNode();
        }
        temp = temp->next[pos];

    }

    // Add '\0' node to end of word
    temp->next[NULL_POS] = createDictNode();
    
    // Word successfully inserted
    shared->wordCountInFile[DICTSRCFILEINDEX]++;
    shared->numOfCharsReadFromFile[DICTSRCFILEINDEX]++;

    return true;

}
