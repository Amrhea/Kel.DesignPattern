#include "catch.hpp"
#include "scoring/HandScore.h"
#include "scoring/ScoreContext.h"
#include "joker/ConcreteJokers.h"
#include "joker/JokerManager.h"
#include "joker/Subject.h"
#include "joker/JokerCard.h"

TEST_CASE("Subsystem E: ScoreContext modification by Jokers", "[subsystem_e]") {
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

TEST_CASE("Subsystem E: PlayedHandResult immutability and ScoreContext isolation", "[subsystem_e]") {
    PlayedHandResult result(PokerHandType::Pair, 10, 2, 1);
    std::vector<Card> cards;
    ScoreContext context(result.handType, "Pair", result.chips, result.mult, cards);
    
    ChipsBoostJoker joker(50);
    joker.apply(context);
    
    REQUIRE(context.chips == 60);
    REQUIRE(result.chips == 10); // Original result unchanged
}

TEST_CASE("Subsystem E: JokerManager accumulation", "[subsystem_e]") {
    std::vector<Card> cards;
    ScoreContext context(PokerHandType::Flush, "Flush", 35, 4, cards);
    
    JokerManager& manager = JokerManager::GetInstance();
    manager.ClearObservers();
    auto joker1 = std::make_unique<ChipsBoostJoker>(15); // 35 + 15 = 50
    auto joker2 = std::make_unique<MultiplierJoker>(6);   // 4 + 6 = 10
    auto joker3 = std::make_unique<ConditionalJoker>(PokerHandType::Flush, 10); // 10 + 10 = 20
    manager.RegisterObserver(joker1.get());
    manager.RegisterObserver(joker2.get());
    manager.RegisterObserver(joker3.get());
    
    manager.NotifyObservers(context);
    
    REQUIRE(context.chips == 50);
    REQUIRE(context.mult == 20);
    REQUIRE(context.calculateFinalScore() == 1000);
}

TEST_CASE("Subsystem E: Observer Pattern - JokerCard implementation", "[subsystem_e]") {
    class TestSubject : public Subject {
    public:
        void TriggerPlay(ScoreContext& context) {
            NotifyObservers(context);
        }
    };

    TestSubject subject;
    JokerCard joker1("Test Joker 1", 10);
    JokerCard joker2("Test Joker 2", 25);

    subject.RegisterObserver(&joker1);
    subject.RegisterObserver(&joker2);

    std::vector<Card> cards;
    ScoreContext context(PokerHandType::Flush, "Flush", 100, 1, cards);
    subject.TriggerPlay(context);
    REQUIRE(context.chips == 135);

    subject.RemoveObserver(&joker1);
    ScoreContext context2(PokerHandType::Flush, "Flush", 100, 1, cards);
    subject.TriggerPlay(context2);
    REQUIRE(context2.chips == 125);
}
