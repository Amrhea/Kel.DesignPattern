#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class ThreeOfKindChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsThreeOfKind(const Hand& hand);
};

