#pragma once
#include "blind/BlindState.h"

class SmallBlindState : public BlindState {
public:
    std::string getName() const override;
    int getTargetScore(int ante) const override;
    int getRewardMoney() const override;
    std::shared_ptr<RewardCommand> createSkipRewardCommand() const override;
    std::shared_ptr<BlindState> nextState(int& ante) const override;
};
