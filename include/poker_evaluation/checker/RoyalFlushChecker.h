#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class RoyalFlushChecker : public IPokerHandChecker
{
    private:
    
    public:
        HandEvaluation Check(const Hand& hand) override;
        bool IsRoyalFlush(const Hand& hand);

};

