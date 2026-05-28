#pragma once
#include <string>

class RuntimeSession;

class RewardCommand {
public:
    virtual ~RewardCommand() = default;
    virtual std::string getName() const = 0;
    virtual std::string getTiming() const = 0;
    virtual void execute(RuntimeSession& session) = 0;
};
