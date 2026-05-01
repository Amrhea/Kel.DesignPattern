#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/lib/Hand.h"
#include "../src/lib/checker/PairChecker.h"
#include "../src/lib/checker/ThreeOfKindChecker.h"
#include "../src/lib/PokerHandUtils.h"

// Helper function to create a Hand from a list of ints using the FromInt utility
Hand CreateHand(std::vector<int> cards) {
    Hand hand;
    for (int cardInt : cards) {
        hand.AddCard(PokerHandUtils::FromInt(cardInt));
    }
    return hand;
}

TEST_CASE("PairChecker Tests", "[checker]") {
    PairChecker checker;

    SECTION("Valid Pair") {
        // Ranks: 0, 0, 1, 2, 3 (Two Aces and three other different cards)
        Hand hand = CreateHand({0, 13, 1, 2, 3});
        ChosenHand result = checker.Check(hand);
        REQUIRE(result.isValid() == true);
        REQUIRE(result.handName == "Pair");
    }

    SECTION("No Pair (High Card)") {
        // Ranks: 0, 1, 2, 3, 4
        Hand hand = CreateHand({0, 1, 2, 3, 4});
        ChosenHand result = checker.Check(hand);
        REQUIRE(result.isValid() == false);
    }

    SECTION("Three of a Kind should not be a Pair") {
        // Ranks: 0, 0, 0, 1, 2
        Hand hand = CreateHand({0, 13, 26, 1, 2});
        ChosenHand result = checker.Check(hand);
        REQUIRE(result.isValid() == false);
    }

    SECTION("Two Pair should not be a Pair") {
        // Ranks: 0, 0, 1, 1, 2
        Hand hand = CreateHand({0, 13, 1, 14, 2});
        ChosenHand result = checker.Check(hand);
        REQUIRE(result.isValid() == false);
    }
}

TEST_CASE("ThreeOfKindChecker Tests", "[checker]") {
    ThreeOfKindChecker checker;

    SECTION("Valid Three of a Kind") {
        // Ranks: 5, 5, 5, 1, 2
        Hand hand = CreateHand({5, 18, 31, 1, 2});
        ChosenHand result = checker.Check(hand);
        REQUIRE(result.isValid() == true);
        REQUIRE(result.handName == "Three of a Kind");
    }

    SECTION("Four of a Kind should not be Three of a Kind") {
        // Ranks: 5, 5, 5, 5, 1
        Hand hand = CreateHand({5, 18, 31, 44, 1});
        ChosenHand result = checker.Check(hand);
        REQUIRE(result.isValid() == false);
    }
}
