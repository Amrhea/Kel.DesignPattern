#include <iostream>
#include "../lib/checker/ThreeOfKindChecker.h"

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
    // Three of a kind is three cards of the same rank
    return false;
}
