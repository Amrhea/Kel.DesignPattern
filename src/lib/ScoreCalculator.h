#pragma once
#include "Hand.h"
#include "ChosenHand.h"
#include "HandHandler.h"
#include <string>

class ScoreCalculator {
public:
    virtual ~ScoreCalculator() = default;

    // The Template Method defining the algorithm's skeleton
    int CalculateScore(const Hand& hand, HandHandler& handler) {
        // Step 1: Check Poker Hand
        ChosenHand chosen = CheckPokerHand(hand, handler);
        if (!chosen.isValid()) {
            return 0;
        }

        // Step 2: Get Base Score
        int baseScore = GetBaseScore(chosen);

        // Step 3: Modify Score
        int finalScore = ModifyScore(chosen.handName, baseScore);

        return finalScore;
    }

protected:
    virtual ChosenHand CheckPokerHand(const Hand& hand, HandHandler& handler) {
        return handler.evaluate(hand);
    }

    virtual int GetBaseScore(const ChosenHand& chosen) {
        return chosen.baseScore;
    }

    // Step 3 is primitive operation to be overridden by subclasses
    virtual int ModifyScore(const std::string& handName, int baseScore) = 0;
};
