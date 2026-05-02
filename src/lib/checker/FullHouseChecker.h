#pragma once

#include "../IPokerHandChecker.h"

class FullHouseChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsFullHouse(const Hand& hand);
};