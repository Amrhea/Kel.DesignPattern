#pragma once
#include <string>

struct ScoreContext;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void apply(ScoreContext& context) = 0;
};
