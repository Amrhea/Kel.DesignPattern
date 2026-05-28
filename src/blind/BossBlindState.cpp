#include "blind/BossBlindState.h"
#include "blind/SmallBlindState.h"
#include "reward/FreePlayingCardCommand.h"

std::string BossBlindState::getName() const {
    return "Boss Blind";
}

int BossBlindState::getTargetScore(int ante) const {
    return 600 * ante;
}

int BossBlindState::getRewardMoney() const {
    return 5;
}

std::shared_ptr<RewardCommand> BossBlindState::createSkipRewardCommand() const {
    return std::make_shared<FreePlayingCardCommand>();
}

std::shared_ptr<BlindState> BossBlindState::nextState(int& ante) const {
    ante += 1;
    return std::make_shared<SmallBlindState>();
}
