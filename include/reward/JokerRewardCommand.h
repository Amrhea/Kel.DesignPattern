#pragma once
#include "reward/RewardCommand.h"
#include "joker/Joker.h"
#include "session/RuntimeSession.h"
#include <memory>

class JokerRewardCommand : public RewardCommand {
private:
    std::shared_ptr<Joker> joker;

public:
    JokerRewardCommand(std::shared_ptr<Joker> j) : joker(j) {}

    std::string getName() const override { return "Joker Reward: " + (joker ? joker->getName() : "None"); }
    std::string getTiming() const override { return "Immediate"; }
    void execute(RuntimeSession& session) override {
        if (joker) {
            session.addJoker(joker);
        }
    }
};
