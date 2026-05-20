#pragma once
#include <memory>

class IBlindStrategy {
public:
    virtual ~IBlindStrategy() = default;
    virtual int GetRequiredScore(int level) = 0;
};

class SmallBlind : public IBlindStrategy {
public:
    int GetRequiredScore(int level) override;
};

class BigBlind : public IBlindStrategy {
public:
    int GetRequiredScore(int level) override;
};

class BossBlind : public IBlindStrategy {
public:
    int GetRequiredScore(int level) override;
};

class BlindRule {
private:
    std::unique_ptr<IBlindStrategy> strategy;

public:
    BlindRule(std::unique_ptr<IBlindStrategy> strat);
    int getRequiredScore(int level);
};
