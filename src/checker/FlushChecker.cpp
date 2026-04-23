#include <iostream>
#include "../lib/checker/FlushChecker.h"
#include "../lib/PokerHandUtils.h"

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
    return PokerHandUtils::IsFlush(hand) && !PokerHandUtils::IsStraight(hand);
}
