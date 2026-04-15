#include <iostream>
#include "../lib/checker/PairChecker.h"

bool PairChecker::Check(const Hand& hand)
{
    if (IsPair(hand))
    {
        std::cout << "Detected PAIR\n";
        return true;
    }
    return false;
}

bool PairChecker::IsPair(const Hand& hand)
{
    // Pair is two cards of the same rank
    return false;
}
