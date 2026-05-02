#include <iostream>
#include "../lib/checker/StraightFlushChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand StraightFlushChecker::Check(const Hand& hand)
{
    if (IsStraightFlush(hand))
    {
        std::cout << "Detected STRAIGHT FLUSH\n";
        return ChosenHand("Straight Flush", 250);
    }
    return ChosenHand();
}

bool StraightFlushChecker::IsStraightFlush(const Hand& hand)
{
    return PokerHandUtils::IsStraight(hand) && PokerHandUtils::IsFlush(hand);
}
