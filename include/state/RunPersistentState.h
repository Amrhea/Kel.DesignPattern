#pragma once
#include <vector>
#include <memory>
#include "joker/Observer.h"
#include "blind/BlindState.h"
#include "reward/RewardCommand.h"
#include "tag/Tag.h"

// PERSISTENT
struct RunPersistentState {
    int ante;
    int money;
    std::vector<std::shared_ptr<Observer>> jokers;
    std::vector<std::shared_ptr<RewardCommand>> pendingCommands;
    std::shared_ptr<BlindState> currentBlind;
    std::vector<std::shared_ptr<Tag>> tagStack;

    RunPersistentState()
        : ante(1), money(4), currentBlind(nullptr) {}
};
