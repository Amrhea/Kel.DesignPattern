#include <iostream>
#include "poker_evaluation/checker/StraightFlushChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation StraightFlushChecker::Check(const Hand& hand)
{
    if (IsStraightFlush(hand))
    {
        return HandEvaluation("Straight Flush", PokerHandType::StraightFlush);
    }
    return HandEvaluation();
}

bool StraightFlushChecker::IsStraightFlush(const Hand& hand)
{
    return PokerHandUtils::IsStraight(hand) && PokerHandUtils::IsFlush(hand);
}

