#pragma once
#include "HandScore.h"
#include <memory>

class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual PlayedHandResult Calculate(PokerHandType type, const HandScoreTable& table) = 0;
};

class BaseScoringRule : public IScoringRule {
public:
    PlayedHandResult Calculate(PokerHandType type, const HandScoreTable& table) override;
};

class ScoringRule {
private:
    std::unique_ptr<IScoringRule> strategy;

public:
    ScoringRule(std::unique_ptr<IScoringRule> strat);
    PlayedHandResult calculateScore(PokerHandType type, const HandScoreTable& table);
};
