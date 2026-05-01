#pragma once

#include "../IPokerHandChecker.h"

class FiveOfKindChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
       bool IsFiveOfKind(const Hand& hand);
};

