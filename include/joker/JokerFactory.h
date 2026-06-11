#pragma once
#include <memory>
#include "joker/Joker.h"

enum class JokerType {
    CHIPS_BOOST,
    MULT_BOOST,
    FLUSH_MULT,
    JOKER_CARD
};

class JokerFactory {
public:
    static std::shared_ptr<Joker> CreateJoker(JokerType type);
    static std::shared_ptr<Joker> CreateRandomJoker();
};
