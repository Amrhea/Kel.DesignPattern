#pragma once

#include "../IPokerHandChecker.h"

class PairChecker : public IPokerHandChecker
{
    private:
    
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsPair(const Hand& hand);
    
};
