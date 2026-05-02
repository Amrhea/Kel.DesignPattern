#include <iostream>
#include "../lib/checker/FourOfKindChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand FourOfKindChecker::Check(const Hand& hand)
{
    if (IsFourOfKind(hand))
    {
        std::cout << "Detected FOUR OF A KIND\n";
        return ChosenHand("Four of a Kind", 400);
    }
    return ChosenHand();
}

bool FourOfKindChecker::IsFourOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 4);
}
