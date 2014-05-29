#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include "utils.h"
#include "tracer.h"

class PREDICTOR {
private:
    UINT32 ghr;
    UINT32 m;
    UINT32 n;
    UINT32* localHistory;
    UINT32* localHistoryCounter;
    UINT32* globalHistory;
    UINT32* meta;
public:
    PREDICTOR();
    ~PREDICTOR();
    bool GetPrediction(UINT32 PC);
    void UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget);
    void TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget);
};

#endif

