#pragma once
#include <vector>
#include <string>
#include <memory>

class BlindState;
class RewardCommand;

class RuntimeSession {
public:
    int ante;
    int remainingPlays;
    std::vector<std::string> deck;
    std::shared_ptr<BlindState> currentBlind;
    std::vector<std::shared_ptr<RewardCommand>> pendingCommands;

    RuntimeSession();
    ~RuntimeSession();

    std::vector<std::string> executePendingCommands(const std::string& timing);
    std::vector<std::string> skipBlind();
    std::vector<std::string> playBlind();
};
