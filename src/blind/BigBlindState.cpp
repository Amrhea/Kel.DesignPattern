#include "blind/BigBlindState.h"
#include "blind/BossBlindState.h"
#include "reward/FreePlayingCardCommand.h"

std::string BigBlindState::getName() const {
    return "Big Blind";
}

int BigBlindState::getTargetScore(int ante) const {
    return 450 * ante;
}

int BigBlindState::getRewardMoney() const {
    return 4;
}

std::shared_ptr<RewardCommand> BigBlindState::createSkipRewardCommand() const {
    return std::make_shared<FreePlayingCardCommand>();
}

std::shared_ptr<BlindState> BigBlindState::nextState(int& ante) const {
    return std::make_shared<BossBlindState>();
}
