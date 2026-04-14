#include <iostream>
#include "../lib/checker/StraightChecker.h"

bool StraightChecker::Check(const Hand& hand)
{
    if (IsStright(hand))
    {
        std::cout << "Detected STRAIGHT\n";
        return true;
    }
    return false;
}

bool StraightChecker::IsStright(const Hand& hand)
{
    // Straight is five consecutive cards
    // To be implemented
    return false;
}
