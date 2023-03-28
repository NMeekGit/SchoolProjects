//TO DO: Extract the entire VPN as well as its frame number and put it into the TLB
//From here, we monitor the 8 entries and cycle them
#include "PageTable.h"
#include "TLB.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <climits>


int checkTLB(unsigned int VPNValue, TLB* tlb){
    unordered_map<int, int> TLBList = tlb -> table;
    unordered_map<int, int> LRUList = tlb -> lruCount;
    int count = tlb -> addressCount;
    if(TLBList.find(VPNValue) == TLBList.end()){
        return 0;
    }
    unordered_map<int, int>::const_iterator PFNLoc;
    PFNLoc = TLBList.find(VPNValue);
    int PFNReturn = PFNLoc -> second;
    LRUList[VPNValue] = count;
    return PFNReturn;
}

void updateTLB(unsigned int VPNValue, int sizeOfTLB, TLB* tlb, int PFN){
    unordered_map<int, int> TLBList = tlb -> table;//find will give second int
    unordered_map<int, int> LRUCounter = tlb -> lruCount;
    int count = tlb -> addressCount;
    if(TLBList.size() < sizeOfTLB){
        TLBList.insert({VPNValue, PFN});
        LRUCounter.insert({VPNValue, count});
    }else{
        int least = INT_MAX;
        int val;
        for(auto i = TLBList.begin(); i != TLBList.end(); i++){
            if(LRUCounter[i -> second] < least){
                least = LRUCounter[i -> second];
                val = i -> first;
            }
        }
        TLBList.erase(val);
        LRUCounter.erase(val);
        TLBList.insert({VPNValue, PFN});
        LRUCounter.insert({VPNValue, count});
    }
}
