#include "catch.hpp"
#include "scoring/HandScore.h"
#include "scoring/ScoringRule.h"
#include "poker_evaluation/PokerHandType.h"

TEST_CASE("Scoring Subsystem Tests", "[scoring]") {
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
        // Simulating a "planet-driven" upgrade by modifying the table
        table[PokerHandType::Pair] = HandScoreData(15, 3, 2);
        PlayedHandResult result = scoringRule.calculateScore(PokerHandType::Pair, table);
        REQUIRE(result.level == 2);
        REQUIRE(result.chips == 15);
        REQUIRE(result.mult == 3);
        REQUIRE(result.finalScore == 45);
    }

    SECTION("PlayedHandResult immutability") {
        PlayedHandResult result = scoringRule.calculateScore(PokerHandType::Pair, table);
        // The following lines would fail to compile if immutability was not enforced
        // result.chips = 100; 
        // result.finalScore = 1000;
        
        REQUIRE(result.chips == 10);
        REQUIRE(result.finalScore == 20);
        
        // Confirming it's a value object with const members
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
