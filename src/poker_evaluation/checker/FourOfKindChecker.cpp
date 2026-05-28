#include <iostream>
#include "poker_evaluation/checker/FourOfKindChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

ChosenHand FourOfKindChecker::Check(const Hand& hand)
{
    if (IsFourOfKind(hand))
    {
        std::cout << "Detected FOUR OF A KIND\n";
        return ChosenHand("Four of a Kind", PokerHandType::FourOfKind, 150);
    }
    return ChosenHand();
}

bool FourOfKindChecker::IsFourOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 4);
}

