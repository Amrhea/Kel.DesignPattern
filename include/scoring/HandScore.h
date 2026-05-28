#pragma once

#include "poker_evaluation/PokerHandType.h"
#include <unordered_map>

struct HandScoreData {
    int baseChips;
    int baseMult;
    int level;

    HandScoreData() : baseChips(0), baseMult(0), level(1) {}
    HandScoreData(int chips, int mult, int lvl = 1) 
        : baseChips(chips), baseMult(mult), level(lvl) {}
};

struct PlayedHandResult {
    const PokerHandType handType;
    const int chips;
    const int mult;
    const int level;
    const int finalScore;

    PlayedHandResult(PokerHandType type, int c, int m, int l)
        : handType(type), chips(c), mult(m), level(l), finalScore(c * m) {}
};

using HandScoreTable = std::unordered_map<PokerHandType, HandScoreData>;
