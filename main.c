#include "umem.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int memIniTest(){
    // testing for initialization
    if (umeminit(1024, BEST_FIT) == 0) {
        printf("umeminit successful.\n");
    } else {
        printf("umeminit failed.\n");
    }

    return 0;
}

int main() {
    umeminit(1024, 1); //initialization
    memIniTest();
    void* eightBound = umalloc(64); //testing for umalloc allocating the memory
    if (eightBound) {
        printf("allo mem block at: %p\n", eightBound);
    }
    else {
        printf("mem allo fail\n");
    }
}