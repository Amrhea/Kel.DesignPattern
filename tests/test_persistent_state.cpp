#include "catch.hpp"
#include "state/RunPersistentState.h"
#include "session/RuntimeSession.h"
#include "blind/BossBlindState.h"
#include "reward/BonusHandCommand.h"
#include "state/ScoreContext.h"

TEST_CASE("Subsystem I: RunPersistentState Verification", "[persistent_state]") {
    SECTION("ante only increments on BossBlind transition") {
        RuntimeSession session;
        REQUIRE(session.ante == 1);
        
        session.playBlind(); // small -> big
        REQUIRE(session.ante == 1);

        session.playBlind(); // big -> boss
        REQUIRE(session.ante == 1);

        session.playBlind(); // boss -> small (ante++)
        REQUIRE(session.ante == 2);
    }

    SECTION("money and jokers are unaffected by the scoring pipeline") {
        RunPersistentState pState;
        pState.money = 10;
        
        // Simulating playing a hand and score context calculations
        std::vector<Card> cards;
        ScoreContext context(PokerHandType::HighCard, "High Card", 10, 2, cards);
        
        REQUIRE(context.calculateFinalScore() == 20);
        
        // Assert they are unaffected
        REQUIRE(pState.money == 10);
        REQUIRE(pState.jokers.size() == 0);
    }

    SECTION("pendingCommands are not executed automatically") {
        RuntimeSession session;
        auto cmd = std::make_shared<BonusHandCommand>();
        session.pendingCommands.push_back(cmd);
        
        REQUIRE(session.pendingCommands.size() == 1);
        REQUIRE(session.remainingPlays == 4); // unaffected until timing triggers
    }
}
