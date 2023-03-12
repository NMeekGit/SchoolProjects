/*
 * CRUNCH.C
 *
 * @author: Noah Meeker
 * @RED ID: 821272563
 * @CLASS: CS 420
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define DASH_ARG '-' 
#define DEGREE_ARG 'd' 
#define SIZE_ARG 'm' 
#define COUNT_ARG 'n'
#define SPACE_ARG 's'
#define BUFF 1000

void help();
int createRandomArray(char [BUFF][BUFF], char [BUFF][BUFF], int);
void printOutput(char [BUFF][BUFF], int, int, int);
int compareStrings(char [BUFF][BUFF], char *, int);
int getSize(char*);

int main(int argc, char **argv)
{
    
    /* default value */
    int degree = 4;
    int size = 6;
    int count = 1;
    int space = 0;

    /* Init Random Seed */
    srand(time(NULL));
    
    /* Check arguements */
    int index;
    for (index = 1; index < argc; index++) {
        
        if (argv[index][0] == DASH_ARG) {

            switch (argv[index][1]) {
                case DEGREE_ARG:  //change degree value
                    sscanf(argv[index + 1], "%d", &degree);
                    break;
                case SIZE_ARG:    // change size value
                    sscanf(argv[index + 1], "%d", &size);
                    break;
                case COUNT_ARG:   // change count value
                    sscanf(argv[index + 1], "%d", &count);
                    break;
                case SPACE_ARG:   // change size value
                    space = 1;
                    break;
                default:
                    break;
            }
        }
            
    }

    printf("Degree: %d, Size: %d, Count: %d, Space: %d\n", degree, size, count, space);

    /* Start */
    int line;
    for (line = 0; line < count; line++) {
        char string[BUFF];
        char words[BUFF][BUFF];
        char randomized[BUFF][BUFF];
        char* token;
        int idx;

        while ( !feof(stdin) ) {

            /* read input */
            fgets(string, BUFF, stdin);

            /* parse input */
            token = strtok(string, " ");

            while (token) {
                
                if (getSize(token) >= size) {
                    strcpy(words[idx], token); // add token to word buffer
                    idx++;
                }

                token = strtok(NULL, " ");

            }

        }

        int randLen = createRandomArray(words, randomized, idx); // create Randomized array, return length
        printOutput(randomized, randLen , degree, space);        // printout
    }
    
    return 0;
}

/* Returns length of string */
int getSize(char* ptr) {
    int count = 0;

    while (ptr[count] != '\0') {
        count++;
    }
    return count;
}

/* create randomized array 
 * 
 * returns length
 */
int createRandomArray(char words[BUFF][BUFF], char random[BUFF][BUFF], int size) {
    int i;
    int randLen = 0;

    for (i = 0; i < size; i++) {
        int randInt = rand() % size; // random number
        
        if (compareStrings(random, words[randInt], randLen)) {
            strcpy(random[i], words[randInt]);
            randLen++;
        }
    }
    return randLen;
}

/* compares strings 
 *
 * returns 
 *  - 0 = found
 *  - 1 = not found
 */
int compareStrings(char arr[BUFF][BUFF], char* string, int size) {
    int i;

    int found = 1;
    for (i = 0; i < size; i++) {
        /* is found */
        if (!strcmp(arr[i], string)) {
            found = 0;
        }
    }
    return found;
}

/*Print Out */
void printOutput(char words[BUFF][BUFF], int arr_size, int degree, int space) {

    int i;

    /* number of words to print each line */
    for (i = 0; i < degree; i++) {

        printf("%s", words[i]);

        /* need of space? */
        if (space == 1) {
            printf(" ");
        }
    }
    printf("\n");
    
}

/* help method for user
 *
 * prints program info
 */
void help() {

    printf("Program Name:       crunch\n");
    printf("---\n");
    printf("Command Args:       -d <degree>\n");
    printf("                        Number of words from input stream to combine in each crunched output (default = 4).\n");
    printf("                    -m <size>\n");
    printf("                        The minimum length of each word used (default = 6)\n");
    printf("                    -n <count>\n");
    printf("                        Number of output strings to produce (default = 1)\n");
    printf("                    -s\n");
    printf("                        Indicates to insert a space between the output words (default = none)\n");

}
