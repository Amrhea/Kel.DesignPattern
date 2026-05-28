#pragma once

#include "hand_selection/Hand.h"
#include "poker_evaluation/HandEvaluation.h"
#include <iostream>
#include <memory>

class IPokerHandChecker
{
protected:
    std::unique_ptr<IPokerHandChecker> nextChecker;

public:
    IPokerHandChecker() : nextChecker(nullptr) {}
    virtual ~IPokerHandChecker() {}

    virtual HandEvaluation Check(const Hand& hand) = 0;
    
    void SetNext(std::unique_ptr<IPokerHandChecker> next) {
        this->nextChecker = std::move(next);
    }

    IPokerHandChecker* GetNext() const {
        return nextChecker.get();
    }

    // Method untuk meneruskan permintaan ke checker berikutnya
    virtual HandEvaluation Handle(const Hand& hand) {
        HandEvaluation result = Check(hand);
        if (result.isValid()) {
            return result;
        }
        if (nextChecker != nullptr) {
            return nextChecker->Handle(hand);
        }
        return HandEvaluation(); // Return invalid if no checker matches
    }
};
