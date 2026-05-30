#include "catch.hpp"
#include "scoring/HandScore.h"
#include "scoring/ScoringRule.h"
#include "scoring/ScoreCalculator.h"
#include "scoring/ConcreteScoreCalculators.h"
#include "poker_evaluation/PokerHandType.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "hand_selection/Hand.h"

// Helper function to create a Hand from a list of ints using the FromInt utility
static Hand CreateHand(std::vector<int> cards) {
    Hand hand;
    for (int cardInt : cards) {
        hand.AddCard(PokerHandUtils::FromInt(cardInt));
    }
    return hand;
}

TEST_CASE("Subsystem D: Scoring Subsystem Tests", "[subsystem_d]") {
    HandScoreTable table;
    table[PokerHandType::Pair] = HandScoreData(10, 2, 1);
    table[PokerHandType::ThreeOfKind] = HandScoreData(30, 3, 1);
    
    ScoringRule scoringRule(std::make_unique<BaseScoringRule>());

    SECTION("Pair lookup correctness") {
        PlayedHandResult result = scoringRule.calculateScore(PokerHandType::Pair, table);
        REQUIRE(result.handType == PokerHandType::Pair);
        REQUIRE(result.chips == 10);
        REQUIRE(result.mult == 2);
        REQUIRE(result.level == 1);
    }

    SECTION("finalScore equals chips times mult") {
        PlayedHandResult result = scoringRule.calculateScore(PokerHandType::ThreeOfKind, table);
        REQUIRE(result.chips == 30);
        REQUIRE(result.mult == 3);
        REQUIRE(result.finalScore == 90);
    }

    SECTION("Planet-driven level upgrades") {
        table[PokerHandType::Pair] = HandScoreData(15, 3, 2);
        PlayedHandResult result = scoringRule.calculateScore(PokerHandType::Pair, table);
        REQUIRE(result.level == 2);
        REQUIRE(result.chips == 15);
        REQUIRE(result.mult == 3);
        REQUIRE(result.finalScore == 45);
    }

    SECTION("PlayedHandResult immutability") {
        PlayedHandResult result = scoringRule.calculateScore(PokerHandType::Pair, table);
        REQUIRE(result.chips == 10);
        REQUIRE(result.finalScore == 20);
        
        static_assert(std::is_const<decltype(result.chips)>::value, "chips should be const");
        static_assert(std::is_const<decltype(result.finalScore)>::value, "finalScore should be const");
    }

    SECTION("Unknown hand type returns zero score") {
        PlayedHandResult result = scoringRule.calculateScore(PokerHandType::Flush, table);
        REQUIRE(result.chips == 0);
        REQUIRE(result.mult == 0);
        REQUIRE(result.finalScore == 0);
    }
}

TEST_CASE("Subsystem D: Template Method Pattern Tests - Score Calculator", "[subsystem_d_template]") {
    PokerHandEvaluator handler;
    Hand hand = CreateHand({0, 13, 1, 2, 3});

    SECTION("Standard Score Calculator") {
        StandardScoreCalculator calc;
        REQUIRE(calc.CalculateScore(hand, handler) == 20);
    }

    SECTION("Bonus Score Calculator") {
        BonusScoreCalculator calc;
        REQUIRE(calc.CalculateScore(hand, handler) == 30);
    }
}
