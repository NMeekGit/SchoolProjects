#ifndef COUNTPREFIX_H
#define COUNTPREFIX_H

void * countPrefix(void *);
dictNode* findEndingNodeOfStr(const char *, SHARED_DATA *);
void countWordsStartingFromNode(dictNode *, int&, SHARED_DATA *);

#endif 
