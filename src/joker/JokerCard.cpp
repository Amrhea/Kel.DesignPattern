#include "joker/JokerCard.h"
#include "scoring/ScoreContext.h"
#include <iostream>

JokerCard::JokerCard(const std::string& name, int bonusPoints)
    : name(name), bonusPoints(bonusPoints) {}

void JokerCard::apply(ScoreContext& context) {
    std::cout << "[Joker: " << name << "] Triggered! Adding +" << bonusPoints << " to chips." << std::endl;
    context.chips += bonusPoints;
}

std::string JokerCard::getName() const {
    return name;
}
