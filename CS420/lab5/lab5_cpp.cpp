/* Lab5
 *      -CPP
 *
 * @author: Noah Meeker
 * @Red ID: 821272563
 *
 * The goal with this lab is to compare the mergesort times and quicksort times between
 * two languages: cpp and haskel.
 *
 * These two sorting algorithms will be tested against three input sequences. One is in
 * order, the next one is in reverse, and the last is in random order.
 */

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>

#define ARRAY_SIZE 10000

using namespace std;

void inputSortedArray(int*);
void inputReverseArray(int*);
void inputRandomArray(int*);

void quicksort(int*, int, int);
/* void mergesort(int& array, int size); */
/* void merge(int& array, int& left, int& right, int sizeRight, int sizeLeft); */

void summary(clock_t, clock_t, string);

int main() {
    clock_t begin;
    clock_t end;
    int sortedArray[ARRAY_SIZE];
    int reverseArray[ARRAY_SIZE];
    int randomArray[ARRAY_SIZE];
    string name;

    inputSortedArray( sortedArray );
    inputReverseArray( reverseArray );
    inputRandomArray( randomArray );

    // Quicksort : sortedArray
    begin = clock();
    quicksort( sortedArray, 0, ARRAY_SIZE);
    end = clock();

    // print result
    name = "Quicksort : Sorted Array";
    summary( begin, end, name );

    // Quicksort : reverseArray
    begin = clock();
    quicksort( reverseArray, 0, ARRAY_SIZE);
    end = clock();

    // print result
    name = "Quicksort : Reverse Array";
    summary( begin, end, name );

    // Quicksort : randomArray
    begin = clock();
    quicksort( randomArray, 0, ARRAY_SIZE);
    end = clock();

    // print result
    name = "Quicksort : Random Array";
    summary( begin, end, name );

    return 0;
};

void inputSortedArray( int* array ) {
    for (int idx = 0; idx < ARRAY_SIZE; idx++) {
        array[idx] = idx;
    }
};

void inputReverseArray( int* array ) {
    for (int idx = ARRAY_SIZE - 1; idx >= 0; idx--) {
        array[idx] = idx;
    }
};

void inputRandomArray( int* array ) {
    srand(time(NULL));
    for (int idx = 0; idx < ARRAY_SIZE; idx++) {
        array[idx] = rand() % ARRAY_SIZE;
    }
};

void quicksort( int* array, int left, int right) {
    int idx = left; 
    int jdx = right;
    int pivot = array[(left + right) / 2];
    int tmp;

    while ( idx <= jdx ) {
        while ( array[idx] < pivot)
            idx++;
        while ( array[jdx] > pivot)
            jdx--;
        if ( idx <= jdx) {
            tmp = array[idx];
            array[idx] = array[jdx];
            array[jdx] = tmp;
            idx++;
            jdx--;
        }
    }

    if (left < jdx)
        quicksort( array, left, jdx );
    if (idx < right)
        quicksort( array, idx, right );
};

void mergesort( int* array, int size ) {
    
    if (size < 2) 
        return;

    int mid = size / 2;
    int left[mid];
    int right[mid - size];
    int idx;

    for ( idx = 0; idx < mid; idx++ ) {
        left[idx] = array[idx];
    }
    for ( idx = mid; i < size; idx++ ) {
        right[idx - mid] = array[idx];
    }

    mergesort( left, mid );
    mergesort( right, size - mid );
    merge( array, left, right, mid, size - mid);
};

void summary( clock_t start, clock_t end, string arrayName ) {

    cout << "\n" << arrayName << endl;
    cout << setfill('-') << setw(32) << ">" << endl;
    cout << setfill(' ') << "Time Elapsed : " << (double) (end - start) / CLOCKS_PER_SEC << "\n" << endl;
};

