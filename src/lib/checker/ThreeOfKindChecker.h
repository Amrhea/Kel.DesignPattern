#pragma once

#include "../IPokerHandChecker.h"

class ThreeOfKindChecker : public IPokerHandChecker
{
    private:
        
    public:
        ChosenHand Check(const Hand& hand) override;
        bool IsThreeOfKind(const Hand& hand);
};
