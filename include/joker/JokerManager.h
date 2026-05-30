#pragma once
#include "joker/Subject.h"
#include "state/ScoreContext.h"
#include "state/RunPersistentState.h"

// RUNTIME / UTILITY
class JokerManager : public Subject {
public:
    static JokerManager& GetInstance() {
        static JokerManager instance;
        return instance;
    }

    static void ApplyJokers(RunPersistentState& persistentState, ScoreContext& context) {
        for (auto& observer : persistentState.jokers) {
            if (observer) {
                observer->apply(context);
            }
        }
    }

private:
    JokerManager() = default;
};
