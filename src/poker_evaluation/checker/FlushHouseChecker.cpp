#include <iostream>
#include "poker_evaluation/checker/FlushHouseChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation FlushHouseChecker::Check(const Hand& hand)
{
    if (IsFlushHouse(hand))
    {
        return HandEvaluation("Flush House", PokerHandType::FlushHouse);
    }
    return HandEvaluation();
}

bool FlushHouseChecker::IsFlushHouse(const Hand& hand)
{
    // Full House + Flush
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && PokerHandUtils::HasCount(hand, 2)
        && PokerHandUtils::IsFlush(hand);
}

