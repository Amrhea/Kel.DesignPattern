#include <iostream>
#include "../lib/checker/StraightChecker.h"
#include "../lib/PokerHandUtils.h"
#include "../lib/PokerHandType.h"

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
