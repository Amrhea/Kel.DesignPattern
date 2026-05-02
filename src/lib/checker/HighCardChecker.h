#pragma once

#include "../IPokerHandChecker.h"

class HighCardChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsHighCard(const Hand& hand);
};