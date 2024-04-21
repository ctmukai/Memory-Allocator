#include "umem.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef struct testingGround {
    size_t size;  // Size of the memory block
    struct testingGround* next;  // Pointer to the next block
} testingGround;

/*int memIniTest(){
    // testing for initialization
    if (umeminit(1024, FIRST_FIT) == 0) {
        printf("umeminit successful.\n");
    } else {
        printf("umeminit failed.\n");
    }

    return 0;
}*/

int userMemAlgoTest() {
    size_t sizeOfRegion;
    int allocationAlgo;

    printf("Enter size of memory region you want to allocate: ");
    scanf("%zu", &sizeOfRegion);

    printf("Enter which allocation algorithm you would like to use: BEST_FIT (1), WORST_FIT (2), FIRST_FIT (3), NEXT_FIT (4): ");
    scanf("%d", &allocationAlgo);

    umeminit(sizeOfRegion, allocationAlgo);
    printf("Memory Initialization has been successfully ran\n");
    return 0;
}

void coalTest() {
    // Initialize memory blocks
    testingGround* test = (testingGround*)umalloc(sizeof(testingGround));
    test -> size = 20;
    test -> next = NULL;

    testingGround* test2 = (testingGround*)umalloc(sizeof(testingGround));
    test2 -> size = 10;
    test2 -> next = NULL;

    testingGround* test3 = (testingGround*)umalloc(sizeof(testingGround));
    test3 -> size = 15;
    test3 -> next = NULL;

    // Create a linked list
    test -> next = test2;
    test2 -> next = test3;

    // Simulate coalescing by freeing block2
    ufree(test2 + 1);

    // Check if coalescing occurred
    assert(test -> size == 20 + 10 + 2 * sizeof(testingGround));
    assert(test -> next == test3);

    printf("Coalescing test passed!\n");

    return;
}

int main() {
    //initialization
    /*int result = umeminit(1024, BEST_FIT); //testing if initialization requests correct # of bytes
    if (result == 1) {
        fprintf(stderr, "umeminit failed.\n");
        return 1;
    }*/
    userMemAlgoTest();
    umemdump();
    coalTest();
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