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
    SECTION("Skip SmallBlind -> command queued -> advance to BigBlind -> NextBlind trigger executes command") {
        RuntimeSession session;
        REQUIRE(session.currentBlind->getName() == "Small Blind");
        REQUIRE(session.remainingPlays == 4);

        session.skipBlind(); // Skips Small Blind, transitions to Big Blind
        // During skipBlind():
        // 1. BonusHandCommand is queued.
        // 2. State advances to Big Blind.
        // 3. executePendingCommands("NextBlind") runs.
        REQUIRE(session.currentBlind->getName() == "Big Blind");
        REQUIRE(session.remainingPlays == 5); // +1 play from command
        REQUIRE(session.pendingCommands.size() == 0);
    }

    SECTION("Skip BossBlind -> ante increments -> NextAnte trigger executes NextAnte commands") {
        RuntimeSession session;
        // Move to Boss Blind
        session.playBlind(); // to Big
        session.playBlind(); // to Boss
        REQUIRE(session.currentBlind->getName() == "Boss Blind");
        REQUIRE(session.ante == 1);

        // Queue a NextAnte command manually to verify NextAnte trigger works
        auto cmd = std::make_shared<FreePlayingCardCommand>(); // NextAnte
        session.pendingCommands.push_back(cmd);

        session.skipBlind(); // Skips Boss Blind, transitions to Small Blind (Ante 2)
        // During skipBlind():
        // 1. Skips Boss Blind (queues FreePlayingCardCommand).
        // 2. Advances to Small Blind, ante becomes 2.
        // 3. executePendingCommands("NextBlind") runs (nothing to execute).
        // 4. Since ante increased, executePendingCommands("NextAnte") runs.
        REQUIRE(session.currentBlind->getName() == "Small Blind");
        REQUIRE(session.ante == 2);
        // FreePlayingCardCommand executed twice (one manual, one from skipping Boss Blind)
        REQUIRE(session.deck.size() == 2);
        REQUIRE(session.pendingCommands.size() == 0);
    }
}
