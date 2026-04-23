#include <iostream>
#include "../lib/checker/StraightChecker.h"
#include "../lib/PokerHandUtils.h"

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
    return PokerHandUtils::IsStraight(hand) && !PokerHandUtils::IsFlush(hand);
}
