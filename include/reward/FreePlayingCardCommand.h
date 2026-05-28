#pragma once
#include "reward/RewardCommand.h"

class FreePlayingCardCommand : public RewardCommand {
public:
    std::string getName() const override;
    std::string getTiming() const override;
    void execute(RuntimeSession& session) override;
};
