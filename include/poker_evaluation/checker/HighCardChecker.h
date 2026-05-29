#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class HighCardChecker : public IPokerHandChecker
{
    private:
        
    public:
        HandEvaluation Check(const Hand& hand) override;
        bool IsHighCard(const Hand& hand);
};
