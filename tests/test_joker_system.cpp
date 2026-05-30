#include "catch.hpp"
#include "scoring/HandScore.h"
#include "scoring/ScoreContext.h"
#include "joker/ConcreteJokers.h"
#include "joker/JokerManager.h"

TEST_CASE("ScoreContext modification by Jokers", "[joker]") {
    std::vector<Card> cards; // Empty for test
    ScoreContext context(PokerHandType::Pair, "Pair", 10, 2, cards);
    
    SECTION("ChipsBoostJoker adds chips") {
        ChipsBoostJoker joker(20);
        joker.apply(context);
        REQUIRE(context.chips == 30);
        REQUIRE(context.mult == 2);
    }
    
    SECTION("MultiplierJoker adds mult") {
        MultiplierJoker joker(5);
        joker.apply(context);
        REQUIRE(context.chips == 10);
        REQUIRE(context.mult == 7);
    }
    
    SECTION("ConditionalJoker triggers on correct hand") {
        ConditionalJoker joker(PokerHandType::Pair, 10);
        joker.apply(context);
        REQUIRE(context.mult == 12);
    }
    
    SECTION("ConditionalJoker does not trigger on wrong hand") {
        ConditionalJoker joker(PokerHandType::Flush, 10);
        joker.apply(context);
        REQUIRE(context.mult == 2);
    }
}

TEST_CASE("PlayedHandResult immutability and ScoreContext isolation", "[scoring]") {
    PlayedHandResult result(PokerHandType::Pair, 10, 2, 1);
    std::vector<Card> cards;
    ScoreContext context(result.handType, "Pair", result.chips, result.mult, cards);
    
    ChipsBoostJoker joker(50);
    joker.apply(context);
    
    REQUIRE(context.chips == 60);
    REQUIRE(result.chips == 10); // Original result unchanged
}

TEST_CASE("JokerManager accumulation", "[joker]") {
    std::vector<Card> cards;
    ScoreContext context(PokerHandType::Flush, "Flush", 35, 4, cards);
    
    JokerManager manager; // Use local manager for test isolation
    manager.RegisterObserver(new ChipsBoostJoker(15)); // 35 + 15 = 50
    manager.RegisterObserver(new MultiplierJoker(6));   // 4 + 6 = 10
    manager.RegisterObserver(new ConditionalJoker(PokerHandType::Flush, 10)); // 10 + 10 = 20
    
    manager.NotifyObservers(context);
    
    REQUIRE(context.chips == 50);
    REQUIRE(context.mult == 20);
    REQUIRE(context.calculateFinalScore() == 1000);
}
