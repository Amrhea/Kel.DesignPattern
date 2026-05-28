#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "blind/BlindManager.h"

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
        // Use all hands without reaching target score
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
