#include "reward/ShopSystem.h"
#include "reward/PlanetCardCommand.h"
#include "reward/JokerRewardCommand.h"
#include "joker/JokerFactory.h"
#include <cstdlib>

void ShopSystem::GenerateInventory(RuntimeSession& session) {
    currentInventory.clear();
    
    // Randomly generate 3 items
    for (int i = 0; i < 3; ++i) {
        int type = std::rand() % 2;
        if (type == 0) {
            PokerHandType hand = static_cast<PokerHandType>(std::rand() % (int)PokerHandType::StraightFlush + 1);
            auto cmd = std::make_shared<PlanetCardCommand>(hand, 15, 2);
            currentInventory.emplace_back(cmd, 3, "Upgrade " + cmd->getName());
        } else {
            auto joker = JokerFactory::CreateRandomJoker();
            auto cmd = std::make_shared<JokerRewardCommand>(joker);
            currentInventory.emplace_back(cmd, 4, "Buy " + joker->getName());
        }
    }
}

const std::vector<ShopItem>& ShopSystem::GetInventory() const {
    return currentInventory;
}

bool ShopSystem::BuyItem(int index, RuntimeSession& session) {
    if (index < 0 || index >= (int)currentInventory.size()) return false;
    
    const auto& item = currentInventory[index];
    if (session.gold >= item.price) {
        session.gold -= item.price;
        if (item.command) {
            item.command->execute(session);
        }
        // Remove item from inventory after purchase
        currentInventory.erase(currentInventory.begin() + index);
        return true;
    }
    return false;
}
