#include "session/RuntimeSession.h"
#include "blind/BlindState.h"
#include "blind/SmallBlindState.h"
#include "reward/RewardCommand.h"

RuntimeSession::RuntimeSession() 
    : ante(1), remainingPlays(4), currentBlind(std::make_shared<SmallBlindState>()) {}

RuntimeSession::~RuntimeSession() {}

std::vector<std::string> RuntimeSession::executePendingCommands(const std::string& timing) {
    std::vector<std::string> logs;
    std::vector<std::shared_ptr<RewardCommand>> remaining;
    for (auto& cmd : pendingCommands) {
        if (cmd && cmd->getTiming() == timing) {
            cmd->execute(*this);
            std::string logMsg = "→ " + cmd->getName() + " executed.";
            if (cmd->getName() == "Bonus Hand") {
                logMsg += " remainingPlays: " + std::to_string(remainingPlays);
            } else if (cmd->getName() == "Free Playing Card") {
                logMsg += " deck size: " + std::to_string(deck.size());
            }
            logs.push_back(logMsg);
        } else {
            remaining.push_back(cmd);
        }
    }
    pendingCommands = std::move(remaining);
    return logs;
}

std::vector<std::string> RuntimeSession::skipBlind() {
    std::vector<std::string> logs;
    if (!currentBlind) return logs;
    auto cmd = currentBlind->createSkipRewardCommand();
    if (cmd) {
        pendingCommands.push_back(cmd);
        logs.push_back("[Reward queued: " + cmd->getName() + " (timing: " + cmd->getTiming() + ")]");
    }
    int oldAnte = ante;
    logs.push_back("Advancing to next blind..."); // General advancement message
    currentBlind = currentBlind->nextState(ante);
    
    logs.push_back("[Executing NextBlind commands...]");
    auto nextBlindLogs = executePendingCommands("NextBlind");
    logs.insert(logs.end(), nextBlindLogs.begin(), nextBlindLogs.end());

    if (ante > oldAnte) {
        logs.push_back("[Executing NextAnte commands...]");
        auto nextAnteLogs = executePendingCommands("NextAnte");
        logs.insert(logs.end(), nextAnteLogs.begin(), nextAnteLogs.end());
    }
    return logs;
}

std::vector<std::string> RuntimeSession::playBlind() {
    std::vector<std::string> logs;
    if (!currentBlind) return logs;
    int oldAnte = ante;
    logs.push_back("Advancing to next blind..."); // General advancement message
    currentBlind = currentBlind->nextState(ante);

    logs.push_back("[Executing NextBlind commands...]");
    auto nextBlindLogs = executePendingCommands("NextBlind");
    logs.insert(logs.end(), nextBlindLogs.begin(), nextBlindLogs.end());

    if (ante > oldAnte) {
        logs.push_back("[Executing NextAnte commands...]");
        auto nextAnteLogs = executePendingCommands("NextAnte");
        logs.insert(logs.end(), nextAnteLogs.begin(), nextAnteLogs.end());
    }
    return logs;
}
