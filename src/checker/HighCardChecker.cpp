#include <iostream>
#include "../lib/checker/HighCardChecker.h"
#include "../lib/PokerHandUtils.h"

bool HighCardChecker::Check(const Hand& hand)
{
    if (IsHighCard(hand))
    {
        std::cout << "Detected HIGH CARD\n";
        return true;
    }
    return false;
}

bool HighCardChecker::IsHighCard(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && !PokerHandUtils::HasCount(hand, 2)
        && !PokerHandUtils::HasCount(hand, 3)
        && !PokerHandUtils::HasCount(hand, 4)
        && !PokerHandUtils::HasCount(hand, 5)
        && !PokerHandUtils::IsStraight(hand)
        && !PokerHandUtils::IsFlush(hand);
}
