#pragma once
#include <string>
#include <memory>

class RewardCommand;

class BlindState {
public:
    virtual ~BlindState() = default;
    virtual std::string getName() const = 0;
    virtual int getTargetScore(int ante) const = 0;
    virtual int getRewardMoney() const = 0;
    virtual std::shared_ptr<RewardCommand> createSkipRewardCommand() const = 0;
    virtual std::shared_ptr<BlindState> nextState(int& ante) const = 0;
};
