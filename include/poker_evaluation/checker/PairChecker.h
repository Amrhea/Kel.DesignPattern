#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class PairChecker : public IPokerHandChecker
{
    private:
    
    public:
        HandEvaluation Check(const Hand& hand) override;
        bool IsPair(const Hand& hand);
    
};

