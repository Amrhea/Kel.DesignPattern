#include <iostream>
#include "../lib/checker/FourOfKindChecker.h"

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
    // Four of a kind is four cards of the same rank
    return false;
}
