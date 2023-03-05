#include <stdio.h>

#define BUFF 1000  // maximum length of input strings
#define NULL_CHAR '\0'
#define NULL_STR '\n'
#define TRUE 1
#define FALSE 0

int main(void) {
    char* input = (char*)malloc(BUFF * sizeof(char));
    char** tokens = (char**)malloc(BUFF * sizeof(input));
    int numTokens;
    int idx;

    // read input strings until EOF
    while (fgets(input, BUFF, stdin)) {
        // remove line ending from input string
        idx = 0;
        while (input[idx] != NULL_CHAR && input[idx] != NULL_STR) {
            idx++;
        }
        input[idx] = NULL_CHAR;

        // split input string into tokens
        char *token = strtok(input, " ");
        int n_tokens = 0;
        while (token != NULL) {
            // check if token is already in array
            int found = FALSE;
            for (idx = 0; idx < n_tokens; idx++) {
                if (!strcmp(token, tokens[idx])) {
                    found = TRUE;
                    break;
                }
            }
            // add token to array if not already present
            if (!found) {
              strcpy(tokens[numTokens], token);
              numTokens++;
            }
            // get next token
            token = strtok(NULL, " ");
        }
    }

    // print unique tokens separated by spaces
    for (idx = 0; idx < numTokens; idx++) {
        printf("%s ", tokens[idx]);
    }
    printf("\n");

    return 0;
}
