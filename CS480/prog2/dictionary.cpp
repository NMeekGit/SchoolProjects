/*
 * dictionary.cpp | Assignment 1
 *
 * @class  CS 480
 * @author Noah Meeker
 * @REDID  821272563
 */

#include "dictionary.h"

const map<char, int> Dict::validCharacters{ {'a' , 0}, {'A' , 0},
                                            {'b' , 1}, {'B' , 1},
                                            {'c' , 2}, {'C' , 2},
                                            {'d' , 3}, {'D' , 3},
                                            {'e' , 4}, {'E' , 4},
                                            {'f' , 5}, {'F' , 5},
                                            {'g' , 6}, {'G' , 6},
                                            {'h' , 7}, {'H' , 7},
                                            {'i' , 8}, {'I' , 8},
                                            {'j' , 9}, {'J' , 9},
                                            {'k' , 10}, {'K' , 10},
                                            {'l' , 11}, {'L' , 11},
                                            {'m' , 12}, {'M' , 12},
                                            {'n' , 13}, {'N' , 13},
                                            {'o' , 14}, {'O' , 14},
                                            {'p' , 15}, {'P' , 15},
                                            {'q' , 16}, {'Q' , 16},
                                            {'r' , 17}, {'R' , 17},
                                            {'s' , 18}, {'S' , 18},
                                            {'t' , 19}, {'T' , 19},
                                            {'u' , 20}, {'U' , 20},
                                            {'v' , 21}, {'V' , 21},
                                            {'w' , 22}, {'W' , 22},
                                            {'x' , 23}, {'X' , 23},
                                            {'y' , 24}, {'Y' , 24},
                                            {'z' , 25}, {'Z' , 25},
                                            {'\'' , 26}, {'-' , 27},
                                            {'_' , 28}, {'\0' , 29}};
                            
/* Constructor */
Dict::Dict() {
    this->root = createDictNode();
}

/* Deconstructor */
Dict::~Dict() {

}

/* Helper Method 
 * ---
 *  Creates a new node and sets its children to NULL
 */
dictNode* Dict::createDictNode() {
    
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
bool Dict::add(const char *wordBeingInserted = nullptr) {
    dictNode* temp = this->root;
    const int NULL_POS = validCharacters.at(NULL_CHAR);

    if (wordBeingInserted == nullptr) {

        // No word was inserted
        return false;
    }

    for (unsigned int index = 0; *(wordBeingInserted + index) != NULL_CHAR; ++index) {

        char c = *(wordBeingInserted + index);
        int pos = validCharacters.at(c);
        if (!temp->next[pos]) {

            // Node doesn't exist
            temp->next[pos] = createDictNode();
        }
        temp = temp->next[pos];

    }

    // Add '\0' node to end of word
    temp->next[NULL_POS] = createDictNode();

    // Word successfully inserted
    return true;

}

/* Search Method 
 * ---
 *  Search for word's last character
 *  returns last node if all characters in word are found
 *  returns NULL if any of the characters in word does not exist
 */
dictNode* Dict::findEndingNodeOfStr(const char *strBeingSearched) {
    dictNode* temp = this->root;

    for (int index = 0; *(strBeingSearched + index) != NULL_CHAR; ++index) {
        char c = *(strBeingSearched + index);
        int pos = validCharacters.at(c);

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
void Dict::countWordsStartingFromNode(dictNode* node, int& count) {
    const int NULL_POS = validCharacters.at(NULL_CHAR);

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
            countWordsStartingFromNode(node->next[index], count);
        }
    }

}
