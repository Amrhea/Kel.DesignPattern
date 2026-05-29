#include <iostream>
#include "poker_evaluation/checker/StraightChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation StraightChecker::Check(const Hand& hand)
{
    if (IsStraight(hand))
    {
        return HandEvaluation("Straight", PokerHandType::Straight);
    }
    return HandEvaluation();
}

bool StraightChecker::IsStraight(const Hand& hand)
{
    return PokerHandUtils::IsStraight(hand);
}

