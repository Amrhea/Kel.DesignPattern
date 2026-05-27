#include <iostream>
#include "poker_evaluation/checker/StraightFlushChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

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

