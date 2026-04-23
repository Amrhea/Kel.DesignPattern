#include <iostream>
#include "../lib/checker/StraightFlushChecker.h"
#include "../lib/PokerHandUtils.h"

bool StraightFlushChecker::Check(const Hand& hand)
{
    if (IsStraightFlush(hand))
    {
        std::cout << "Detected STRAIGHT FLUSH\n";
        return true;
    }
    return false;
}

bool StraightFlushChecker::IsStraightFlush(const Hand& hand)
{
    return PokerHandUtils::IsStraight(hand)
        && PokerHandUtils::IsFlush(hand)
        && !PokerHandUtils::IsRoyalFlush(hand);
}
