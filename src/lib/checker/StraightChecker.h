#pragma once 

#include "../IPokerHandChecker.h"

class StraightChecker : public IPokerHandChecker
{
    private:
    
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsStraight(const Hand& hand);
};

