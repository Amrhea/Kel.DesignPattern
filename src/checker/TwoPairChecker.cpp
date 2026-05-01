#include <iostream>
#include "../lib/checker/TwoPairChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand TwoPairChecker::Check(const Hand& hand)
{
    if (IsTwoPair(hand))
    {
        std::cout << "Detected TWO PAIR\n";
        return ChosenHand("Two Pair", 50);
    }
    return ChosenHand();
}

bool TwoPairChecker::IsTwoPair(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::CountRanksWithOccurrences(hand, 2) == 2;
}
