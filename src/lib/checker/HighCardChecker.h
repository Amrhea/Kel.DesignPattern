#pragma once

#include "../IPokerHandChecker.h"

class HighCardChecker : public IPokerHandChecker
{
    private:
        
    public:
        bool Check(const Hand& hand) override;
        bool IsHighCard(const Hand& hand);
};