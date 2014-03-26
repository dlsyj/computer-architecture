#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define ITER_NUM (1 << 15)
#define L2_CACHE_SIZE (256 * 1024)
#define INCREMENT_TAG_SIZE (4096)
#define BLOCK_SIZE (64)

uint64_t* data;

void initFifo() { // Sequence for FIFO
    uint8_t* store = (uint8_t*) (data + BLOCK_SIZE / sizeof(uint64_t));
    store[0] = 0;
    store[1] = 8;
    store[2] = 1;
    store[3] = 1;
    store[4] = 0;
    store[5] = 2;
    store[6] = 2;
    store[7] = 1;
    store[8] = 3;
    store[9] = 3;
    store[10] = 2;
    store[11] = 4;
    store[12] = 4;
    store[13] = 3;
    store[14] = 5;
    store[15] = 5;
    store[16] = 4;
    store[17] = 6;
    store[18] = 6;
    store[19] = 5;
    store[20] = 7;
    store[21] = 7;
    store[22] = 6;
    store[23] = 8;
    store[24] = 8;
    store[25] = 7;
    store[26] = 0;
}

void initLru() { // Sequence for LRu
    uint8_t* store = (uint8_t*) (data + BLOCK_SIZE / sizeof(uint64_t));
    store[0] = 0;
    store[1] = 8;
    store[2] = 0;
    store[3] = 2;
    store[4] = 1;
    store[5] = 2;
    store[6] = 4;
    store[7] = 3;
    store[8] = 4;
    store[9] = 6;
    store[10] = 5;
    store[11] = 6;
    store[12] = 8;
    store[13] = 7;
    store[14] = 8;
    store[15] = 1;
    store[16] = 0;
    store[17] = 1;
    store[18] = 3;
    store[19] = 2;
    store[20] = 3;
    store[21] = 5;
    store[22] = 4;
    store[23] = 5;
    store[24] = 7;
    store[25] = 6;
    store[26] = 7;
}

inline void test() {
    volatile int sum = 0;
    int i;
    for (i = 0; i < 27; ++i) {
        sum += *(data + *(((uint8_t*) (data + BLOCK_SIZE / sizeof(uint64_t))) + i) * INCREMENT_TAG_SIZE / sizeof(uint64_t));
    }
}

void run() {
    int cycles;
    clock_t start;
    int iterNum = ITER_NUM;
    initFifo();
    test(); // Warm up
    start = clock();
    while (iterNum--) { // Repeat
        test();
    }
    cycles = (double) (clock() - start);
    printf("FIFO: %d\n", cycles);
    iterNum = ITER_NUM;
    initLru();
    test(); // Warm up
    start = clock();
    while (iterNum--) { // Repeat
        test();
    }
    cycles = (double) (clock() - start);
    printf("LRU: %d\n", cycles);
}

int main(int argc, char** argv) {
    data = (uint64_t*) calloc(L2_CACHE_SIZE / sizeof(uint64_t), sizeof(uint64_t));
    assert(!(((uint64_t) data) & 0x3f));
    run();
    free(data);
    return 0;
}

