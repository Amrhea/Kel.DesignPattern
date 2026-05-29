#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class FiveOfKindChecker : public IPokerHandChecker
{
    private:
        
    public:
        HandEvaluation Check(const Hand& hand) override;
       bool IsFiveOfKind(const Hand& hand);
};


