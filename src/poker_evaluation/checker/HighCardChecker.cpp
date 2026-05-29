#include <iostream>
#include "poker_evaluation/checker/HighCardChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

HandEvaluation HighCardChecker::Check(const Hand& hand)
{
    if (IsHighCard(hand))
    {
        return HandEvaluation("High Card", PokerHandType::HighCard);
    }
    return HandEvaluation();
}

bool HighCardChecker::IsHighCard(const Hand& hand)
{
    // If it reached here, it didn't match any stronger hand
    return hand.GetCardCount() >= 1;
}

