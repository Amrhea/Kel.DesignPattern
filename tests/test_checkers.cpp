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
        REQUIRE(result.handType == PokerHandType::Pair);
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
        REQUIRE(result.handType == PokerHandType::ThreeOfKind);
    }

    SECTION("Four of a Kind should not be Three of a Kind") {
        // Ranks: 5, 5, 5, 5, 1
        Hand hand = CreateHand({5, 18, 31, 44, 1});
        ChosenHand result = checker.Check(hand);
        REQUIRE(result.isValid() == false);
    }
}

#include "../src/lib/Observer.h"
#include "../src/lib/Subject.h"
#include "../src/lib/JokerCard.h"
#include "../src/lib/ScoringRule.h"
#include "../src/lib/BlindRule.h"
#include "../src/lib/RewardRule.h"
#include "../src/lib/ScoreCalculator.h"
#include "../src/lib/ConcreteScoreCalculators.h"
#include "../src/lib/GameManage.h"
#include "../src/lib/HandHandler.h"

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

    // Initial 100 + 10 (joker1) + 25 (joker2) = 135
    REQUIRE(score == 135);

    subject.RemoveObserver(&joker1);
    score = 100;
    subject.TriggerPlay("Flush", score);

    // Initial 100 + 25 (joker2 only) = 125
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

        BlindRule bigBlindRule(std::make_unique<BigBlind>());
        REQUIRE(bigBlindRule.getRequiredScore(2) == 300);

        BlindRule bossBlindRule(std::make_unique<BossBlind>());
        REQUIRE(bossBlindRule.getRequiredScore(2) == 600);
    }

    SECTION("Reward Strategy") {
        RewardRule standardReward(std::make_unique<StandardReward>());
        REQUIRE(standardReward.calculateReward(50) == 100);

        RewardRule generousReward(std::make_unique<GenerousReward>());
        REQUIRE(generousReward.calculateReward(50) == 150);
    }
}

TEST_CASE("Template Method Pattern Tests - Score Calculator", "[template]") {
    HandHandler handler;
    // Create a Pair hand
    Hand hand = CreateHand({0, 13, 1, 2, 3});

    SECTION("Standard Score Calculator") {
        StandardScoreCalculator calc;
        // Pair baseScore is 20
        REQUIRE(calc.CalculateScore(hand, handler) == 20);
    }

    SECTION("Bonus Score Calculator") {
        BonusScoreCalculator calc;
        // Pair is modified by +10 -> 30
        REQUIRE(calc.CalculateScore(hand, handler) == 30);
    }
}

TEST_CASE("Singleton Pattern Tests - GameManager", "[singleton]") {
    GameManager* instance1 = GameManager::GetInstance();
    GameManager* instance2 = GameManager::GetInstance();
    REQUIRE(instance1 == instance2);
}

