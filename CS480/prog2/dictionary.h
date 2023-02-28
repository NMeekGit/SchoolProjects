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

#define NULL_CHAR '\0'
#define NCHILD 30
typedef struct dictNode {
    struct dictNode* next[NCHILD]; // a-z,',-,_,NULL_CHAR
};

class Dict{
    static const map<char, int> validCharacters;
        
    public:
        Dict();                                             // Constructor
        ~Dict();                                            // Deconstructor
        dictNode* root;                                     // root node
        bool add(const char*);                              // Add() Method
        dictNode* createDictNode();                         // Creat() Method
        dictNode* findEndingNodeOfStr(const char*);         // Search() Method
        void countWordsStartingFromNode(dictNode*, int&);   // Count() Method
};

#endif
