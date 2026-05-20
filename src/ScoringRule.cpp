#include "lib/ScoringRule.h"

int StandardScoring::CalculateScore(const std::string& handName, int baseScore) {
    return baseScore;
}

int DoubleScoring::CalculateScore(const std::string& handName, int baseScore) {
    return baseScore * 2;
}

ScoringRule::ScoringRule(std::unique_ptr<IScoringStrategy> strat) : strategy(std::move(strat)) {}

int ScoringRule::calculateScore(const std::string& handName, int baseScore) {
    return strategy ? strategy->CalculateScore(handName, baseScore) : baseScore;
}
