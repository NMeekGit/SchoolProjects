#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cmath>
#include <array>
#include <map>

using namespace std;

const int NCHILD = 30;
struct dictNode {
    array<dictNode*, NCHILD> next = {};
    char character;
};

class Dict{
    static const map<char, int> validCharacters;
        
public:
    Dict();
    
    dictNode* root;
    bool add(const char*);
    dictNode* createDictNode();
    dictNode* findEndingNodeOfStr(const char*);
    void countWordsStartingFromNode(dictNode*, int&);
};

#endif
