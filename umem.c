#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "umem.h"
#define align(size) ((size + 7) & ~(8))
#define pageSize getpagesize()

//Free list structure for memory chunk
typedef struct listThatIsFree {
    size_t size;
    struct listThatIsFree *next;
} listThatIsFree;

//Global Variables
static int init = 0;
static int AA = -1;
static void *memRegion = NULL;

listThatIsFree *luffy = NULL;

//--------------------------------------------------//
//                                                  //
//                   Helper Functions               //
//                                                  //      
//--------------------------------------------------//
size_t roundingPages(size_t size) {
    size_t remain = size % pageSize;
    if (remain == 0) {
        return size;
    }
    else {
        return size + (pageSize - remain);
    }
}


//----------------------------------------//
//                                        //
//                  Debug                 //
//                                        //
//----------------------------------------//

//testing roundingPages functionality
void pageRoundTest() {
    size_t sizes[] = {0, 1, 4096, 8192, 63, 41, 33, 26};
    printf("testing roundingPage function:\n");
    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        size_t original_size = sizes[i];
        size_t rounded_size = roundingPages(original_size);
        printf("Original size: %zu, Rounded size: %zu\n", original_size, rounded_size);
    }
}

int umeminit(size_t sizeOfRegion, int allocationAlgo) {
    //checking for multiple umeminit calls
    if (init) {
        fprintf(stderr, "umeminit is an initialization thus only needs to be called once.\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Invalid region size.\n");
        return -1;
    }

    AA = allocationAlgo;
    init = 1;

    sizeOfRegion = roundingPages(sizeOfRegion);
    memRegion = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);

    if (memRegion == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    luffy = (listThatIsFree*)memRegion;
    luffy->size = sizeOfRegion - sizeof(listThatIsFree);
    luffy->next = NULL;

    return 0;
}

void *umalloc(size_t size) {
    int allocation;
    //take input of size and return pointer to beginning of object
    switch (allocation) {
        case BEST_FIT:
            size = align(size);
            if (size == 0)
                return NULL;
            
        case WORST_FIT:
        //next fit
        case NEXT_FIT:
        //first fit
        case FIRST_FIT:

        default:
            fprintf(stderr, "Invalid Allocation\n");
            return NULL;
    }
}

int ufree(void *ptr) {
    //coalesce free space
    return 0;
}

void umemdump() {
    //debug routine. will print regions of free memory to the screen
}

