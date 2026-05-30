#pragma once
#include "joker/Joker.h"

// PERSISTENT
class MultBoostJoker : public Joker {
private:
    int multBonus;

public:
    MultBoostJoker(int bonus);
    void apply(ScoreContext& context) override;
    std::string getName() const override;
};
