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
                            
Dict::Dict() {
    this->root = new dictNode;
}

dictNode* Dict::createDictNode() {
    dictNode* node = (dictNode*) calloc (1, sizeof(dictNode));
    node->character = ' ';
    for (int index = 0; index < NCHILD; index++) {
        node->next[index] = NULL;
    }
    return node;
}

bool Dict::add(const char *wordBeingInserted = nullptr) {
    dictNode* temp = this->root;
    const int NULL_CHAR = validCharacters.at('\0');

    if (wordBeingInserted == nullptr) {
        return false;
    }

    for (unsigned int index = 0; *(wordBeingInserted + index) != '\0'; ++index) {

        char c = *(wordBeingInserted + index);
        int pos = validCharacters.at(c);
        if (!temp->next[pos]) {
            temp->next[pos] = createDictNode();
            temp->next[pos]->character = c;
        }
        temp = temp->next[pos];

    }

    temp->next[NULL_CHAR] = createDictNode();
    return true;

}

dictNode* Dict::findEndingNodeOfStr(const char *strBeingSearched) {
    dictNode* temp = this->root;
    const char NULL_TERMINATOR = '\0';

    for (int index = 0; *(strBeingSearched + index) != NULL_TERMINATOR; ++index) {
        char c = *(strBeingSearched + index);
        int pos = validCharacters.at(c);

        if (!temp->next[pos]) {
            return NULL;
        }
        temp = temp->next[pos];
    }
    if (temp) {
        return temp;
    }
    return NULL;
}

void Dict::countWordsStartingFromNode(dictNode* node, int& count) {
    const int NULL_CHAR = validCharacters.at('\0');

    if (!node) {
        return;
    }

    if (node->next[NULL_CHAR]) {
        count++;
    }

    for (int index = 0; index < NCHILD; index++) {
        
        if (node->next[index]) {
            countWordsStartingFromNode(node->next[index], count);
        }
    }

}
