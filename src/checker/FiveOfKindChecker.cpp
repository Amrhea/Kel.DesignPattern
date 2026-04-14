#include <iostream>
#include "../lib/checker/FiveOfKindChecker.h"

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
    // Five of a kind is only possible with wildcards/jokers
    // Four of a kind plus a wildcard
    return false;
}
