#include <iostream>
#include "poker_evaluation/checker/ThreeOfKindChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

ChosenHand ThreeOfKindChecker::Check(const Hand& hand)
{
    if (IsThreeOfKind(hand))
    {
        std::cout << "Detected THREE OF A KIND\n";
        return ChosenHand("Three of a Kind", PokerHandType::ThreeOfKind, 40);
    }
    return ChosenHand();
}

bool ThreeOfKindChecker::IsThreeOfKind(const Hand& hand)
{
    return hand.GetCardCount() == 5
        && PokerHandUtils::HasCount(hand, 3)
        && !PokerHandUtils::HasCount(hand, 2);
}

