#pragma once 

#include "poker_evaluation/IPokerHandChecker.h"

class StraightChecker : public IPokerHandChecker
{
    private:
    
    public:
        HandEvaluation Check(const Hand& hand) override;
        bool IsStraight(const Hand& hand);
};


