#include "lib/JokerCard.h"
#include <iostream>

JokerCard::JokerCard(const std::string& name, int bonusPoints)
    : name(name), bonusPoints(bonusPoints) {}

void JokerCard::OnHandPlayed(const std::string& handName, int& score) {
    std::cout << "[Joker: " << name << "] Triggered! Adding +" << bonusPoints << " to score." << std::endl;
    score += bonusPoints;
}

std::string JokerCard::GetName() const {
    return name;
}
