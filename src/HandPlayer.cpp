#include "lib/HandPlayer.h"

HandPlayer::HandPlayer() : gold(0) {}

HandPlayer::~HandPlayer() {}

void HandPlayer::AddGold(int amount) {
    gold += amount;
}

int HandPlayer::GetGold() const {
    return gold;
}

void HandPlayer::AddJoker(std::shared_ptr<JokerCard> joker) {
    jokers.push_back(joker);
}

const std::vector<std::shared_ptr<JokerCard>>& HandPlayer::GetJokers() const {
    return jokers;
}

void HandPlayer::ClearJokers() {
    jokers.clear();
}
