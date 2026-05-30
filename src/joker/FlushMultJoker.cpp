#include "joker/FlushMultJoker.h"
#include <iostream>

FlushMultJoker::FlushMultJoker(int bonus) : targetHand(PokerHandType::Flush), multBonus(bonus) {}

FlushMultJoker::FlushMultJoker(PokerHandType handType, int bonus) : targetHand(handType), multBonus(bonus) {}

void FlushMultJoker::apply(ScoreContext& context) {
    if (context.handType == targetHand) {
        context.mult += multBonus;
        std::cout << "[ConditionalJoker] Hand matches! Added +" << multBonus << " mult." << std::endl;
    }
}

std::string FlushMultJoker::getName() const {
    return "ConditionalJoker";
}
