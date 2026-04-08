#pragma once

#include "Hand.h"

class IPokerHandChecker
{
    protected:
        IPokerHandChecker* nextChecker = nullptr;
    

    public:
        virtual bool Check(const Hand& hand) = 0;
        void SetNext(IPokerHandChecker* next);
};