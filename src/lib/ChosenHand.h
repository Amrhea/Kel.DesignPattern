#ifndef CHOSENHAND_H
#define CHOSENHAND_H

#include <string>
#include "PokerHandType.h"

struct ChosenHand {
    std::string handName;
    PokerHandType handType;
    int baseScore;

    ChosenHand() : handName(""), handType(PokerHandType::HighCard), baseScore(0) {}
    ChosenHand(std::string name, int score) : handName(name), handType(PokerHandType::HighCard), baseScore(score) {}
    ChosenHand(std::string name, PokerHandType type, int score) : handName(name), handType(type), baseScore(score) {}

    bool isValid() const {
        return !handName.empty();
    }
};

#endif // CHOSENHAND_H
