#pragma once
#include "hand_selection/Hand.h"
#include "poker_evaluation/HandEvaluation.h"
#include "poker_evaluation/PokerHandEvaluator.h"
#include <string>
#include <map>

class ScoreCalculator {
public:
    virtual ~ScoreCalculator() = default;

    // The Template Method defining the algorithm's skeleton
    int CalculateScore(const Hand& hand, PokerHandEvaluator& handler) {
        // Step 1: Check Poker Hand
        HandEvaluation chosen = CheckPokerHand(hand, handler);
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
    virtual HandEvaluation CheckPokerHand(const Hand& hand, PokerHandEvaluator& handler) {
        return handler.Evaluate(hand);
    }

    virtual int GetBaseScore(const HandEvaluation& chosen) {
        // Simple mapping for now to replace the old chosen.baseScore
        static std::map<PokerHandType, int> scores = {
            {PokerHandType::HighCard, 10},
            {PokerHandType::Pair, 20},
            {PokerHandType::TwoPair, 30},
            {PokerHandType::ThreeOfKind, 40},
            {PokerHandType::Straight, 60},
            {PokerHandType::Flush, 80},
            {PokerHandType::FullHouse, 100},
            {PokerHandType::FlushHouse, 140},
            {PokerHandType::FourOfKind, 150},
            {PokerHandType::StraightFlush, 250},
            {PokerHandType::RoyalFlush, 500},
            {PokerHandType::FiveOfKind, 1000},
            {PokerHandType::FlushFive, 2000}
        };
        return scores[chosen.handType];
    }

    // Step 3 is primitive operation to be overridden by subclasses
    virtual int ModifyScore(const std::string& handName, int baseScore) = 0;
};
