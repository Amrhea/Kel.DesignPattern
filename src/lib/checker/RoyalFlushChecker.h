#pragma once

#include "../IPokerHandChecker.h"

class RoyalFlushChecker : public IPokerHandChecker
{
    private:
    
    public:
        bool Check(const Hand& hand) override;
        bool IsRoyalFlush(const Hand& hand);

};
