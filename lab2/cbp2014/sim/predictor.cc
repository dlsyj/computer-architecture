#include "predictor.h"

#define PHT_CTR_MAX 3
#define PHT_CTR_INIT 2
#define HIST_LEN 17

void up(UINT32* v) {
    if (*v < 3) {
        ++*v;
    }
}

void down(UINT32* v) {
    if (0 < *v) {
        --*v;
    }
}

UINT32 lowerBits(UINT32 v, int c) {
    return v & ((1 << c) - 1);
}

PREDICTOR::PREDICTOR() {
    ghr = 0;
    m = 14;
    n = 13;
    localHistory = new UINT32[1 << m]();
    localHistoryCounter = new UINT32[1 << n];
    for (int i = 0; i < (1 << n); ++i) {
        localHistoryCounter[i] = 2;
    }
    globalHistory = new UINT32[1 << n];
    for (int i = 0; i < (1 << n); ++i) {
        globalHistory[i] = 2;
    }
    meta = new UINT32[1 << n];
    for (int i = 0; i < (1 << n); ++i) {
        meta[i] = 1;
    }
}

PREDICTOR::~PREDICTOR() {
    delete[] meta;
    delete[] globalHistory;
    delete[] localHistoryCounter;
    delete[] localHistory;
}

bool PREDICTOR::GetPrediction(UINT32 PC) {
    UINT32 xored = lowerBits(PC ^ ghr, n);
    if (meta[xored] / 2) { // Prefer gshare
        return globalHistory[xored] / 2;
    } else { // Prefer local
        return localHistoryCounter[localHistory[lowerBits(PC, m)]] / 2;
    }
}

void PREDICTOR::UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {
    bool localPred, globalPred;
    UINT32 xored = lowerBits(PC ^ ghr, n);
    localPred = localHistoryCounter[localHistory[lowerBits(PC, m)]] / 2;
    globalPred = globalHistory[xored] / 2;
    if (globalPred != localPred) {
        if (resolveDir == globalPred) {
            up(&meta[xored]);
        } else {
            down(&meta[xored]);
        }
    }
    if (resolveDir) {
        up(&localHistoryCounter[localHistory[lowerBits(PC, m)]]);
        localHistory[lowerBits(PC, m)] = lowerBits((localHistory[lowerBits(PC, m)] << 1) + 1, n);
        up(&globalHistory[xored]);
    } else {
        down(&localHistoryCounter[localHistory[lowerBits(PC, m)]]);
        localHistory[lowerBits(PC, m)] = lowerBits(localHistory[lowerBits(PC, m)] << 1, n);
        down(&globalHistory[xored]);
    }
    // update the GHR
    ghr = (ghr << 1);
    if(resolveDir == TAKEN){
        ghr++;
    }
}

void PREDICTOR::TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget) {
    // This function is called for instructions which are not
    // conditional branches, just in case someone decides to design
    // a predictor that uses information from such instructions.
    // We expect most contestants to leave this function untouched.
    return;
}

