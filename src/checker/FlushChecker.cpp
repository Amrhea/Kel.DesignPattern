#include <iostream>
#include "../lib/checker/FlushChecker.h"
#include "../lib/PokerHandUtils.h"

ChosenHand FlushChecker::Check(const Hand& hand)
{
    if (IsFlush(hand))
    {
        std::cout << "Detected FLUSH\n";
        return ChosenHand("Flush", 200);
    }
    return ChosenHand();
}

bool FlushChecker::IsFlush(const Hand& hand)
{
    return PokerHandUtils::IsFlush(hand) && !PokerHandUtils::IsStraight(hand);
}
