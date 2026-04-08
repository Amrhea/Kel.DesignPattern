#pragma once

#include "../IPokerHandChecker.h"

class PairChecker : public IPokerHandChecker
{
    private:
    
    public:
        bool Check(const Hand& hand) override;
        bool IsPair(const Hand& hand);
    
};
