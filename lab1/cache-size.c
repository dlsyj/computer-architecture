#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define TOTALREAD (1ULL << 32)

uint64_t* data;

void init(int elemNum, int stride) { // Initialize as linked list
    int i;
    for (i = 0; i < (elemNum >> 1); i += stride) {
        data[i] = (uint64_t) &data[i + (elemNum >> 1)];
        data[i + (elemNum >> 1)] = (uint64_t) &data[i + stride];
    }
    data[i + (elemNum >> 1) - stride] = 0;
}

inline void test() { // Chase list to the end
    uint64_t* cur = data;
    while (*cur) {
        cur = (uint64_t*) *cur;
    }
}

double run(int elemNum, int stride) {
    double cycles;
    clock_t start;
    uint64_t iterNum = (TOTALREAD / (elemNum * sizeof(uint64_t))) * stride;
    init(elemNum, stride);
    test(); // Warm up
    start = clock();
    while (iterNum--) { // Repeat
        test();
    }
    cycles = (double) (clock() - start);
    return ((TOTALREAD / (1 << 20)) * CLOCKS_PER_SEC) / cycles;
}

int main(int argc, char** argv) {
    assert(argc == 3);
    int start = atoi(argv[1]);
    int end = atoi(argv[2]);
    int elemNum;
    for (elemNum = start * 1024 / sizeof(uint64_t); elemNum <= end * 1024 / sizeof(uint64_t); elemNum <<= 1) { // For every size between
        data = (uint64_t*) calloc(elemNum, sizeof(uint64_t));
        printf("size: %dKB, throughput: %lf\n", elemNum * sizeof(uint64_t) / 1024, run(elemNum, 32));
    }
    return 0;
}

