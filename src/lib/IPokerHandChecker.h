#pragma once

#include "Hand.h"
#include "ChosenHand.h"
#include <iostream>
#include <memory>

class IPokerHandChecker
{
protected:
    std::unique_ptr<IPokerHandChecker> nextChecker;

public:
    IPokerHandChecker() : nextChecker(nullptr) {}
    virtual ~IPokerHandChecker() {}

    virtual ChosenHand Check(const Hand& hand) = 0;
    
    void SetNext(std::unique_ptr<IPokerHandChecker> next) {
        this->nextChecker = std::move(next);
    }

    IPokerHandChecker* GetNext() const {
        return nextChecker.get();
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
