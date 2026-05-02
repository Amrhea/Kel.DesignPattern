#pragma once

#include "../IPokerHandChecker.h"

class FlushHouseChecker : public IPokerHandChecker
{
    private:
            
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsFlushHouse(const Hand& hand);
};