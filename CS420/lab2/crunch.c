#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void help();

#define FILE_ARG '<'
#define DASH_ARG '-' 
#define DEGREE_ARG 'd' 
#define SIZE_ARG 'm' 
#define COUNT_ARG 'n'
#define SPACE_ARG 's'

int main(int argc, char **argv)
{
    printf("Num Args: %d\n", argc);
    printf("%s\n", argv[0]);
    
    int degree = 4;
    int size = 6;
    int count = 1;
    int space = 0;
    
    if (argc == 1) {
        help();
        return 1;
    }

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

        if (argv[index][0] == FILE_ARG) {
            printf("File Given\n");
        }
            
    }
    printf("Degree: %d, Size: %d, Count: %d, Space: %d\n", degree, size, count, space);

    return 0;
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
