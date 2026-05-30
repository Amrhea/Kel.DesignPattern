#pragma once
#include "session/RuntimeSession.h"
#include "blind/BlindManager.h"
#include "scoring/ScoringRule.h"
#include <memory>

class RoundManager {
private:
    std::unique_ptr<ScoringRule> scoringRule;

public:
    RoundManager();
    ~RoundManager();

    // Runs a round, returning true if won, false if lost.
    // Handles all input and output for the round.
    bool RunRound(RuntimeSession& session, const BlindData& blindData);
};
