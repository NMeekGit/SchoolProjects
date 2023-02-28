#include <stdio.h>
#include <pthread.h>
#include "shared.h"
#include "populatetree.h"
#include "dictionary.h"

void * populatetree (void *voidPtr) {

    SHARED_DATA *shared = (SHARED_DATA *)voidPtr;

    if (shared == NULL)
        return NULL;

    /* Opening Vocabulary File */
    ifstream dictStream (shared->filePath[DICTSRCFILEINDEX]);
    if (dictStream.is_open()) {
        while (getline(dictStream, line)) {
            char* line_c = new char[line.length() + 1];
            strcpy(line_c, line.c_str());
            char *word = strtok(line_c, delimiters);
            while (word) {
                if (!add(word)) {

                    // If word from vocabulary file could not be added to the dictionary
                    return NULL;
                }
                word = nullptr;
            }
            delete[] line_c;
        }
    }

    dictStream.close();
    

    pthread_exit();
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
bool add(const char *wordBeingInserted = nullptr) {
    dictNode* temp = this->root;
    const int NULL_POS = shared->validCharacters.at(NULL_CHAR);

    if (wordBeingInserted == nullptr) {

        // No word was inserted
        return false;
    }

    for (unsigned int index = 0; *(wordBeingInserted + index) != NULL_CHAR; ++index) {

        char c = *(wordBeingInserted + index);
        int pos = shared->validCharacters.at(c);
        if (!temp->next[pos]) {

            // Node doesn't exist
            temp->next[pos] = createDictNode();
        }
        temp = temp->next[pos];
        shared->numOfCharsProcessedFromFile[DICTSRCFILEINDEX]++;

    }

    // Add '\0' node to end of word
    temp->next[NULL_POS] = createDictNode();
    
    // Word successfully inserted
    shared->wordCountInFile[DICTSRCFILEINDEX]++;

    return true;

}
