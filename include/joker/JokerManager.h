#pragma once
#include "joker/Subject.h"

class JokerManager : public Subject {
public:
    static JokerManager& GetInstance() {
        static JokerManager instance;
        return instance;
    }

private:
    JokerManager() = default;
};
