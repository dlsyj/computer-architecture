#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define TOTALREAD (1ULL << 32)
#define L1_CACHE_SIZE (32 * 1024)
#define L2_CACHE_SIZE (256 * 1024)
#define BLOCK_SIZE (1 << 6)

uint64_t* data;

void init(uint64_t size) { // Initialize memory
    int i;
    for (i = 0; i < size / sizeof(uint64_t); i += BLOCK_SIZE / sizeof(uint64_t)) {
        data[i] = (uint64_t) &data[i + BLOCK_SIZE / sizeof(uint64_t)];
    }
    data[i - BLOCK_SIZE / sizeof(uint64_t)] = 0;
}

inline void test() {
    uint64_t* cur = data;
    while (*cur) {
        ++*(cur + 1);
        cur = (uint64_t*) *cur;
    }
}

double run(uint64_t size) {
    double cycles;
    clock_t start;
    uint64_t iterNum = TOTALREAD * BLOCK_SIZE / (size * sizeof(uint64_t));
    init(size);
    test(); // Warm up
    start = clock();
    while (iterNum--) { // Repeat
        test();
    }
    cycles = (double) (clock() - start);
    return ((TOTALREAD / (1 << 20)) * CLOCKS_PER_SEC) / cycles;
}

int main(int argc, char** argv) {
    uint64_t size;
    data = (uint64_t*) calloc(L2_CACHE_SIZE / sizeof(uint64_t), sizeof(uint64_t));
    for (size = L1_CACHE_SIZE; size <= 2 * L1_CACHE_SIZE; size <<=1) {
        printf("size: %d, throughput: %lf\n", size, run(size));
    }
    free(data);
    return 0;
}

