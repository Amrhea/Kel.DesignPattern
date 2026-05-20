#pragma once
#include "Observer.h"
#include <string>

class JokerCard : public Observer {
private:
    std::string name;
    int bonusPoints;

public:
    JokerCard(const std::string& name, int bonusPoints);
    void OnHandPlayed(const std::string& handName, int& score) override;
    std::string GetName() const;
};
