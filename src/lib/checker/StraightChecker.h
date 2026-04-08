#pragma once 

#include "../IPokerHandChecker.h"

class StraightChecker : public IPokerHandChecker
{
    private:
    
    public:
        bool Check(const Hand& hand) override;
        bool IsStright(const Hand& hand);
};

