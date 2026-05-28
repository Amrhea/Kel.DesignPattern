#include <iostream>
#include "poker_evaluation/checker/FiveOfKindChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

ChosenHand FiveOfKindChecker::Check(const Hand& hand)
{
    if (IsFiveOfKind(hand))
    {
        std::cout << "Detected FIVE OF A KIND\n";
        return ChosenHand("Five of a Kind", PokerHandType::FiveOfKind, 1000);
    }
    return ChosenHand();
}

bool FiveOfKindChecker::IsFiveOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 5);
}

