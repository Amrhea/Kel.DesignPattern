#include <iostream>
#include "poker_evaluation/checker/HighCardChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

ChosenHand HighCardChecker::Check(const Hand& hand)
{
    if (IsHighCard(hand))
    {
        std::cout << "Detected HIGH CARD\n";
        return ChosenHand("High Card", PokerHandType::HighCard, 10);
    }
    return ChosenHand();
}

bool HighCardChecker::IsHighCard(const Hand& hand)
{
    // If it reached here, it didn't match any stronger hand
    return hand.GetCardCount() == 5;
}

