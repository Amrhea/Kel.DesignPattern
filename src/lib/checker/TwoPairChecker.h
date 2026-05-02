#pragma once

#include "../IPokerHandChecker.h"

class TwoPairChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsTwoPair(const Hand& hand);    
};
