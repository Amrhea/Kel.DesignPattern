#pragma once

#include "../IPokerHandChecker.h"

class ThreeOfKindChecker : public IPokerHandChecker
{
    private:
        
    public:
        bool Check(const Hand& hand) override;
        bool IsThreeOfKind(const Hand& hand);
};
