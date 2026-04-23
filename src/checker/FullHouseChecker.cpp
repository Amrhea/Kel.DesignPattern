#include <iostream>
#include "../lib/checker/FullHouseChecker.h"
#include "../lib/PokerHandUtils.h"

bool FullHouseChecker::Check(const Hand& hand)
{
    if (IsFullHouse(hand))
    {
        std::cout << "Detected FULL HOUSE\n";
        return true;
    }
    return false;
}

bool FullHouseChecker::IsFullHouse(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && PokerHandUtils::HasCount(hand, 2)
        && !PokerHandUtils::IsFlush(hand);
}
