#ifndef HANDEVALUATION_H
#define HANDEVALUATION_H

#include <string>
#include "poker_evaluation/PokerHandType.h"

struct HandEvaluation {
    std::string handName;
    PokerHandType handType;

    HandEvaluation() : handName(""), handType(PokerHandType::HighCard) {}
    HandEvaluation(std::string name, PokerHandType type) : handName(name), handType(type) {}

    bool isValid() const {
        return !handName.empty();
    }
};

#endif // HANDEVALUATION_H
