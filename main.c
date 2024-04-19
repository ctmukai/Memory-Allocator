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
    //initialization
    int result = umeminit(80983, WORST_FIT); //testing if initialization requests correct # of bytes
    if (result == 1) {
        fprintf(stderr, "umeminit failed.\n");
        return 1;
    }
    return 0;
    umemdump();
    /*memIniTest();
    void* eightBound = umalloc(64); //testing for umalloc allocating the memory
    if (eightBound) {
        printf("allo mem block at: %p\n", eightBound);
    }
    else {
        printf("mem allo fail\n");
    }*/
}