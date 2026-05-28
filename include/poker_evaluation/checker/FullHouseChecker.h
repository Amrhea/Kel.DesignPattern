#pragma once

#include "poker_evaluation/IPokerHandChecker.h"

class FullHouseChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsFullHouse(const Hand& hand);
};
