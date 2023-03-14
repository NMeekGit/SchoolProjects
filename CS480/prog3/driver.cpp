/* 
 * DRIVER_CPP
 *
 * Simulation of demond paging using multi-level page tree/table
 * with address translation caching.
 *
 * 32-bit address space is assumed
 *
 * @author: Noah Meeker
 * @RED ID: 821272563
 * @Class:  CS 480
 */

int main (int argc, char **argv)
{
    
    if (argc < 3) {

        cout << "[ERROR] Not enough arguments" << endl;

    }

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
                    cout << "Cache capacity must be a number, greater than or rqual to 0" << endl;
                    exit(2);
                }
                hashInterval = atoi(optarg);
                break;
            case 'p':
                if (strcmp(optarg, LEVELBITMASKS) == 0) {
                    levelbitmasks = true;
                    summary = false;
                }
                if (strcmp(optarg, VA2PA) == 0) {
                    va2pa = true;
                    summary = true;
                }
                if (strcmp(optarg, VA2PA_TLB_PTWALK) == 0) {
                    va2pa_tlb_ptwalk = true;
                    summary = false;
                }
                if (strcmp(optarg, VPN2PFN) == 0) {
                    vpn2pfn = true;
                    summary = false;
                }
                if (strcmp(optarg, OFFSET) == 0) {
                    offset = true;
                    summary = false;
                }
                if (strcmp(optarg, SUMMARY) == 0) {
                    summary = true;
                break;
            default:
                cout << "" << endl;
                exit(2);
        }
    }

    int idx, level, bitTotal;

    DATA *data = (DATA*)malloc(sizeof(DATA));

    idx = optind;
    level = 0;
    bitTotal = 0;

    if (idx < argc) {

        /* Process positional arguments */
        
        tracerFile = argv[idx];

        ifstream checkFile (vocabFile);
        if (!checkFile.is_open()) {
            cout << "Unable to open <<" << tracerFile << ">>" << endl;
            exit(2);
        }
        checkFile.close();

        idx ++;

        while (idx < argc) {
            
            if (argv[idx] < 1)
                cout << "Level " << level << " page table must be at least 1 bit" << endl;
            
            data->levels[level] = argv[idx];
            bitTotal += argv[idx];

            idx ++;
            level ++;
        }
    }

    if (bitTotal > 28) {
        cout << "Too many bits used in page tables" << endl;
        exit(2);
    }



    return 0;
}
