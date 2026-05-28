#include "session/RuntimeSession.h"
#include "blind/BlindState.h"
#include "blind/SmallBlindState.h"
#include "reward/RewardCommand.h"
#include <iostream>

RuntimeSession::RuntimeSession() 
    : ante(1), remainingPlays(4), currentBlind(std::make_shared<SmallBlindState>()) {}

RuntimeSession::~RuntimeSession() {}

void RuntimeSession::executePendingCommands(const std::string& timing) {
    std::vector<std::shared_ptr<RewardCommand>> remaining;
    for (auto& cmd : pendingCommands) {
        if (cmd && cmd->getTiming() == timing) {
            std::cout << "→ " << cmd->getName() << " executed.";
            cmd->execute(*this);
            if (cmd->getName() == "Bonus Hand") {
                std::cout << " remainingPlays: " << remainingPlays << std::endl;
            } else if (cmd->getName() == "Free Playing Card") {
                std::cout << " deck size: " << deck.size() << std::endl;
            } else {
                std::cout << std::endl;
            }
        } else {
            remaining.push_back(cmd);
        }
    }
    pendingCommands = std::move(remaining);
}

void RuntimeSession::skipBlind() {
    if (!currentBlind) return;
    auto cmd = currentBlind->createSkipRewardCommand();
    if (cmd) {
        pendingCommands.push_back(cmd);
        std::cout << "[Reward queued: " << cmd->getName() << " (timing: " << cmd->getTiming() << ")]" << std::endl;
    }
    int oldAnte = ante;
    std::cout << "Advancing to ";
    currentBlind = currentBlind->nextState(ante);
    std::cout << currentBlind->getName() << "..." << std::endl;

    std::cout << "[Executing NextBlind commands...]" << std::endl;
    executePendingCommands("NextBlind");

    if (ante > oldAnte) {
        std::cout << "[Executing NextAnte commands...]" << std::endl;
        executePendingCommands("NextAnte");
    }
}

void RuntimeSession::playBlind() {
    if (!currentBlind) return;
    int oldAnte = ante;
    std::cout << "Advancing to ";
    currentBlind = currentBlind->nextState(ante);
    std::cout << currentBlind->getName() << "..." << std::endl;

    std::cout << "[Executing NextBlind commands...]" << std::endl;
    executePendingCommands("NextBlind");

    if (ante > oldAnte) {
        std::cout << "[Executing NextAnte commands...]" << std::endl;
        executePendingCommands("NextAnte");
    }
}
