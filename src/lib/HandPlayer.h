#pragma once
#include <vector>
#include <memory>
#include "JokerCard.h"

class HandPlayer {
private:
    int gold;
    std::vector<std::shared_ptr<JokerCard>> jokers;

public:
    HandPlayer();
    ~HandPlayer();

    void AddGold(int amount);
    int GetGold() const;

    void AddJoker(std::shared_ptr<JokerCard> joker);
    const std::vector<std::shared_ptr<JokerCard>>& GetJokers() const;
    void ClearJokers();
};
