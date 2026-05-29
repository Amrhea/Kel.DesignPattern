#include <iostream>
#include "poker_evaluation/checker/FiveOfKindChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation FiveOfKindChecker::Check(const Hand& hand)
{
    if (IsFiveOfKind(hand))
    {
        return HandEvaluation("Five of a Kind", PokerHandType::FiveOfKind);
    }
    return HandEvaluation();
}

bool FiveOfKindChecker::IsFiveOfKind(const Hand& hand)
{
    return hand.GetCardCount() >= 5
        && PokerHandUtils::HasCount(hand, 5);
}

