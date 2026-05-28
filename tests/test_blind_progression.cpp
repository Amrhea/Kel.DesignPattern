#include "catch.hpp"
#include "blind/SmallBlindState.h"
#include "blind/BigBlindState.h"
#include "blind/BossBlindState.h"
#include "session/RuntimeSession.h"

TEST_CASE("Blind Progression Tests", "[blind]") {
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

    SECTION("BossBlind skipped -> ante still increments on transition") {
        RuntimeSession session;
        // Move to BossBlind
        session.playBlind(); // to Big
        session.playBlind(); // to Boss
        REQUIRE(session.currentBlind->getName() == "Boss Blind");
        REQUIRE(session.ante == 1);

        session.skipBlind(); // Skip Boss
        REQUIRE(session.currentBlind->getName() == "Small Blind");
        REQUIRE(session.ante == 2);
    }

    SECTION("Full cycle SmallBlind -> BigBlind -> BossBlind -> SmallBlind completes without crash") {
        RuntimeSession session;
        REQUIRE(session.currentBlind->getName() == "Small Blind");
        REQUIRE(session.ante == 1);

        session.playBlind();
        REQUIRE(session.currentBlind->getName() == "Big Blind");

        session.playBlind();
        REQUIRE(session.currentBlind->getName() == "Boss Blind");

        session.playBlind();
        REQUIRE(session.currentBlind->getName() == "Small Blind");
        REQUIRE(session.ante == 2);
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
}
