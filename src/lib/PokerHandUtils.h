#pragma once

#include "Hand.h"

#include <algorithm>
#include <array>
#include <vector>

namespace PokerHandUtils {

inline std::vector<int> GetRanks(const Hand& hand) {
    std::vector<int> ranks;
    for (int i = 0; i < hand.GetCardCount(); ++i) {
        ranks.push_back(hand.GetCard(i).rank);
    }
    return ranks;
}

inline std::vector<char> GetSuits(const Hand& hand) {
    std::vector<char> suits;
    for (int i = 0; i < hand.GetCardCount(); ++i) {
        suits.push_back(hand.GetCard(i).suit);
    }
    return suits;
}

inline std::array<int, 13> GetRankCounts(const Hand& hand) {
    std::array<int, 13> counts = {};
    for (int i = 0; i < hand.GetCardCount(); ++i) {
        int r = hand.GetCard(i).rank;
        if (r >= 0 && r < 13) {
            ++counts[r];
        }
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

    const std::vector<char> suits = GetSuits(hand);
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

    // Ace-low straight (A, 2, 3, 4, 5) -> In our 0-12 rank: 0, 1, 2, 3, 4
    // Wait, the original code had {0, 9, 10, 11, 12} as special case? 
    // That's 10, J, Q, K, A. (0 is Ace, 9 is 10, 10 is J, 11 is Q, 12 is K).
    // So 10-J-Q-K-A is handled by regular straight if A is 12? No, A is 0.
    // So {0, 9, 10, 11, 12} is indeed 10, J, Q, K, A.
    return ranks == std::vector<int>{0, 9, 10, 11, 12};
}

inline bool IsRoyalFlush(const Hand& hand) {
    if (!IsFlush(hand)) {
        return false;
    }

    std::vector<int> ranks = GetRanks(hand);
    std::sort(ranks.begin(), ranks.end());
    // Royal Flush is 10, J, Q, K, A (ranks 9, 10, 11, 12, 0)
    return ranks == std::vector<int>{0, 9, 10, 11, 12};
}

inline Card FromInt(int card) {
    int rank = card % 13;
    int suitIdx = card / 13;
    char suit;
    switch (suitIdx) {
        case 0: suit = 'C'; break;
        case 1: suit = 'D'; break;
        case 2: suit = 'H'; break;
        case 3: suit = 'S'; break;
        default: suit = '?'; break;
    }
    return {rank, suit};
}

}  // namespace PokerHandUtils
