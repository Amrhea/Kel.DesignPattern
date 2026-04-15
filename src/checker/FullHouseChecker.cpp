#include <iostream>
#include "../lib/checker/FullHouseChecker.h"

bool FullHouseChecker::Check(const Hand& hand)
{
    if (IsFullHouse(hand))
    {
        std::cout << "Detected FULL HOUSE\n";
        return true;
    }
    return false;
}

bool FullHouseChecker::IsFullHouse(const Hand& hand)
{
    // Full house is three of a kind + a pair
    return false;
}
