#pragma once

#include "poker_evaluation/IPokerHandChecker.h"
#include <memory>
#include <vector>

class PokerHandEvaluator {
private:
    std::unique_ptr<IPokerHandChecker> head;

public:
    PokerHandEvaluator();
    ~PokerHandEvaluator();

    void AddChecker(std::unique_ptr<IPokerHandChecker> checker);
    HandEvaluation Evaluate(const Hand& hand);
};
