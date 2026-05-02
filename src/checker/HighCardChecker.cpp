#include <iostream>
#include "../lib/checker/HighCardChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand HighCardChecker::Check(const Hand& hand)
{
    if (IsHighCard(hand))
    {
        std::cout << "Detected HIGH CARD\n";
        return ChosenHand("High Card", 10);
    }
    return ChosenHand();
}

bool HighCardChecker::IsHighCard(const Hand& hand)
{
    // If it reached here, it didn't match any stronger hand
    return hand.GetCardCount() == 5;
}
