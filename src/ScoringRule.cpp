#include "lib/ScoringRule.h"

PlayedHandResult BaseScoringRule::Calculate(PokerHandType type, const HandScoreTable& table) {
    auto it = table.find(type);
    if (it != table.end()) {
        const auto& data = it->second;
        return PlayedHandResult(type, data.baseChips, data.baseMult, data.level);
    }
    return PlayedHandResult(type, 0, 0, 0);
}

ScoringRule::ScoringRule(std::unique_ptr<IScoringRule> strat) : strategy(std::move(strat)) {}

PlayedHandResult ScoringRule::calculateScore(PokerHandType type, const HandScoreTable& table) {
    if (strategy) {
        return strategy->Calculate(type, table);
    }
    return PlayedHandResult(type, 0, 0, 0);
}
