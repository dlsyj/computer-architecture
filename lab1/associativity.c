#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define TOTALREAD (1ULL << 32)
#define CACHE_SIZE (256 * 1024)
#define BLOCK_SIZE (1 << 6)

uint64_t* data;

void init(uint64_t mask) {
    int num = CACHE_SIZE / mask;
    uint64_t* cur = data;
    while (--num) {
        *cur = (uint64_t)cur + mask;
        cur = (uint64_t*) *cur;
    }
    *cur = 0;
}

inline void test() {
    uint64_t* cur = data;
    while (*cur) {
        cur = (uint64_t*) *cur;
    }
}

double run(uint64_t mask) {
    double cycles;
    clock_t start;
    uint64_t iterNum = TOTALREAD * mask / (CACHE_SIZE * sizeof(uint64_t));
    init(mask);
    test(); // Warm up
    start = clock();
    while (iterNum--) { // Repeat
        test();
    }
    cycles = (double) (clock() - start);
    return ((TOTALREAD / (1 << 20)) * CLOCKS_PER_SEC) / cycles;
}

int main(int argc, char** argv) {
    uint64_t mask;
    data = (uint64_t*) calloc(CACHE_SIZE / sizeof(uint64_t), sizeof(uint64_t));
    for (mask = BLOCK_SIZE; mask < CACHE_SIZE; mask <<= 1) {
        printf("mask: %d, throughput: %lf\n", mask, run(mask));
    }
    free(data);
    return 0;
}

