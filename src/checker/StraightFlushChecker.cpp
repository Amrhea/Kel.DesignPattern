#include <iostream>
#include "../lib/checker/StraightFlushChecker.h"

bool StraightFlushChecker::Check(const Hand& hand)
{
    if (IsStraightFlush(hand))
    {
        std::cout << "Detected STRAIGHT FLUSH\n";
        return true;
    }
    return false;
}

bool StraightFlushChecker::IsStraightFlush(const Hand& hand)
{
    // Straight flush is both a straight and a flush
    // To be implemented with helper checkers
    return false;
}
