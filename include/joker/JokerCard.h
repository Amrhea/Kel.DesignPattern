#pragma once
#include "joker/Joker.h"
#include <string>

class JokerCard : public Joker {
private:
    std::string name;
    int bonusPoints;

public:
    JokerCard(const std::string& name, int bonusPoints);
    void apply(ScoreContext& context) override;
    std::string getName() const override;
};
