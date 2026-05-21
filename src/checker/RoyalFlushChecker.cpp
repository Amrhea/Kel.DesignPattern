#include <iostream>
#include "../lib/checker/RoyalFlushChecker.h"
#include "../lib/PokerHandUtils.h"
#include "../lib/PokerHandType.h"

ChosenHand RoyalFlushChecker::Check(const Hand& hand)
{
    if (IsRoyalFlush(hand))
    {
        std::cout << "Detected ROYAL FLUSH\n";
        return ChosenHand("Royal Flush", PokerHandType::RoyalFlush, 500);
    }
    return ChosenHand();
}

bool RoyalFlushChecker::IsRoyalFlush(const Hand& hand)
{
    return PokerHandUtils::IsRoyalFlush(hand);
}
