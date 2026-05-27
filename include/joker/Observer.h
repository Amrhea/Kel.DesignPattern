#pragma once
#include <string>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void OnHandPlayed(const std::string& handName, int& score) = 0;
};
