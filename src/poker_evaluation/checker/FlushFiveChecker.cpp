#include <iostream>
#include "poker_evaluation/checker/FlushFiveChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation FlushFiveChecker::Check(const Hand& hand)
{
    if (IsFlushFive(hand))
    {
        return HandEvaluation("Flush Five", PokerHandType::FlushFive);
    }
    return HandEvaluation();
}

bool FlushFiveChecker::IsFlushFive(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 5)
        && PokerHandUtils::IsFlush(hand);
}
