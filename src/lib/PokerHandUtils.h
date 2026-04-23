#pragma once

#include "Hand.h"

#include <algorithm>
#include <array>
#include <vector>

namespace PokerHandUtils {

inline std::vector<int> GetRanks(const Hand& hand) {
    std::vector<int> ranks;
    for (int i = 0; i < hand.GetCardCount(); ++i) {
        ranks.push_back(hand.GetCard(i) % 13);
    }
    return ranks;
}

inline std::vector<int> GetSuits(const Hand& hand) {
    std::vector<int> suits;
    for (int i = 0; i < hand.GetCardCount(); ++i) {
        suits.push_back(hand.GetCard(i) / 13);
    }
    return suits;
}

inline std::array<int, 13> GetRankCounts(const Hand& hand) {
    std::array<int, 13> counts = {};
    for (int i = 0; i < hand.GetCardCount(); ++i) {
        ++counts[hand.GetCard(i) % 13];
    }
    return counts;
}

inline bool HasCount(const Hand& hand, int targetCount) {
    const std::array<int, 13> counts = GetRankCounts(hand);
    for (int count : counts) {
        if (count == targetCount) {
            return true;
        }
    }
    return false;
}

inline int CountRanksWithOccurrences(const Hand& hand, int targetCount) {
    const std::array<int, 13> counts = GetRankCounts(hand);
    int total = 0;
    for (int count : counts) {
        if (count == targetCount) {
            ++total;
        }
    }
    return total;
}

inline bool IsFlush(const Hand& hand) {
    if (hand.GetCardCount() != 5) {
        return false;
    }

    const std::vector<int> suits = GetSuits(hand);
    for (size_t i = 1; i < suits.size(); ++i) {
        if (suits[i] != suits[0]) {
            return false;
        }
    }
    return true;
}

inline bool IsStraight(const Hand& hand) {
    if (hand.GetCardCount() != 5) {
        return false;
    }

    std::vector<int> ranks = GetRanks(hand);
    std::sort(ranks.begin(), ranks.end());

    for (size_t i = 1; i < ranks.size(); ++i) {
        if (ranks[i] == ranks[i - 1]) {
            return false;
        }
    }

    bool isRegularStraight = true;
    for (size_t i = 1; i < ranks.size(); ++i) {
        if (ranks[i] != ranks[0] + static_cast<int>(i)) {
            isRegularStraight = false;
            break;
        }
    }

    if (isRegularStraight) {
        return true;
    }

    return ranks == std::vector<int>{0, 9, 10, 11, 12};
}

inline bool IsRoyalFlush(const Hand& hand) {
    if (!IsFlush(hand)) {
        return false;
    }

    std::vector<int> ranks = GetRanks(hand);
    std::sort(ranks.begin(), ranks.end());
    return ranks == std::vector<int>{0, 9, 10, 11, 12};
}

}  // namespace PokerHandUtils
