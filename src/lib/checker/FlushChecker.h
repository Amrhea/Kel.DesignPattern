#pragma once 

#include "../IPokerHandChecker.h"

class FlushChecker : public IPokerHandChecker
{
    private:
        
    public:
        bool Check(const Hand& hand) override;
        bool IsFlush(const Hand& hand);
};
