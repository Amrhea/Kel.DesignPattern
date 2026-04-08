#pragma once 

#include "../IPokerHandChecker.h"

class FourOfKindChecker : public IPokerHandChecker
{
    private:
       
    public:
        bool Check(const Hand& hand) override;
        bool IsFourOfKind(const Hand& hand);
};
