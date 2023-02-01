#include <stdio.h>
#include <stdlib.h>

void DecToBin(int* arr, int* val, int* size);
void OneComp(int* arr, int* size);
void TwoComp(int* arr, int* size);
void PrintBin(int* arr, int* size);

int main(int argc, char *argv[])
{

    int* arr;
    int* val = (int*)malloc(sizeof(int));
    int* size = (int*)malloc(sizeof(int));

    printf("CS 420: Lab 1\n");
    printf("-------------\n");
    printf("Bit Manipulation\n\n");
    
    printf("Input Decimal Number: ");
    scanf("%d", *&val);
    printf("\n");

    printf("Input Bit Size: ");
    scanf("%d", *&size);
    printf("\n");

    /* if (*val > (2 << *size)) { */
    /*     printf("[ERROR] Decimal Number does not fit desired bit system\n"); */
    /*     return 1; */
    /* } */

    printf("Number: %d\n", *val);
    printf("Bit System: %d\n", *size);

    arr = (int*)malloc(sizeof(int) * *size);

    DecToBin(arr, val, size);

    PrintBin(arr, size);

    OneComp(arr, size);

    PrintBin(arr, size);

    TwoComp(arr, size);

    PrintBin(arr, size);

    return 0;
}

void DecToBin(int* arr, int* val, int* size) {

    int* arr_index = arr;
    int* arr_end = arr + *size;
    
    while (arr_index < arr_end) {

        if (*val > 0) {

            *arr_index = *val % 2;
            *val = *val / 2;

        } else {

            *arr_index = 0;

        }

        arr_index += 1;
    }
}

void OneComp(int* arr, int* size) {

    int* arr_index = arr;
    int* arr_end = arr + *size;

    while (arr_index < arr_end) {

        switch (*arr_index) {
            case 0:
                *arr_index = 1;
                break;
            case 1:
                *arr_index = 0;
                break;
        }

        arr_index += 1;
    }

}

void TwoComp(int* arr, int* size) {

    int* arr_index = arr;
    int* arr_end = arr + *size;

    int* carry = (int*)malloc(sizeof(int));

    *carry = 1;

    while (arr_index < arr_end) {

        if (*arr_index == 1 && *carry == 1) {
            *arr_index = 0;
        } else if ( *arr_index == 0 && *carry == 1) {
            *arr_index = 1;
            *carry = 0;
        }

        arr_index += 1;
    }
}

void PrintBin(int* arr, int* size) {

    int* arr_start = arr;
    int* arr_index = arr + (*size - 1);

    while (arr_index >= arr_start) {

        printf("%d", *arr_index);
        arr_index -= 1;

    }
    printf("\n");
}
