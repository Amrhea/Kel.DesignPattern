#include <iostream>
#include "../lib/checker/TwoPairChecker.h"

bool TwoPairChecker::Check(const Hand& hand)
{
    if (IsTwoPair(hand))
    {
        std::cout << "Detected TWO PAIR\n";
        return true;
    }
    return false;
}

bool TwoPairChecker::IsTwoPair(const Hand& hand)
{
    // Two pair is two different pairs
    return false;
}
