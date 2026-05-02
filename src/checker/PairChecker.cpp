#include <iostream>
#include "../lib/checker/PairChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand PairChecker::Check(const Hand& hand)
{
    if (IsPair(hand))
    {
        std::cout << "Detected PAIR\n";
        return ChosenHand("Pair", 20);
    }
    return ChosenHand();
}

bool PairChecker::IsPair(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::CountRanksWithOccurrences(hand, 2) == 1
        && !PokerHandUtils::HasCount(hand, 3);
}
