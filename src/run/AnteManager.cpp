#include "run/AnteManager.h"
#include "blind/BlindState.h"

AnteManager::AnteManager(RuntimeSession& s) : session(s) {}

AnteManager::~AnteManager() {}

int AnteManager::GetAnte() const {
    return session.ante;
}

std::string AnteManager::GetBlindName() const {
    return session.currentBlind ? session.currentBlind->getName() : "";
}

int AnteManager::GetTargetScore() const {
    return session.currentBlind ? session.currentBlind->getTargetScore(session.ante) : 0;
}

int AnteManager::GetRewardMoney() const {
    return session.currentBlind ? session.currentBlind->getRewardMoney() : 0;
}

std::vector<std::string> AnteManager::Advance() {
    return session.playBlind();
}

std::vector<std::string> AnteManager::Skip() {
    return session.skipBlind();
}
