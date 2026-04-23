#include <iostream>
#include "../lib/checker/FiveOfKindChecker.h"
#include "../lib/PokerHandUtils.h"

bool FiveOfKindChecker::Check(const Hand& hand)
{
    if (IsFiveOfKind(hand))
    {
        std::cout << "Detected FIVE OF A KIND\n";
        return true;
    }
    return false;
}

bool FiveOfKindChecker::IsFiveOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 5);
}
