#include "joker/JokerFactory.h"
#include "joker/ChipsBoostJoker.h"
#include "joker/MultBoostJoker.h"
#include "joker/FlushMultJoker.h"
#include "joker/JokerCard.h"
#include <cstdlib>

std::shared_ptr<Joker> JokerFactory::CreateJoker(JokerType type) {
    switch (type) {
        case JokerType::CHIPS_BOOST:
            return std::make_shared<ChipsBoostJoker>(20);
        case JokerType::MULT_BOOST:
            return std::make_shared<MultBoostJoker>(4);
        case JokerType::FLUSH_MULT:
            return std::make_shared<FlushMultJoker>(PokerHandType::Flush, 15);
        case JokerType::JOKER_CARD:
            return std::make_shared<JokerCard>("Lucky Joker", 25);
        default:
            return nullptr;
    }
}

std::shared_ptr<Joker> JokerFactory::CreateRandomJoker() {
    int r = std::rand() % 4;
    return CreateJoker(static_cast<JokerType>(r));
}
