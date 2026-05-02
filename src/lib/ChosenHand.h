#ifndef CHOSENHAND_H
#define CHOSENHAND_H

#include <string>

struct ChosenHand {
    std::string handName;
    int baseScore;

    ChosenHand() : handName(""), baseScore(0) {}
    ChosenHand(std::string name, int score) : handName(name), baseScore(score) {}

    bool isValid() const {
        return !handName.empty();
    }
};

#endif // CHOSENHAND_H
