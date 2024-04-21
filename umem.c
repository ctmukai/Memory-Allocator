#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "umem.h"
#define align(x) ((x + 7) & ~7)

//Free list structure for memory chunk
typedef struct listThatIsFree {
    size_t size;
    struct listThatIsFree *next;
} listThatIsFree;

//Global Variables
static int init = 0;
static int AA;
static void* memRegion = NULL;
static listThatIsFree* luffy = NULL; //most free man (on a list) captain
static listThatIsFree* zoro = NULL; //next fit (right hand man)
static listThatIsFree* present;
static listThatIsFree* past = NULL;


//--------------------------------------------------//
//                                                  //
//                   Helper Functions               //
//                                                  //      
//--------------------------------------------------//



//----------------------------------------//
//                                        //
//                  Debug                 //
//                                        //
//----------------------------------------//

//testing roundingPages functionality
/*void pageRoundTest() {
    size_t sizes[] = {0, 1, 4096, 8192, 63, 41, 33, 26};
    printf("testing roundingPage function:\n");
    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        size_t original_size = sizes[i];
        size_t rounded_size = roundingPages(original_size);
        printf("Original size: %zu, Rounded size: %zu\n", original_size, rounded_size);
    }
}*/

int umeminit(size_t sizeOfRegion, int allocationAlgo) {
    //checking for multiple umeminit calls
    AA = allocationAlgo;
    if (init) {
        fprintf(stderr, "umeminit is an initialization thus only needs to be called once.\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Invalid region size.\n");
        return -1;
    }

    size_t pageSize = getpagesize();
    sizeOfRegion = (sizeOfRegion + pageSize - 1) / pageSize;
    sizeOfRegion *= pageSize;
    int fd = open("/dev/zero", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    memRegion = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if (memRegion == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    luffy = (listThatIsFree*)memRegion;
    luffy -> size = sizeOfRegion - sizeof(listThatIsFree);
    luffy -> next = NULL;
    init = 1;

    umemdump();
    close(fd);
    return 0;
}

void *umalloc(size_t size) {
    //take input of size and return pointer to beginning of object with allocation strategy
    switch (AA) {
        case BEST_FIT:
            size = align(size);
            if (size == 0)
                return NULL;
            
            listThatIsFree* bf = NULL;
            listThatIsFree* lastBF = NULL;
            size_t bfSize = SIZE_MAX;
            present = luffy;

            while (present) {
                if (present -> size >= size && present -> size < bfSize) {
                    bf = present;
                    bfSize = present -> size;
                }
                past = present;
                present = present -> next;
            }

            if (bf) {
                if (bfSize > size) {
                    listThatIsFree* newWorld = (listThatIsFree*)((char*)bf + size + sizeof(listThatIsFree));
                    newWorld -> size = bfSize - size - sizeof(listThatIsFree);
                    newWorld -> next = bf -> next;
                    bf -> next = newWorld;
                    bf -> size = size;
                }
                void* eightBound = (void*)((char*)bf + sizeof(listThatIsFree));
                eightBound = (void*)align((size_t)eightBound);
                return eightBound;
            } 
            else {
                return NULL;
            }

        case WORST_FIT:
            present = luffy;
            listThatIsFree* wf = NULL;
            size_t wfSize = 0;

            while (present != NULL) {
                if (present -> size >= size && present -> size > wfSize) {
                    wf = present;
                    wfSize = present -> size;
                }
                present = present -> next;
            }

            if (wf == NULL) {
                return NULL;
            }

            if (wfSize > size + sizeof(listThatIsFree)) {
                listThatIsFree* newWorld = (listThatIsFree*)((char*)wf + size + sizeof(listThatIsFree));
                newWorld -> size = wfSize - size - sizeof(listThatIsFree);
                newWorld -> next = wf -> next;
                wf -> size = size;
                wf -> next = newWorld;
            }

            if (wf == luffy) {
                luffy = wf -> next;
            } else {
                present = luffy;
                while (present -> next != wf) {
                    present = present -> next;
                }
                present -> next = wf -> next;
            }
            return (void*)(wf + 1);

        case NEXT_FIT:

            listThatIsFree* present = zoro ? zoro : luffy;

            while (present != NULL) {
                if (present -> size >= size) {
                    if (present -> size > size + sizeof(listThatIsFree)) {
                        listThatIsFree* newWorld = (listThatIsFree*)((char*)present + size + sizeof(listThatIsFree));
                        newWorld -> size = present -> size - size - sizeof(listThatIsFree);
                        newWorld -> next = present -> next;
                        present -> size = size;
                        present -> next = newWorld;
                    }

                    if (past == NULL) {
                        luffy = present -> next;
                    } else {
                        past -> next = present -> next;
                    }
                    zoro = present -> next;
                    return (void*)(present + 1);
                }
                past = present;
                present = present -> next;
            }
            return NULL;

        case FIRST_FIT:
            present = luffy;

            while (present != NULL) {
                if (present -> size >= size) {
                    if (present -> size > size + sizeof(listThatIsFree)) {
                        listThatIsFree* newWorld = (listThatIsFree*)((char*)present + size + sizeof(listThatIsFree));
                        newWorld -> size = present -> size - size - sizeof(listThatIsFree);
                        newWorld -> next = present -> next;
                        present -> size = size;
                        present -> next = newWorld;
                    }

                    if (past == NULL) {
                        luffy = present -> next;
                    } else {
                        past -> next = present -> next;
                    }
                    return (void*)(present + 1);
                }
                past = present;
                present = present -> next;
            }
            return NULL;

        default:
            fprintf(stderr, "Invalid Allocation\n");
            return NULL;
    }
}

int ufree(void *ptr) {
    if (ptr == NULL) {
        return 0;
    }
    listThatIsFree* impelDown = (listThatIsFree*)ptr - 1;
    listThatIsFree* past = NULL;
    listThatIsFree* present = luffy;

    while (present != NULL) {
        if ((char*)present + present -> size + sizeof(listThatIsFree) == (char*)impelDown) {
            impelDown -> size += present -> size + sizeof(listThatIsFree);
            impelDown -> next = present -> next;
            if (past == NULL)
                luffy = impelDown;
            break;
        } else if ((char*)impelDown + impelDown -> size + sizeof(listThatIsFree) == (char*)present) {
            past -> size += impelDown -> size + sizeof(listThatIsFree);
            past -> next = present -> next;
            impelDown = past;
            break;
        } else {
            past = present;
            present = present -> next;
        }
    }

    if (impelDown != past) {
        if (past != NULL) {
            past -> next = impelDown;
        } else {
            luffy = impelDown;
        }
        impelDown -> next = present;
    }
    umemdump();
    return 0;
}

void umemdump() {
    //debug routine. will print regions of free memory to the screen
    printf("Memeory Dump - ");
    listThatIsFree* present = luffy;
    int counterspell = 0;
    int max = 10;
    while (present != NULL && counterspell < max) {
        printf("Chunk %d: Address %p, Size: %zu bytes, Next Chunk: %p\n", counterspell++, (void*)present, present -> size, (void*)present -> next);
        present = present -> next;
    }
    if (counterspell >= max) {
        printf("Too many chunks can't load or stuck in loop\n");
    }
}
