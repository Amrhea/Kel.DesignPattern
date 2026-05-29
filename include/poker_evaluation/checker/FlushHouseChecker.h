#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class FlushHouseChecker : public IPokerHandChecker
{
    private:
            
    public:
        HandEvaluation Check(const Hand& hand) override;
        bool IsFlushHouse(const Hand& hand);
};
