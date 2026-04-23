#include <iostream>
#include "../lib/checker/RoyalFlushChecker.h"
#include "../lib/PokerHandUtils.h"

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
    return PokerHandUtils::IsRoyalFlush(hand);
}
