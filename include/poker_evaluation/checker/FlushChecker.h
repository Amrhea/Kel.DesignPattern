#pragma once 

#include "poker_evaluation/IPokerHandChecker.h"

class FlushChecker : public IPokerHandChecker
{
    private:
        
    public:
        HandEvaluation Check(const Hand& hand) override;
        bool IsFlush(const Hand& hand);
};

