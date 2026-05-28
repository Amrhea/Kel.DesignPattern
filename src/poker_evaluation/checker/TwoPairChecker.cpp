#include <iostream>
#include "poker_evaluation/checker/TwoPairChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation TwoPairChecker::Check(const Hand& hand)
{
    if (IsTwoPair(hand))
    {
        return HandEvaluation("Two Pair", PokerHandType::TwoPair);
    }
    return HandEvaluation();
}

bool TwoPairChecker::IsTwoPair(const Hand& hand)
{
    return hand.GetCardCount() >= 4
        && PokerHandUtils::CountRanksWithOccurrences(hand, 2) == 2;
}

