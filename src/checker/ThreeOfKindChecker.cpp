#include <iostream>
#include "../lib/checker/ThreeOfKindChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand ThreeOfKindChecker::Check(const Hand& hand)
{
    if (IsThreeOfKind(hand))
    {
        std::cout << "Detected THREE OF A KIND\n";
        return ChosenHand("Three of a Kind", 100);
    }
    return ChosenHand();
}

bool ThreeOfKindChecker::IsThreeOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && !PokerHandUtils::HasCount(hand, 2);
}
