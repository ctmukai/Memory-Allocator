#include "umem.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define align(size) ((size + 7) & ~(8))



int umeminit(size_t sizeOfRegion, int allocationAlgo) {
    //code

    int fd = open("/dev/zero", O_RDWR);
    void *ptr = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);
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
}

void umemdump {
    //debug routine. will print regions of free memory to the screen
}



