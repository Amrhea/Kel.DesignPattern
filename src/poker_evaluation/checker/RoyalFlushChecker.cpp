#include <iostream>
#include "poker_evaluation/checker/RoyalFlushChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation RoyalFlushChecker::Check(const Hand& hand)
{
    if (IsRoyalFlush(hand))
    {
        return HandEvaluation("Royal Flush", PokerHandType::RoyalFlush);
    }
    return HandEvaluation();
}

bool RoyalFlushChecker::IsRoyalFlush(const Hand& hand)
{
    return PokerHandUtils::IsRoyalFlush(hand);
}

