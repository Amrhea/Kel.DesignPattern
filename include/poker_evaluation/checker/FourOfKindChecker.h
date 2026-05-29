#pragma once 

#include "poker_evaluation/IPokerHandChecker.h"

class FourOfKindChecker : public IPokerHandChecker
{
    private:
       
    public:
        HandEvaluation Check(const Hand& hand) override;
        bool IsFourOfKind(const Hand& hand);
};

