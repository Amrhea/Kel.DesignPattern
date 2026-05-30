#include "reward/RewardManager.h"
#include "reward/PlanetCardCommand.h"
#include "reward/JokerRewardCommand.h"
#include <cstdlib>
#include <ctime>

std::vector<std::shared_ptr<RewardCommand>> RewardManager::GeneratePostBlindRewards() {
    std::vector<std::shared_ptr<RewardCommand>> rewards;
    
    // Seed random if not already done
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }

    // Generate 2 random rewards for choice (simplified)
    for (int i = 0; i < 2; ++i) {
        int type = std::rand() % 2;
        if (type == 0) {
            // Planet Card
            PokerHandType hand = static_cast<PokerHandType>(std::rand() % (int)PokerHandType::StraightFlush + 1);
            rewards.push_back(std::make_shared<PlanetCardCommand>(hand, 15, 2));
        } else {
            // Joker
            rewards.push_back(std::make_shared<JokerRewardCommand>(std::make_shared<JokerCard>("Lucky Joker", 20)));
        }
    }

    return rewards;
}
