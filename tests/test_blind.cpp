#include "catch.hpp"
#include "blind/BlindManager.h"
#include "blind/SmallBlindState.h"
#include "blind/BigBlindState.h"
#include "blind/BossBlindState.h"
#include "session/RuntimeSession.h"
#include "blind/BlindRule.h"

TEST_CASE("Subsystem F: Blind and Round State Tests", "[subsystem_f]") {
    BlindManager manager;
    BlindData data = manager.GetCurrentBlindData();
    RoundState round(data);

    SECTION("Initial State") {
        REQUIRE(round.GetStatus() == RoundStatus::PLAYING);
        REQUIRE(round.GetCurrentScore() == 0);
        REQUIRE(round.GetHandsRemaining() == data.handsLimit);
        REQUIRE(round.GetDiscardsRemaining() == data.discardsLimit);
    }

    SECTION("Continue Behavior") {
        round.AddScore(data.targetScore / 2);
        REQUIRE(round.GetStatus() == RoundStatus::PLAYING);
        
        bool used = round.UseHand();
        REQUIRE(used == true);
        REQUIRE(round.GetHandsRemaining() == data.handsLimit - 1);
        REQUIRE(round.GetStatus() == RoundStatus::PLAYING);
    }

    SECTION("Win Behavior") {
        round.AddScore(data.targetScore);
        REQUIRE(round.GetStatus() == RoundStatus::WON);
    }

    SECTION("Loss Behavior") {
        for (int i = 0; i < data.handsLimit; ++i) {
            REQUIRE(round.GetStatus() == RoundStatus::PLAYING);
            round.UseHand();
        }
        REQUIRE(round.GetStatus() == RoundStatus::LOST);
    }

    SECTION("Discard Decrement") {
        REQUIRE(round.GetDiscardsRemaining() == data.discardsLimit);
        round.UseDiscard();
        REQUIRE(round.GetDiscardsRemaining() == data.discardsLimit - 1);
    }

    SECTION("Blind Progression") {
        REQUIRE(manager.GetCurrentBlindData().type == BlindType::SMALL);
        manager.NextBlind();
        REQUIRE(manager.GetCurrentBlindData().type == BlindType::BIG);
        manager.NextBlind();
        REQUIRE(manager.GetCurrentBlindData().type == BlindType::BOSS);
        manager.NextBlind();
        REQUIRE(manager.GetCurrentBlindData().type == BlindType::SMALL);
        REQUIRE(manager.GetAnte() == 2);
    }
}

TEST_CASE("Subsystem F: Blind State Transitions and Rules", "[subsystem_f_transitions]") {
    int ante = 1;

    SECTION("SmallBlind -> nextState returns BigBlind") {
        SmallBlindState small;
        auto next = small.nextState(ante);
        REQUIRE(next->getName() == "Big Blind");
        REQUIRE(ante == 1);
    }

    SECTION("BigBlind -> nextState returns BossBlind") {
        BigBlindState big;
        auto next = big.nextState(ante);
        REQUIRE(next->getName() == "Boss Blind");
        REQUIRE(ante == 1);
    }

    SECTION("BossBlind -> nextState returns SmallBlind AND increments ante") {
        BossBlindState boss;
        auto next = boss.nextState(ante);
        REQUIRE(next->getName() == "Small Blind");
        REQUIRE(ante == 2);
    }

    SECTION("Each blind returns correct targetScore and rewardMoney") {
        SmallBlindState small;
        REQUIRE(small.getTargetScore(1) == 300);
        REQUIRE(small.getTargetScore(2) == 600);
        REQUIRE(small.getRewardMoney() == 3);

        BigBlindState big;
        REQUIRE(big.getTargetScore(1) == 450);
        REQUIRE(big.getTargetScore(3) == 1350);
        REQUIRE(big.getRewardMoney() == 4);

        BossBlindState boss;
        REQUIRE(boss.getTargetScore(1) == 600);
        REQUIRE(boss.getTargetScore(4) == 2400);
        REQUIRE(boss.getRewardMoney() == 5);
    }

    SECTION("BlindRule Strategy matches required scores") {
        BlindRule smallBlindRule(std::make_unique<SmallBlind>());
        REQUIRE(smallBlindRule.getRequiredScore(2) == 200);
    }
}
