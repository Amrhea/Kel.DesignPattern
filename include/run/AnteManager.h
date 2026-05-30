#pragma once
#include <string>
#include <vector>
#include "session/RuntimeSession.h"

class AnteManager {
private:
    RuntimeSession& session;

public:
    AnteManager(RuntimeSession& session);
    ~AnteManager();

    int GetAnte() const;
    std::string GetBlindName() const;
    int GetTargetScore() const;
    int GetRewardMoney() const;
    std::vector<std::string> Advance();
    std::vector<std::string> Skip();
};
