#include "umem.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

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
    int result = umeminit(1024, BEST_FIT); //testing if initialization requests correct # of bytes
    if (result == 1) {
        fprintf(stderr, "umeminit failed.\n");
        return 1;
    }
    umemdump();
    memIniTest();
    int* ptr = NULL;
    assert(ufree(ptr) == 0);
    umemdump();
    int* arr = (int*)umalloc(1);
    assert(1024);
    ufree(arr);
    umemdump();
    void* eightBound = umalloc(64); //testing for umalloc allocating the memory
    if (eightBound) {
        printf("allo mem block at: %p\n", eightBound);
    }
    else {
        printf("mem allo fail\n");
    }
    umemdump();
}