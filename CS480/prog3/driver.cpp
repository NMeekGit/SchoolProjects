/* 
 * DRIVER_CPP
 *
 * Simulation of demond paging using multi-level page tree/table
 * with address translation caching.
 *
 * 32-bit address space is assumed
 *
 * @author: Noah Meeker, Victor Cintra
 * @RED ID: 821272563, 824115494
 * @Class:  CS 480
 */
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "PageTable.h"
#include "TLB.h"
#include "print_helpers.h"
#include "vaddr_tracereader.h"

using namespace std;

int main (int argc, char **argv){
    
    if (argc < 3) {
        cout << "[ERROR] Not enough arguments" << endl;
    }

    int option;
    int numOfAccess = 0; 
    int sizeTLB = 0;
    bool ifTLB = false;
    
    OutputOptionsType* outputOptions = (OutputOptionsType*)malloc(sizeof(OutputOptionsType));

    outputOptions -> levelbitmasks = false;
    outputOptions -> va2pa = false;
    outputOptions -> va2pa_tlb_ptwalk = false;
    outputOptions -> vpn2pfn = false;
    outputOptions -> offset = false;
    outputOptions -> summary = true;

    TLB* tlb = (TLB*) malloc (sizeof(TLB));

    tlb -> addressCount = 0;

    while ((option = getopt(argc, argv, "n:c:p:")) != -1) {

        switch (option) {
            case 'n':
                if (atoi(optarg) < 0) {
                    cout << "Number of memory accesses must be a number, greater than or equal to 0" << endl;
                    exit(2);
                }
                numOfAccess = atoi(optarg);
                break;
            case 'c':
                if (atoi(optarg) < 0) {
                    cout << "Cache capacity must be a number, greater than or equal to 0" << endl;
                    exit(2);
                }
                sizeTLB = atoi(optarg);
                ifTLB = true;
                break;
            case 'p':
                if (strcmp(optarg, "levelbitmasks") == 0) {
                    outputOptions -> levelbitmasks = true;
                    outputOptions -> summary = false;
                }
                if (strcmp(optarg, "va2pa") == 0) {
                    outputOptions -> va2pa = true;
                    outputOptions -> summary = false;
                }
                if (strcmp(optarg, "va2pa_tlb_ptwalk") == 0) {
                    outputOptions -> va2pa_tlb_ptwalk = true;
                    outputOptions -> summary = false;
                }
                if (strcmp(optarg, "vpn2pfn") == 0) {
                    outputOptions -> vpn2pfn = true;
                    outputOptions -> summary = false;
                }
                if (strcmp(optarg, "offset") == 0) {
                    outputOptions -> offset = true;
                    outputOptions -> summary = false;
                }
                if (strcmp(optarg, "summary") == 0) {
                    outputOptions -> summary = true;
                break;
            default:
                cout << "Incorrect optional input" << endl;
                exit(2);
        }
        }
    }

    int* bitInputArray = new int[MAX_NUM_OF_LEVELS];
    int totalBitSize = 0;

    PageTable* pageTable = (PageTable*)malloc(sizeof(PageTable));
    
    /* PageTable-> levelCount initialization */
    pageTable -> levelCount = 0;
    int idx = optind;
    char* tracerFile;
    int fullMask;
    if (idx < argc) {

        /* Process positional arguments */
        
        tracerFile = argv[idx];

        ifstream checkFile (tracerFile);
        if (!checkFile.is_open()) {
            cout << "Unable to open <<" << tracerFile << ">>" << endl;
            exit(2);
        }
        checkFile.close();

        idx++;

        while (idx < argc) {

            if (atoi(argv[idx]) < 1) {
                cout << "Level " << pageTable -> levelCount << " page table must be at least 1 bit" << endl;
                exit(2);
            }

            totalBitSize += atoi(argv[idx]);
            if(checkTotalBitSize(totalBitSize)) {
                cout << "Too many bits used in page tables" << endl;
                exit(2);
            }

            bitInputArray[pageTable -> levelCount] = atoi(argv[idx]);
            fullMask = atoi(argv[idx]);
            pageTable -> levelCount += 1;

            idx++;

        }
    }

    /* Initialize bitShift[] */
    pageTable -> bitShift = bitShiftArray(bitInputArray, pageTable -> levelCount);

    /* Initialize bitMask[] */
    pageTable -> bitMask = bitMaskArray(bitInputArray, pageTable -> bitShift, pageTable -> levelCount);

    /* Initialize entryCount[] */
    pageTable -> entryCount = entryCountArray(bitInputArray, pageTable -> levelCount);


    if (outputOptions -> levelbitmasks) 
        report_levelbitmasks(pageTable -> levelCount, (uint32_t*)pageTable -> bitMask);

    /* cout << "totalbitsize: " << totalBitSize << endl; */
    /* cout << "bitShift[]:" << setw(10) << pageTable -> bitShift[0] << endl; */
    /* printf("bitMask[]: 0x%X\n", pageTable -> bitMask[0]); */
    cout << "Entry Level:" << endl;
    for (int i = 0; i < pageTable -> levelCount; i++) {
        cout <<  pageTable -> entryCount[i] <<  setw(6);
    }
    cout << "" << endl;

    if (pageTable -> levelCount == 1) {
        pageTable -> root = createLevel(pageTable -> entryCount[0], true);
    } else {
        pageTable -> root = createLevel(pageTable -> entryCount[0], false);
    }

    pageTable -> root -> PageTablePtr = pageTable;
    
    FILE *ifp;	        /* trace file */
    unsigned long vAddr;  /* instructions processed */
    p2AddrTr trace;	/* traced address */
    int frame = 0;
    Map* PFN;

    ifp = fopen(tracerFile, "r");

    int fullShift = 32 - fullMask;

    int readCount = 0;
    int TLBhit = 0;
    int pageHit = 0;
    unsigned int bytes = sizeof(pageTable);
    bool tlbHitCheck;
    bool ptHitCheck;
    
    while (!feof(ifp)) {
        unsigned int fullVPN;
        bool checkMap = false;
        tlbHitCheck = false;
        ptHitCheck = false;
        if (numOfAccess != 0 && readCount == numOfAccess)
            break;

        if (NextAddress(ifp, &trace)) {
            vAddr = trace.addr;
        }


        if(ifTLB == true){
            fullVPN = virtualAddressToVPN(vAddr, fullMask, fullShift);
            if(checkTLB(fullVPN, tlb) == 0){
                tlb -> addressCount++;
                PFN = lookup_vpn2pfn(pageTable, vAddr);
                if(!PFN) {
                    insert_vpn2pfn(pageTable, vAddr, frame);
                    PFN = lookup_vpn2pfn(pageTable, vAddr);
                } else {
                    ptHitCheck = true;
                }
                updateTLB(fullVPN, sizeTLB, tlb, PFN -> frame);
            }else{
                tlbHitCheck = true;
                tlb -> addressCount++; 
                insert_vpn2pfn(pageTable, vAddr, frame);
                PFN = lookup_vpn2pfn(pageTable, vAddr);   
            }
        }else{
            PFN = lookup_vpn2pfn(pageTable, vAddr);
            if(!PFN){
                insert_vpn2pfn(pageTable, vAddr, frame);
                PFN = lookup_vpn2pfn(pageTable, vAddr);
            }else{
                pageHit++;
                ptHitCheck = true;
                }
        }


        
        int physicalAddr = grabPhysicalAddress(PFN -> frame, MAX_BIT_SIZE - totalBitSize, vAddr);

        if (outputOptions -> va2pa) 
            report_virtualAddr2physicalAddr((uint32_t)vAddr, (uint32_t)physicalAddr);

        if(outputOptions -> va2pa_tlb_ptwalk){
            report_va2pa_TLB_PTwalk((uint32_t) vAddr, (uint32_t) physicalAddr, tlbHitCheck, ptHitCheck);
        }
        
        if (outputOptions -> offset) {
            unsigned int mask = bit_Mask(MAX_BIT_SIZE - totalBitSize, 0);
            hexnum(vAddr & mask);
        }

        if (outputOptions -> vpn2pfn) {
            pageTable -> pages = pagesArray(pageTable -> bitMask, pageTable -> bitShift, pageTable -> levelCount, vAddr);
            report_pagetable_map(pageTable -> levelCount, (uint32_t*)pageTable -> pages, PFN -> frame);
        }

        /* printf("%08X => %08X\n", vAddr, physicalAddr); */

        readCount++;
    }

    if (outputOptions -> summary) {
            //countTotalBytes(pageTable -> root, bytes, pageTable);
            report_summary(MAX_BIT_SIZE - totalBitSize, TLBhit, pageHit, readCount, PFN -> frame, bytes);
        }

    fclose(ifp);
    cout << "Complete" << endl;

    return 0;
}
