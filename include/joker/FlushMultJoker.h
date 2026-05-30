#pragma once
#include "joker/Joker.h"

// PERSISTENT
class FlushMultJoker : public Joker {
private:
    PokerHandType targetHand;
    int multBonus;

public:
    FlushMultJoker(int bonus);
    FlushMultJoker(PokerHandType handType, int bonus);
    void apply(ScoreContext& context) override;
    std::string getName() const override;
};
