#include "blind/SmallBlindState.h"
#include "blind/BigBlindState.h"
#include "reward/BonusHandCommand.h"

std::string SmallBlindState::getName() const {
    return "Small Blind";
}

int SmallBlindState::getTargetScore(int ante) const {
    return 300 * ante;
}

int SmallBlindState::getRewardMoney() const {
    return 3;
}

std::shared_ptr<RewardCommand> SmallBlindState::createSkipRewardCommand() const {
    return std::make_shared<BonusHandCommand>();
}

std::shared_ptr<BlindState> SmallBlindState::nextState(int& ante) const {
    return std::make_shared<BigBlindState>();
}
