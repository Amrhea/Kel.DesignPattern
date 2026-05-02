#pragma once

#include "../IPokerHandChecker.h"

class RoyalFlushChecker : public IPokerHandChecker
{
    private:
    
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsRoyalFlush(const Hand& hand);

};
