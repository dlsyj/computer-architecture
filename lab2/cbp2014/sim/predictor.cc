#include "predictor.h"

#define PHT_CTR_MAX 3
#define PHT_CTR_INIT 2
#define HIST_LEN 17

PREDICTOR::PREDICTOR() {
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
        meta[i] = 2;
    }
}

PREDICTOR::~PREDICTOR() {
    delete[] meta;
    delete[] globalHistory;
    delete[] localHistoryCounter;
    delete[] localHistory;
}

bool PREDICTOR::GetPrediction(UINT32 PC) {
    UINT32 phtIndex = (PC ^ ghr) % (numPhtEntries);
    UINT32 phtCounter = pht[phtIndex];
    if(phtCounter > PHT_CTR_MAX/2){
        return TAKEN;
    }else{
        return NOT_TAKEN;
    }
}

void  PREDICTOR::UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget){

    UINT32 phtIndex   = (PC^ghr) % (numPhtEntries);
    UINT32 phtCounter = pht[phtIndex];

    // update the PHT

    if(resolveDir == TAKEN){
        pht[phtIndex] = SatIncrement(phtCounter, PHT_CTR_MAX);
    }else{
        pht[phtIndex] = SatDecrement(phtCounter);
    }

    // update the GHR
    ghr = (ghr << 1);

    if(resolveDir == TAKEN){
        ghr++;
    }

}

void    PREDICTOR::TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget){

    // This function is called for instructions which are not
    // conditional branches, just in case someone decides to design
    // a predictor that uses information from such instructions.
    // We expect most contestants to leave this function untouched.

    return;
}

