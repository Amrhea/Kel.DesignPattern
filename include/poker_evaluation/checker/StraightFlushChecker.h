#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class StraightFlushChecker : public IPokerHandChecker
{
    private:
        
    public:
        HandEvaluation Check(const Hand& hand) override;
        bool IsStraightFlush(const Hand& hand);
        
};

