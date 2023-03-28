//TO DO: Extract the entire VPN as well as its frame number and put it into the TLB
//From here, we monitor the 8 entries and cycle them

#ifndef TLB_H
#define TLB_H
#include <iostream>
#include <stdio.h>
#include <unordered_map>

using namespace std;

typedef struct TLB{
    int addressCount;
    unordered_map<int, int> table;
    unordered_map<int, int> lruCount;
};

int checkTLB(unsigned int VPNValue, TLB* tlb);
void updateTLB(unsigned int VPNValue, int sizeOfTLB, TLB* table, int PFN);

#endif
