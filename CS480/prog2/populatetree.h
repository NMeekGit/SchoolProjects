/* 
 * POPULATETREE.H
 *
 * Header file for populatetree.cpp implementation file
 *
 * @author: Noah Meeker
 * @RED ID: 821272563
 * @CLass: CS 480
 */

#ifndef POPULATETREE_H
#define POPULATETREE_H

void * populatetree(void *);            // main thread function
bool add(const char *, SHARED_DATA *);  // Add vocab to dictionary
dictNode * createDictNode();            // Node creatio helper function

#endif
