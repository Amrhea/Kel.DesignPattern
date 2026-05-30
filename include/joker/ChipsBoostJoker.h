#pragma once
#include "joker/Joker.h"

// PERSISTENT
class ChipsBoostJoker : public Joker {
private:
    int chipsBonus;

public:
    ChipsBoostJoker(int bonus);
    void apply(ScoreContext& context) override;
    std::string getName() const override;
};
