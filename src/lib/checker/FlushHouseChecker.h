#pragma once

#include "../IPokerHandChecker.h"

class FlushHouseChecker : public IPokerHandChecker
{
    private:
            
    public:
        bool Check(const Hand& hand) override;
        bool IsFlushHouse(const Hand& hand);
};