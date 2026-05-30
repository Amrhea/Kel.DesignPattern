#pragma once
#include "joker/Observer.h"
#include <string>

class JokerCard : public Observer {
private:
    std::string name;
    int bonusPoints;

public:
    JokerCard(const std::string& name, int bonusPoints);
    void apply(ScoreContext& context) override;
    std::string GetName() const;
};
