#include <iostream>
#include "poker_evaluation/checker/StraightChecker.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "poker_evaluation/PokerHandType.h"

ChosenHand StraightChecker::Check(const Hand& hand)
{
    if (IsStraight(hand))
    {
        std::cout << "Detected STRAIGHT\n";
        return ChosenHand("Straight", PokerHandType::Straight, 60);
    }
    return ChosenHand();
}

bool StraightChecker::IsStraight(const Hand& hand)
{
    return PokerHandUtils::IsStraight(hand);
}

