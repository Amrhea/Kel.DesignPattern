#include <iostream>
#include "../lib/checker/FlushHouseChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand FlushHouseChecker::Check(const Hand& hand)
{
    if (IsFlushHouse(hand))
    {
        std::cout << "Detected FLUSH HOUSE\n";
        return ChosenHand("Flush House", 140);
    }
    return ChosenHand();
}

bool FlushHouseChecker::IsFlushHouse(const Hand& hand)
{
    // Full House + Flush
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && PokerHandUtils::HasCount(hand, 2)
        && PokerHandUtils::IsFlush(hand);
}
