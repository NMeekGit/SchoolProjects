#include <stdio.h>

#define MAX_STR_LEN 1000  // maximum length of input strings
#define MAX_TOKENS 1000   // maximum number of unique tokens

int main(void) {
  char input_str[MAX_STR_LEN];
  char tokens[MAX_TOKENS][MAX_STR_LEN];
  int n_tokens = 0;
  int i, j;

  // read input strings until EOF
  while (fgets(input_str, MAX_STR_LEN, stdin) != NULL) {
    // remove line ending from input string
    int len = 0;
    while (input_str[len] != '\0' && input_str[len] != '\n') {
      len++;
    }
    input_str[len] = '\0';

    // split input string into tokens
    char *token = strtok(input_str, " ");
    while (token != NULL) {
      // check if token is already in array
      int found = 0;
      for (i = 0; i < n_tokens; i++) {
        if (strcmp(token, tokens[i]) == 0) {
          found = 1;
          break;
        }
      }
      // add token to array if not already present
      if (!found) {
        strcpy(tokens[n_tokens], token);
        n_tokens++;
      }
      // get next token
      token = strtok(NULL, " ");
    }
  }

  // print unique tokens separated by spaces
  for (i = 0; i < n_tokens; i++) {
    printf("%s ", tokens[i]);
  }
  printf("\n");

  return 0;
}
