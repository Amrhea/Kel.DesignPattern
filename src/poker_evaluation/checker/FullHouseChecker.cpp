#include <iostream>
#include "poker_evaluation/checker/FullHouseChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

ChosenHand FullHouseChecker::Check(const Hand& hand)
{
    if (IsFullHouse(hand))
    {
        std::cout << "Detected FULL HOUSE\n";
        return ChosenHand("Full House", PokerHandType::FullHouse, 100);
    }
    return ChosenHand();
}

bool FullHouseChecker::IsFullHouse(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && PokerHandUtils::HasCount(hand, 2);
}

