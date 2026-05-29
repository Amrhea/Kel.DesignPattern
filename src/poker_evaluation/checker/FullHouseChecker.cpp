#include <iostream>
#include "poker_evaluation/checker/FullHouseChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation FullHouseChecker::Check(const Hand& hand)
{
    if (IsFullHouse(hand))
    {
        return HandEvaluation("Full House", PokerHandType::FullHouse);
    }
    return HandEvaluation();
}

bool FullHouseChecker::IsFullHouse(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && PokerHandUtils::HasCount(hand, 2);
}

