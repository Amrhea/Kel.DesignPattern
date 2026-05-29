#include <iostream>
#include "poker_evaluation/checker/FourOfKindChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation FourOfKindChecker::Check(const Hand& hand)
{
    if (IsFourOfKind(hand))
    {
        return HandEvaluation("Four of a Kind", PokerHandType::FourOfKind);
    }
    return HandEvaluation();
}

bool FourOfKindChecker::IsFourOfKind(const Hand& hand)
{
    return hand.GetCardCount() >= 4
        && PokerHandUtils::HasCount(hand, 4);
}

