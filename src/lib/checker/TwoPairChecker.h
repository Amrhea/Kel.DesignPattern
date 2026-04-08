#pragma once

#include "../IPokerHandChecker.h"

class TwoPairChecker : public IPokerHandChecker
{
    private:
        
    public:
        bool Check(const Hand& hand) override;
        bool IsTwoPair(const Hand& hand);    
};
