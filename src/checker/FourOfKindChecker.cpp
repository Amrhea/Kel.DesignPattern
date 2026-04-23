#include <iostream>
#include "../lib/checker/FourOfKindChecker.h"
#include "../lib/PokerHandUtils.h"

bool FourOfKindChecker::Check(const Hand& hand)
{
    if (IsFourOfKind(hand))
    {
        std::cout << "Detected FOUR OF A KIND\n";
        return true;
    }
    return false;
}

bool FourOfKindChecker::IsFourOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 4);
}
