#include <iostream>
#include "../lib/checker/FiveOfKindChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand FiveOfKindChecker::Check(const Hand& hand)
{
    if (IsFiveOfKind(hand))
    {
        std::cout << "Detected FIVE OF A KIND\n";
        return ChosenHand("Five of a Kind", 1000);
    }
    return ChosenHand();
}

bool FiveOfKindChecker::IsFiveOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 5);
}
