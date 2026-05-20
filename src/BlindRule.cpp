#include "lib/BlindRule.h"

int SmallBlind::GetRequiredScore(int level) {
    return level * 100;
}

int BigBlind::GetRequiredScore(int level) {
    return level * 150;
}

int BossBlind::GetRequiredScore(int level) {
    return level * 300;
}

BlindRule::BlindRule(std::unique_ptr<IBlindStrategy> strat) : strategy(std::move(strat)) {}

int BlindRule::getRequiredScore(int level) {
    return strategy ? strategy->GetRequiredScore(level) : level * 100;
}
