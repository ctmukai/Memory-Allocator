#include "umem.c"

int memIniTest(){
    // Example usage
    if (umeminit(4096, FIRST_FIT) == 0) {
        printf("umeminit successful.\n");
    } else {
        printf("umeminit failed.\n");
    }

    return 0;
}

void pageRoundTest() {
    size_t sizes[] = {0, 1, 4096, 8192, 63, 41, 33, 26};
    printf("testing roundPage function:\n");
    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        size_t original_size = sizes[i];
        size_t rounded_size = roundingPages(original_size);
        printf("Original size: %zu, Rounded size: %zu\n", original_size, rounded_size);
    }
}

int main() {
    memIniTest();
    pageRoundTest();
    return 0;
}