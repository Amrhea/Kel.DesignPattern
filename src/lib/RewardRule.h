#pragma once
#include <memory>

class IRewardStrategy {
public:
    virtual ~IRewardStrategy() = default;
    virtual int CalculateReward(int score) = 0;
};

class StandardReward : public IRewardStrategy {
public:
    int CalculateReward(int score) override;
};

class GenerousReward : public IRewardStrategy {
public:
    int CalculateReward(int score) override;
};

class RewardRule {
private:
    std::unique_ptr<IRewardStrategy> strategy;

public:
    RewardRule(std::unique_ptr<IRewardStrategy> strat);
    int calculateReward(int score);
};
