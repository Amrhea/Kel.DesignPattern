#include <iostream>
#include "../lib/checker/FlushChecker.h"

bool FlushChecker::Check(const Hand& hand)
{
    if (IsFlush(hand))
    {
        std::cout << "Detected FLUSH\n";
        return true;
    }
    return false;
}

bool FlushChecker::IsFlush(const Hand& hand)
{
    // To be implemented: check if all 5 cards have the same suit
    return false;
}
