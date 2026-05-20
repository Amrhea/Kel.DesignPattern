#pragma once
#include "ScoreCalculator.h"

class StandardScoreCalculator : public ScoreCalculator {
protected:
    int ModifyScore(const std::string& handName, int baseScore) override {
        return baseScore;
    }
};

class BonusScoreCalculator : public ScoreCalculator {
protected:
    int ModifyScore(const std::string& handName, int baseScore) override {
        if (handName == "Flush" || handName == "Full House" || handName == "Straight") {
            return baseScore + 50;
        } else if (handName == "Four of a Kind" || handName == "Straight Flush" || handName == "Royal Flush" || handName == "Five of a Kind") {
            return baseScore + 150;
        }
        return baseScore + 10;
    }
};
