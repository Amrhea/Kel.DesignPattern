#include "catch.hpp"
#include "hand_selection/Hand.h"
#include "poker_evaluation/PokerHandEvaluator.h"
#include "poker_evaluation/PokerHandUtils.h"
#include <vector>

// Helper function to create a Hand from a list of ints using the FromInt utility
static Hand CreateHand(std::vector<int> cards) {
    Hand hand;
    for (int cardInt : cards) {
        hand.AddCard(PokerHandUtils::FromInt(cardInt));
    }
    return hand;
}

TEST_CASE("Subsystem C: PokerHandEvaluator - Priority and Special Hands", "[subsystem_c]") {
    PokerHandEvaluator evaluator;

    SECTION("Royal Flush Priority") {
        Hand hand = CreateHand({47, 48, 49, 50, 51});
        HandEvaluation result = evaluator.Evaluate(hand);
        REQUIRE(result.handType == PokerHandType::RoyalFlush);
    }

    SECTION("Flush Five over Five of a Kind") {
        Hand hand;
        for(int i=0; i<5; ++i) hand.AddCard(PokerHandUtils::FromInt(51));
        
        HandEvaluation result = evaluator.Evaluate(hand);
        REQUIRE(result.handType == PokerHandType::FlushFive);
    }

    SECTION("Five of a Kind (different suits)") {
        Hand hand = CreateHand({12, 25, 38, 51});
        hand.AddCard(PokerHandUtils::FromInt(51)); 
        
        HandEvaluation result = evaluator.Evaluate(hand);
        REQUIRE(result.handType == PokerHandType::FiveOfKind);
    }

    SECTION("Duplicate-rank Straight Rejection") {
        Hand hand = CreateHand({0, 13, 1, 2, 3});
        HandEvaluation result = evaluator.Evaluate(hand);
        REQUIRE(result.handType != PokerHandType::Straight);
        REQUIRE(result.handType == PokerHandType::Pair);
    }

    SECTION("2-card HIGH_CARD") {
        Hand hand = CreateHand({12, 0});
        HandEvaluation result = evaluator.Evaluate(hand);
        REQUIRE(result.handType == PokerHandType::HighCard);
    }

    SECTION("Flush Detection") {
        Hand hand = CreateHand({0, 2, 4, 6, 8});
        HandEvaluation result = evaluator.Evaluate(hand);
        REQUIRE(result.handType == PokerHandType::Flush);
    }
}
