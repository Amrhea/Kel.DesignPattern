#include <iostream>
#include "../lib/checker/FlushChecker.h"
#include "../lib/PokerHandUtils.h"
#include "../lib/PokerHandType.h"

ChosenHand FlushChecker::Check(const Hand& hand)
{
    if (IsFlush(hand))
    {
        std::cout << "Detected FLUSH\n";
        return ChosenHand("Flush", PokerHandType::Flush, 80);
    }
    return ChosenHand();
}

bool FlushChecker::IsFlush(const Hand& hand)
{
    return PokerHandUtils::IsFlush(hand);
}
