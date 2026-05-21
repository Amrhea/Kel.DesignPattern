#include <iostream>
#include "../lib/checker/FourOfKindChecker.h"
#include "../lib/PokerHandUtils.h"
#include "../lib/PokerHandType.h"

ChosenHand FourOfKindChecker::Check(const Hand& hand)
{
    if (IsFourOfKind(hand))
    {
        std::cout << "Detected FOUR OF A KIND\n";
        return ChosenHand("Four of a Kind", PokerHandType::FourOfKind, 150);
    }
    return ChosenHand();
}

bool FourOfKindChecker::IsFourOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 4);
}
