#pragma once
#include <vector>
#include <memory>
#include "reward/RewardCommand.h"

class RewardManager {
public:
    static std::vector<std::shared_ptr<RewardCommand>> GeneratePostBlindRewards();
};
