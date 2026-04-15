#pragma once

#include "Hand.h"
#include <iostream>

class IPokerHandChecker
{
protected:
    IPokerHandChecker* nextChecker;

public:
    IPokerHandChecker() : nextChecker(nullptr) {}
    virtual ~IPokerHandChecker() {}

    virtual bool Check(const Hand& hand) = 0;
    void SetNext(IPokerHandChecker* next) {
        this->nextChecker = next;
    }

    IPokerHandChecker* GetNext() const {
        return nextChecker;
    }

    // Method untuk meneruskan permintaan ke checker berikutnya
    bool Handle(const Hand& hand) {
        if (Check(hand)) {
            return true;
        }
        if (nextChecker != nullptr) {
            return nextChecker->Handle(hand);
        }
        return false;
    }
};
