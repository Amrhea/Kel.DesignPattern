#pragma once

#include "../IPokerHandChecker.h"

class StraightFlushChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsStraightFlush(const Hand& hand);
        
};
