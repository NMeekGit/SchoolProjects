/* 
 * CS 420 | Lab 1
 *
 * bitmanipulation.c
 *
 * @author: Noah Meeker
 * @REDID: 821272563
 *
 */

#include <stdio.h>
#include <stdlib.h>

// Header Declaration
void DecToBin(int* arr, unsigned long long* val, int* size);
void OneComp(int* arr, int* size);
void TwoComp(int* arr, int* size);
void PrintBin(int* arr, int* size);

unsigned long long* pow_base2(int* exp);




int main(int argc, char *argv[]) {

    int* arr;
    unsigned long long* val = (unsigned long long*)malloc(sizeof(unsigned long long));
    int* size = (int*)malloc(sizeof(int));


    printf("CS 420: Lab 1\n");
    printf("-------------\n");
    printf("Bit Manipulation\n\n");
    
    printf("Input Number: ");
    
    // Check Integer Validation
    if (scanf("%llu", *&val) != 1) {
        printf("[ERROR] Incorrect Input. Must Be A Number\n");
        return 1;
    }
    printf("\n");

    printf("Input Bit Size: ");

    // Check Integer Validation
    if (scanf("%d", *&size) != 1) {
        printf("[ERROR] Incorrect Input. Must Be A Number\n");
        return 1;
    }
    printf("\n");

    // First *size in the below expression is for the case when *size is 0
    if (!(*size && !(*size & (*size - 1)))) {
        printf("[ERROR] Desired bit size %d is invalid. Try 4/8/16/32/64\n", *size);
        return 1;
    }

    if (*val/2 >= *(pow_base2(size))/2) {
        printf("[ERROR] Number %llu does not fit in desired bit system %d.\n", *val, *size);
        return 1;
    }

    printf("Number: %llu\n", *val);
    printf("Bit System: %d\n", *size);

    arr = (int*)malloc(sizeof(int) * *size); // Allocate array to bit system size

    DecToBin(arr, val, size);

    PrintBin(arr, size);

    OneComp(arr, size);

    PrintBin(arr, size);

    TwoComp(arr, size);

    PrintBin(arr, size);

    free(val);
    free(size);
    free(arr);

    return 0;
}

/* Convert Decimal to Binary */
void DecToBin(int* arr, unsigned long long* val, int* size) {

    int* arr_index = arr;
    int* arr_end = arr + *size;
    
    while (arr_index < arr_end) {

        if (*val > 0) {

            // Add remainder to array
            *arr_index = *val % 2;
            *val = *val / 2;

        } else {

            // Pad out the rest of the array with 0's
            *arr_index = 0;

        }

        arr_index += 1;
    }
}

/* Perform One's Compliment */
void OneComp(int* arr, int* size) {

    int* arr_index = arr;
    int* arr_end = arr + *size;

    while (arr_index < arr_end) {

        // Check value and flip
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

/* Perform Two's Compliment */
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

    free(carry);
}

/* Print Method */
void PrintBin(int* arr, int* size) {

    int* arr_start = arr;
    int* arr_index = arr + (*size - 1);

    while (arr_index >= arr_start) {

        printf("%d", *arr_index);
        arr_index -= 1;

    }
    printf("\n");
}

unsigned long long* pow_base2(int* exp) {

    int* temp = (int*)malloc(sizeof(int));
    int* base = (int*)malloc(sizeof(int));
    unsigned long long* sum = (unsigned long long*)malloc(sizeof(unsigned long long));
    *temp = *exp - 1;
    *base = 2;
    *sum = 1;

    while (*temp > 0) {

        *sum = *sum * (unsigned long long)(*base);
        *temp -= 1;
    }

    free(temp);
    free(base);
    return sum;

}
