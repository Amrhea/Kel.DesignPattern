#include <iostream>
#include "../lib/checker/FullHouseChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand FullHouseChecker::Check(const Hand& hand)
{
    if (IsFullHouse(hand))
    {
        std::cout << "Detected FULL HOUSE\n";
        return ChosenHand("Full House", 300);
    }
    return ChosenHand();
}

bool FullHouseChecker::IsFullHouse(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && PokerHandUtils::HasCount(hand, 2)
        && !PokerHandUtils::IsFlush(hand);
}
