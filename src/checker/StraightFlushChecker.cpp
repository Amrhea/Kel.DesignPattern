#include <iostream>
#include "../lib/checker/StraightFlushChecker.h"
#include "../lib/PokerHandUtils.h"
#include "../lib/PokerHandType.h"

ChosenHand StraightFlushChecker::Check(const Hand& hand)
{
    if (IsStraightFlush(hand))
    {
        std::cout << "Detected STRAIGHT FLUSH\n";
        return ChosenHand("Straight Flush", PokerHandType::StraightFlush, 250);
    }
    return ChosenHand();
}

bool StraightFlushChecker::IsStraightFlush(const Hand& hand)
{
    return PokerHandUtils::IsStraight(hand) && PokerHandUtils::IsFlush(hand);
}
