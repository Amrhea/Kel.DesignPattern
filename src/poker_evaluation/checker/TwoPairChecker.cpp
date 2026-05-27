#include <iostream>
#include "poker_evaluation/checker/TwoPairChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

ChosenHand TwoPairChecker::Check(const Hand& hand)
{
    if (IsTwoPair(hand))
    {
        std::cout << "Detected TWO PAIR\n";
        return ChosenHand("Two Pair", PokerHandType::TwoPair, 30);
    }
    return ChosenHand();
}

bool TwoPairChecker::IsTwoPair(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::CountRanksWithOccurrences(hand, 2) == 2;
}

