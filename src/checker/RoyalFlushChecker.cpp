#include <iostream>
#include "../lib/checker/RoyalFlushChecker.h"

bool RoyalFlushChecker::Check(const Hand& hand)
{
    if (IsRoyalFlush(hand))
    {
        std::cout << "Detected ROYAL FLUSH\n";
        return true;
    }
    return false;
}

bool RoyalFlushChecker::IsRoyalFlush(const Hand& hand)
{
    // Royal flush is A, K, Q, J, 10 of the same suit
    // Highest straight flush
    return false;
}
