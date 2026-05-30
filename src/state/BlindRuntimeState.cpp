#include "state/BlindRuntimeState.h"

void BlindRuntimeState::reset(int handsLimit, int discardsLimit) {
    blindScore = 0;
    remainingHands = handsLimit;
    remainingDiscards = discardsLimit;
}
