#include "joker/ChipsBoostJoker.h"
#include <iostream>

ChipsBoostJoker::ChipsBoostJoker(int bonus) : chipsBonus(bonus) {}

void ChipsBoostJoker::apply(ScoreContext& context) {
    context.chips += chipsBonus;
    std::cout << "[ChipsBoostJoker] Added +" << chipsBonus << " chips." << std::endl;
}

std::string ChipsBoostJoker::getName() const {
    return "ChipsBoostJoker";
}
