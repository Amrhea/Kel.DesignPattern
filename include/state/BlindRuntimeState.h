#pragma once

// RUNTIME
struct BlindRuntimeState {
    int blindScore;
    int remainingHands;
    int remainingDiscards;

    BlindRuntimeState()
        : blindScore(0), remainingHands(4), remainingDiscards(3) {}

    void reset(int handsLimit = 4, int discardsLimit = 3);
};
