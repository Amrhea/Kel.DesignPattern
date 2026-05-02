#pragma once

#include "Hand.h"
#include "ChosenHand.h"
#include <iostream>

class IPokerHandChecker
{
protected:
    IPokerHandChecker* nextChecker;

public:
    IPokerHandChecker() : nextChecker(nullptr) {}
    virtual ~IPokerHandChecker() {}

    virtual ChosenHand Check(const Hand& hand) = 0;
    void SetNext(IPokerHandChecker* next) {
        this->nextChecker = next;
    }

    IPokerHandChecker* GetNext() const {
        return nextChecker;
    }

    // Method untuk meneruskan permintaan ke checker berikutnya
    virtual ChosenHand Handle(const Hand& hand) {
        ChosenHand result = Check(hand);
        if (result.isValid()) {
            return result;
        }
        if (nextChecker != nullptr) {
            return nextChecker->Handle(hand);
        }
        return ChosenHand(); // Return invalid if no checker matches
    }
};
