#include "catch.hpp"
#include "session/RuntimeSession.h"
#include "blind/SmallBlindState.h"
#include "blind/BossBlindState.h"
#include "reward/BonusHandCommand.h"
#include "reward/FreePlayingCardCommand.h"

TEST_CASE("Reward Command Tests", "[reward]") {
    SECTION("Skipping SmallBlind creates a RewardCommand with correct timing") {
        SmallBlindState small;
        auto cmd = small.createSkipRewardCommand();
        REQUIRE(cmd != nullptr);
        REQUIRE(cmd->getName() == "Bonus Hand");
        REQUIRE(cmd->getTiming() == "NextBlind");
    }

    SECTION("BonusHandCommand::execute() increments remainingPlays by 1") {
        RuntimeSession session;
        session.remainingPlays = 4;
        BonusHandCommand cmd;
        cmd.execute(session);
        REQUIRE(session.remainingPlays == 5);
    }

    SECTION("FreePlayingCardCommand::execute() adds 1 card to session.deck") {
        RuntimeSession session;
        int initialSize = session.deck.size();
        FreePlayingCardCommand cmd;
        cmd.execute(session);
        REQUIRE(session.deck.size() == initialSize + 1);
        REQUIRE(session.deck.back() == "PlayingCard");
    }

    SECTION("executePendingCommands timing filtering and queue cleanup") {
        RuntimeSession session;
        auto cmd1 = std::make_shared<BonusHandCommand>();      // NextBlind
        auto cmd2 = std::make_shared<FreePlayingCardCommand>(); // NextAnte

        session.pendingCommands.push_back(cmd1);
        session.pendingCommands.push_back(cmd2);

        // Execute NextBlind timing
        session.executePendingCommands("NextBlind");
        REQUIRE(session.remainingPlays == 5); // cmd1 executed
        REQUIRE(session.deck.size() == 0);     // cmd2 not executed
        REQUIRE(session.pendingCommands.size() == 1);
        REQUIRE(session.pendingCommands[0] == cmd2);

        // Execute NextAnte timing
        session.executePendingCommands("NextAnte");
        REQUIRE(session.deck.size() == 1);     // cmd2 executed
        REQUIRE(session.pendingCommands.size() == 0);
    }
}

TEST_CASE("Integration Tests", "[integration]") {
    SECTION("Skip SmallBlind -> tag queued -> advance to BigBlind -> NextBlind trigger executes tag") {
        RuntimeSession session;
        REQUIRE(session.currentBlind->getName() == "Small Blind");
        REQUIRE(session.remainingPlays == 4);

        session.skipBlind(); // Skips Small Blind, transitions to Big Blind
        // During skipBlind():
        // 1. HandyTag is added to tagStack.
        // 2. State advances to Big Blind.
        // 3. triggerTags(TagTrigger::NEXT_BLIND) runs.
        REQUIRE(session.currentBlind->getName() == "Big Blind");
        REQUIRE(session.remainingPlays == 5); // +1 play from tag
        REQUIRE(session.tagStack.size() == 0);
    }

    SECTION("Skip BossBlind -> ante increments -> NextAnte trigger executes NextAnte tags") {
        RuntimeSession session;
        // Move to Boss Blind
        session.playBlind(); // to Big
        session.playBlind(); // to Boss
        REQUIRE(session.currentBlind->getName() == "Boss Blind");
        REQUIRE(session.ante == 1);

        session.skipBlind(); // Skips Boss Blind, transitions to Small Blind (Ante 2)
        // During skipBlind():
        // 1. Skips Boss Blind (adds OrbitalTag).
        // 2. Advances to Small Blind, ante becomes 2.
        // 3. Since ante increased, triggerTags(TagTrigger::NEXT_ANTE) runs.
        REQUIRE(session.currentBlind->getName() == "Small Blind");
        REQUIRE(session.ante == 2);
        REQUIRE(session.tagStack.size() == 0);
    }
}
