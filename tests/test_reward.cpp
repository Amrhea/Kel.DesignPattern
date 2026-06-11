#include "catch.hpp"
#include "session/RuntimeSession.h"
#include "blind/SmallBlindState.h"
#include "blind/BossBlindState.h"
#include "reward/BonusHandCommand.h"
#include "reward/FreePlayingCardCommand.h"
#include "reward/PlanetCardCommand.h"
#include "reward/JokerRewardCommand.h"
#include "reward/RewardManager.h"
#include "reward/ShopSystem.h"

TEST_CASE("Subsystem G: Reward Command Tests", "[subsystem_g]") {
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

    SECTION("FreePlayingCardCommand::execute() adds 1 card to session.deck and persistentDeck") {
        RuntimeSession session;
        int initialSize = session.deck.size();
        int initialPersistentSize = session.persistentDeck.size();
        FreePlayingCardCommand cmd;
        cmd.execute(session);
        REQUIRE(session.deck.size() == initialSize + 1);
        REQUIRE(session.deck.back() == "PlayingCard");
        REQUIRE(session.persistentDeck.size() == initialPersistentSize + 1);
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

TEST_CASE("Subsystem G: Planet and Joker Reward Commands", "[subsystem_g_rewards]") {
    SECTION("PlanetCardCommand upgrades target hand") {
        RuntimeSession session;
        int initialChips = session.handScores[PokerHandType::Pair].baseChips;
        int initialMult = session.handScores[PokerHandType::Pair].baseMult;
        int initialLevel = session.handScores[PokerHandType::Pair].level;

        PlanetCardCommand cmd(PokerHandType::Pair, 15, 2);
        cmd.execute(session);

        REQUIRE(session.handScores[PokerHandType::Pair].level == initialLevel + 1);
        REQUIRE(session.handScores[PokerHandType::Pair].baseChips == initialChips + 15);
        REQUIRE(session.handScores[PokerHandType::Pair].baseMult == initialMult + 2);
        
        // Ensure other hands are NOT upgraded
        REQUIRE(session.handScores[PokerHandType::Flush].level == 1);
    }

    SECTION("JokerRewardCommand adds joker to session") {
        RuntimeSession session;
        size_t initialSize = session.jokers.size();

        auto joker = std::make_shared<JokerCard>("Test Joker", 50);
        JokerRewardCommand cmd(joker);
        cmd.execute(session);

        REQUIRE(session.jokers.size() == initialSize + 1);
        auto retrievedJoker = std::dynamic_pointer_cast<JokerCard>(session.jokers.back());
        REQUIRE(retrievedJoker != nullptr);
        REQUIRE(retrievedJoker->getName() == "Test Joker");
    }
}

TEST_CASE("Subsystem G: ShopSystem and RewardManager", "[subsystem_g_shop]") {
    SECTION("ShopSystem buying items") {
        RuntimeSession session;
        session.gold = 10;
        
        ShopSystem shop;
        shop.GenerateInventory(session);
        
        auto inventory = shop.GetInventory();
        REQUIRE(inventory.size() == 3);
        
        int price = inventory[0].price;
        bool success = shop.BuyItem(0, session);
        
        REQUIRE(success == true);
        REQUIRE(session.gold == 10 - price);
        REQUIRE(shop.GetInventory().size() == 2);
    }

    SECTION("ShopSystem cannot buy if not enough gold") {
        RuntimeSession session;
        session.gold = 0;
        
        ShopSystem shop;
        shop.GenerateInventory(session);
        
        bool success = shop.BuyItem(0, session);
        
        REQUIRE(success == false);
        REQUIRE(session.gold == 0);
        REQUIRE(shop.GetInventory().size() == 3);
    }

    SECTION("RewardManager generates rewards") {
        auto rewards = RewardManager::GeneratePostBlindRewards();
        REQUIRE(rewards.size() == 2);
        for (auto& r : rewards) {
            REQUIRE(r != nullptr);
        }
    }
}
