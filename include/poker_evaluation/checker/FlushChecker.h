#pragma once 

#include "poker_evaluation/IPokerHandChecker.h"

class FlushChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsFlush(const Hand& hand);
};

