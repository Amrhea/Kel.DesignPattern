#include <iostream>
#include "poker_evaluation/checker/FlushChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation FlushChecker::Check(const Hand& hand)
{
    if (IsFlush(hand))
    {
        return HandEvaluation("Flush", PokerHandType::Flush);
    }
    return HandEvaluation();
}

bool FlushChecker::IsFlush(const Hand& hand)
{
    return PokerHandUtils::IsFlush(hand);
}

