#include "catch.hpp"
#include "session/RuntimeSession.h"
#include "reward/PlanetCardCommand.h"
#include "reward/JokerRewardCommand.h"
#include "reward/RewardManager.h"
#include "reward/ShopSystem.h"

TEST_CASE("PlanetCardCommand upgrades target hand", "[reward]") {
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

TEST_CASE("JokerRewardCommand adds joker to session", "[reward]") {
    RuntimeSession session;
    size_t initialSize = session.jokers.size();

    auto joker = std::make_shared<JokerCard>("Test Joker", 50);
    JokerRewardCommand cmd(joker);
    cmd.execute(session);

    REQUIRE(session.jokers.size() == initialSize + 1);
    REQUIRE(session.jokers.back()->getName() == "Test Joker");
}

TEST_CASE("ShopSystem buying items", "[shop]") {
    RuntimeSession session;
    session.gold = 10;
    
    ShopSystem shop;
    shop.GenerateInventory();
    
    auto inventory = shop.GetInventory();
    REQUIRE(inventory.size() == 3);
    
    int price = inventory[0].price;
    bool success = shop.BuyItem(0, session);
    
    REQUIRE(success == true);
    REQUIRE(session.gold == 10 - price);
    REQUIRE(shop.GetInventory().size() == 2);
}

TEST_CASE("ShopSystem cannot buy if not enough gold", "[shop]") {
    RuntimeSession session;
    session.gold = 0;
    
    ShopSystem shop;
    shop.GenerateInventory();
    
    bool success = shop.BuyItem(0, session);
    
    REQUIRE(success == false);
    REQUIRE(session.gold == 0);
    REQUIRE(shop.GetInventory().size() == 3);
}

TEST_CASE("RewardManager generates rewards", "[reward]") {
    auto rewards = RewardManager::GeneratePostBlindRewards();
    REQUIRE(rewards.size() == 2);
    for (auto& r : rewards) {
        REQUIRE(r != nullptr);
    }
}
