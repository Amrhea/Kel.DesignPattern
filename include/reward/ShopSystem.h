#pragma once
#include <vector>
#include <memory>
#include <string>
#include "reward/RewardCommand.h"
#include "session/RuntimeSession.h"

struct ShopItem {
    std::shared_ptr<RewardCommand> command;
    int price;
    std::string description;

    ShopItem(std::shared_ptr<RewardCommand> cmd, int p, std::string desc)
        : command(cmd), price(p), description(desc) {}
};

class ShopSystem {
private:
    std::vector<ShopItem> currentInventory;

public:
    void GenerateInventory(RuntimeSession& session);
    const std::vector<ShopItem>& GetInventory() const;
    bool BuyItem(int index, RuntimeSession& session);
};
