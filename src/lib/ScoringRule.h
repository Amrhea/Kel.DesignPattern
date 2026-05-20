#pragma once
#include <string>
#include <memory>

class IScoringStrategy {
public:
    virtual ~IScoringStrategy() = default;
    virtual int CalculateScore(const std::string& handName, int baseScore) = 0;
};

class StandardScoring : public IScoringStrategy {
public:
    int CalculateScore(const std::string& handName, int baseScore) override;
};

class DoubleScoring : public IScoringStrategy {
public:
    int CalculateScore(const std::string& handName, int baseScore) override;
};

class ScoringRule {
private:
    std::unique_ptr<IScoringStrategy> strategy;

public:
    ScoringRule(std::unique_ptr<IScoringStrategy> strat);
    int calculateScore(const std::string& handName, int baseScore);
};
