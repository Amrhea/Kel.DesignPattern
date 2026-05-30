#include "joker/MultBoostJoker.h"
#include <iostream>

MultBoostJoker::MultBoostJoker(int bonus) : multBonus(bonus) {}

void MultBoostJoker::apply(ScoreContext& context) {
    context.mult += multBonus;
    std::cout << "[MultiplierJoker] Added +" << multBonus << " mult." << std::endl;
}

std::string MultBoostJoker::getName() const {
    return "MultiplierJoker";
}
