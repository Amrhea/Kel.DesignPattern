#include <iostream>
#include "../lib/checker/HighCardChecker.h"

bool HighCardChecker::Check(const Hand& hand)
{
    if (IsHighCard(hand))
    {
        std::cout << "Detected HIGH CARD\n";
        return true;
    }
    return false;
}

bool HighCardChecker::IsHighCard(const Hand& hand)
{
    // High card is the default case when no other hand is formed
    // This checker should be at the end of the chain
    return true;
}
