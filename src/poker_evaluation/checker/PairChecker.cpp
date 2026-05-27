#include <iostream>
#include "poker_evaluation/checker/PairChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

ChosenHand PairChecker::Check(const Hand& hand)
{
    if (IsPair(hand))
    {
        std::cout << "Detected PAIR\n";
        return ChosenHand("Pair", PokerHandType::Pair, 20);
    }
    return ChosenHand();
}

bool PairChecker::IsPair(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::CountRanksWithOccurrences(hand, 2) == 1
        && !PokerHandUtils::HasCount(hand, 3);
}

