#include "reward/RewardRule.h"

int StandardReward::CalculateReward(int score) {
    return score * 2;
}

int GenerousReward::CalculateReward(int score) {
    return score * 3;
}

RewardRule::RewardRule(std::unique_ptr<IRewardStrategy> strat) : strategy(std::move(strat)) {}

int RewardRule::calculateReward(int score) {
    return strategy ? strategy->CalculateReward(score) : score * 2;
}
