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
    for (int index = 0; index < NCHILD; index++) {
        node->next[index] = NULL;
    }
    return node;
}

bool Dict::add(const char *wordBeingInserted = nullptr) {
    dictNode* temp = this->root;

    if (wordBeingInserted == nullptr) {
        return false;
    }

    for (unsigned int index = 0; index < strlen(wordBeingInserted); ++index) {

        char c = *(wordBeingInserted + index);
        int pos = validCharacters.at(c);

        if (!temp->next[pos]) {
            temp->next[pos] = createDictNode();
        }
        temp = temp->next[pos];
    }
    return true;

}

dictNode* Dict::findEndingNodeOfStr(const char *strBeingSearched) {
    dictNode* temp = this->root;
    const int NULL_TERMINATOR = validCharacters.at('\0');

    for (unsigned int index = 0; index < strlen(strBeingSearched); ++index) {
        char c = *(strBeingSearched + index);
        int pos = validCharacters.at(c);

        if (!temp->next[pos]) {
            return NULL;
        }
        else if (pos == NULL_TERMINATOR) {
            return temp;
        }
        temp = temp->next[pos];
    }
    return temp;
}

void Dict::countWordsStartingFromNode(dictNode* node, int& count) {
    dictNode* temp = node;
    const int NULL_TERMINATOR = validCharacters.at('\0');
    
    if (!temp) {
        return;
    }

    for (int index = 0; index < NCHILD; index++) {
        
        if (temp->next[index]) {
            if (index == NULL_TERMINATOR) {
                count++;
                return;
            }
            countWordsStartingFromNode(temp->next[index], count);
        }
    }

}
