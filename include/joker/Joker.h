#pragma once
#include "joker/Observer.h"
#include "state/ScoreContext.h"
#include <string>

// PERSISTENT
class Joker : public Observer {
public:
    virtual ~Joker() = default;
    virtual void apply(ScoreContext& context) override = 0;
    virtual std::string getName() const = 0;
};
