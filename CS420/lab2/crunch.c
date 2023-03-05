#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>

void help();
void splitString(char**, char*, const char*);
void printOutput(char**, int, int, int, int);

#define DASH_ARG '-' 
#define DEGREE_ARG 'd' 
#define SIZE_ARG 'm' 
#define COUNT_ARG 'n'
#define SPACE_ARG 's'
#define BUFF_SIZE 100

int main(int argc, char **argv)
{
    printf("Num Args: %d\n", argc);
    printf("%s\n", argv[0]);
    
    int degree = 4;
    int size = 6;
    int count = 1;
    int space = 0;
    
    int index;
    for (index = 1; index < argc; index++) {
        
        if (argv[index][0] == DASH_ARG) {

            switch (argv[index][1]) {
                case DEGREE_ARG: 
                    sscanf(argv[index + 1], "%d", &degree);
                    break;
                case SIZE_ARG:
                    sscanf(argv[index + 1], "%d", &size);
                    break;
                case COUNT_ARG:
                    sscanf(argv[index + 1], "%d", &count);
                    break;
                case SPACE_ARG:
                    space = 1;
                    break;
                default:
                    break;
            }
        }
            
    }

    printf("Degree: %d, Size: %d, Count: %d, Space: %d\n", degree, size, count, space);

    int i;
    char* string = (char*)malloc(BUFF_SIZE * sizeof(char));
    char** words = (char**)malloc(BUFF_SIZE * sizeof(char));
    for (i = 0; i < BUFF_SIZE; i++) {
        words[i] = (char*)malloc(BUFF_SIZE * sizeof(char));
    }

    while ( !feof(stdin) ) {

        fgets(string, sizeof(string), stdin);
        splitString(words, string, " ");

    }

    printOutput(words, degree, size, count, space);
    
    free(string);
    free(words);
    return 0;
}

void splitString(char** words, char* string, const char* delim) {
    
    char** array = words;
    char* string_index = string;
    int i, j;

    for (i = 0; i < BUFF_SIZE; i++) {

        for (j = 0; j < BUFF_SIZE; j++) {

            if (!*string_index) {
                return;
            }

            if (*string_index != *delim) {
                array[i][j] = *string_index;
            } else {
                break;
            }

            string_index++;
        }
    }
}

void printOutput(char** words, int degree, int size, int count, int space) {

    char** array = words;
    int i, j;

    for (i = 0; i < sizeof(array); i++) {

        for (j = 0; j < sizeof(array[i]); j++) {
            printf("%c", array[i][j]);
        }

        if (space == 1) {
            printf(" ");
        }
    }
    
}

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
