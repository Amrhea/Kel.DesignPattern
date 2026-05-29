#include <iostream>
#include "poker_evaluation/checker/ThreeOfKindChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation ThreeOfKindChecker::Check(const Hand& hand)
{
    if (IsThreeOfKind(hand))
    {
        return HandEvaluation("Three of a Kind", PokerHandType::ThreeOfKind);
    }
    return HandEvaluation();
}

bool ThreeOfKindChecker::IsThreeOfKind(const Hand& hand)
{
    return hand.GetCardCount() >= 3
        && PokerHandUtils::HasCount(hand, 3)
        && !PokerHandUtils::HasCount(hand, 2);
}

