#include <iostream>
#include "poker_evaluation/checker/PairChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation PairChecker::Check(const Hand& hand)
{
    if (IsPair(hand))
    {
        return HandEvaluation("Pair", PokerHandType::Pair);
    }
    return HandEvaluation();
}

bool PairChecker::IsPair(const Hand& hand)
{
    return hand.GetCardCount() >= 2
        && PokerHandUtils::CountRanksWithOccurrences(hand, 2) == 1
        && !PokerHandUtils::HasCount(hand, 3)
        && !PokerHandUtils::HasCount(hand, 4)
        && !PokerHandUtils::HasCount(hand, 5);
}

