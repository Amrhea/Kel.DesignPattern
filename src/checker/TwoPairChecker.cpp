#include <iostream>
#include "../lib/checker/TwoPairChecker.h"
#include "../lib/PokerHandUtils.h"

bool TwoPairChecker::Check(const Hand& hand)
{
    if (IsTwoPair(hand))
    {
        std::cout << "Detected TWO PAIR\n";
        return true;
    }
    return false;
}

bool TwoPairChecker::IsTwoPair(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::CountRanksWithOccurrences(hand, 2) == 2;
}
