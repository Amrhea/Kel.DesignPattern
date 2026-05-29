#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "hand_selection/Hand.h"
#include "poker_evaluation/PokerHandEvaluator.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "scoring/ScoreCalculator.h"
#include "scoring/ConcreteScoreCalculators.h"
#include "joker/Observer.h"
#include "joker/Subject.h"
#include "joker/JokerCard.h"
#include "scoring/ScoringRule.h"
#include "blind/BlindRule.h"
#include "reward/RewardRule.h"
#include "run/GameManage.h"

// Helper function to create a Hand from a list of ints using the FromInt utility
Hand CreateHand(std::vector<int> cards) {
    Hand hand;
    for (int cardInt : cards) {
        hand.AddCard(PokerHandUtils::FromInt(cardInt));
    }
    return hand;
}

TEST_CASE("PokerHandEvaluator - Priority and Special Hands", "[evaluator]") {
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

TEST_CASE("Observer Pattern Tests - Joker Cards", "[observer]") {
    class TestSubject : public Subject {
    public:
        void TriggerPlay(const std::string& name, int& score) {
            NotifyObservers(name, score);
        }
    };

    TestSubject subject;
    JokerCard joker1("Test Joker 1", 10);
    JokerCard joker2("Test Joker 2", 25);

    subject.RegisterObserver(&joker1);
    subject.RegisterObserver(&joker2);

    int score = 100;
    subject.TriggerPlay("Flush", score);
    REQUIRE(score == 135);

    subject.RemoveObserver(&joker1);
    score = 100;
    subject.TriggerPlay("Flush", score);
    REQUIRE(score == 125);
}

TEST_CASE("Strategy Pattern Tests - Rules", "[strategy]") {
    SECTION("Scoring Strategy") {
        HandScoreTable table;
        table[PokerHandType::Flush] = HandScoreData(80, 4, 1);
        
        ScoringRule standardRule(std::make_unique<BaseScoringRule>());
        PlayedHandResult result = standardRule.calculateScore(PokerHandType::Flush, table);
        REQUIRE(result.finalScore == 320);
    }

    SECTION("Blind Strategy") {
        BlindRule smallBlindRule(std::make_unique<SmallBlind>());
        REQUIRE(smallBlindRule.getRequiredScore(2) == 200);
    }
}

TEST_CASE("Template Method Pattern Tests - Score Calculator", "[template]") {
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

TEST_CASE("Singleton Pattern Tests - GameManager", "[singleton]") {
    GameManager* instance1 = GameManager::GetInstance();
    GameManager* instance2 = GameManager::GetInstance();
    REQUIRE(instance1 == instance2);
}
