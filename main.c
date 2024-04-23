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
//replaced by userMemAlgoTest to create a more dynamic testing approach
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
    size_t sizeOfRegion; //variables
    int allocationAlgo;
    //input size of the memory region that you want to declare
    printf("Enter size of memory region you want to allocate: ");
    scanf("%zu", &sizeOfRegion);
    //input allocation style
    printf("Enter which allocation algorithm you would like to use: BEST_FIT (1), WORST_FIT (2), FIRST_FIT (3), NEXT_FIT (4): ");
    scanf("%d", &allocationAlgo);
    //initialize the memory with the inputted options
    umeminit(sizeOfRegion, allocationAlgo);

    if (allocationAlgo < 1 || allocationAlgo > 4) {
        printf("Invalid Allocation Algorithm");
        return -1;
    } else {
        printf("Memory Initialization has been successfully ran\n");
        return 0;
    }
}

void coalTest() {
    // Initialize memory blocks
    testingGround* test = (testingGround*)umalloc(sizeof(testingGround));
    test -> size = 55;
    test -> next = NULL;

    testingGround* test2 = (testingGround*)umalloc(sizeof(testingGround));
    test2 -> size = 32;
    test2 -> next = NULL;

    testingGround* test3 = (testingGround*)umalloc(sizeof(testingGround));
    test3 -> size = 64;
    test3 -> next = NULL;

    // Create a "linked list"
    test -> next = test2;
    test2 -> next = test3;

    // Simulate coalescing by freeing test 2
    ufree(test2 + 1);

    // now check if coalescing occurred
    assert(test -> size == 55 + 32 + (7 * sizeof(testingGround)));
    assert(test -> next == test3);
    //* side not cant get assertion to work, might have something to do with how i did my allocation styles and my ufree? *
    printf("Coalescing test passed!\n");

    return;
}

int main() {
    userMemAlgoTest(); //run my allocation test
    umemdump(); //check the chunk of memory the current pointer is at
    int* ptr = NULL; //create new ptr
    coalTest();
    assert(ufree(ptr) == 0);
    umemdump();
    int* arr = (int*)umalloc(1);
    //assert(1024);
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
    umalloc(0); //allocate to new chunk
    umemdump();
    ufree(eightBound); //free memory and then check for changes
    umemdump();
}