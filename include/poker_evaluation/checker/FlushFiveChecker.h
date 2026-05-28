#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class FlushFiveChecker : public IPokerHandChecker
{
public:
    HandEvaluation Check(const Hand& hand) override;
    bool IsFlushFive(const Hand& hand);
};
