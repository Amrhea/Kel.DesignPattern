#include <iostream>
#include "../lib/checker/ThreeOfKindChecker.h"
#include "../lib/PokerHandUtils.h"

bool ThreeOfKindChecker::Check(const Hand& hand)
{
    if (IsThreeOfKind(hand))
    {
        std::cout << "Detected THREE OF A KIND\n";
        return true;
    }
    return false;
}

bool ThreeOfKindChecker::IsThreeOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && !PokerHandUtils::HasCount(hand, 2);
}
