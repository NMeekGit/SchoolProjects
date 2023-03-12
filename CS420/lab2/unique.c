/*
 * UNIQUE.C
 *
 * @author: Noah Meeker
 * @RED ID: 821272563
 * @CLASS: CS 420
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 1000      // maximum buffer size 
#define NULL_CHAR '\0'  // NULL character
#define NULL_STR '\n'   // Newline character

int main() {
    char input[BUFF];
    char uniqueTokens[BUFF][BUFF];
    int numTokens;
    int idx;

    /* read in input file */
    while (fgets(input, BUFF, stdin)) {

        /* Parse input */
        char *token = strtok(input, " ");
        while (token) {

            /* add word to buffer if it doesn't exist */
            int found = 1;
            for (idx = 0; idx < numTokens; idx++) {
                if (!strcmp(token, uniqueTokens[idx])) {
                    found = 0; // found
                    break;
                }
            }

            if (found) {
              strcpy(uniqueTokens[numTokens], token);
              numTokens++;
            }
            
            /* Move to next token */
            token = strtok(NULL, " ");
        }
    }

    /* print unique words */
    for (idx = 0; idx < numTokens; idx++) {
        printf("%s ", uniqueTokens[idx]);
    }
    printf("\n");

    return 0;
}
