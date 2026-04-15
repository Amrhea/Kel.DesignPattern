#include <iostream>
#include "../lib/checker/FlushHouseChecker.h"

bool FlushHouseChecker::Check(const Hand& hand)
{
    if (IsFlushHouse(hand))
    {
        std::cout << "Detected FLUSH HOUSE\n";
        return true;
    }
    return false;
}

bool FlushHouseChecker::IsFlushHouse(const Hand& hand)
{
    // Flush house: flush with all cards of the same suit
    // and containing exactly one pair
    return false;
}
