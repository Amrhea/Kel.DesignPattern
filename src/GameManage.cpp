#include "lib/GameManage.h"
#include "lib/Hand.h"
#include "lib/HandHandler.h"

#include <iostream>

#include "lib/PokerHandUtils.h"

GameManager::GameManager() {
}

GameManager::~GameManager() {
}

void GameManager::RunSession() {
    HandHandler handHandler;
    std::cout << "--- Poker Game Session Started ---" << std::endl;

    // Pipeline: Generate -> Play -> Evaluate -> Check Blind -> Earn Money -> Print
    
    // 1. Generate
    std::cout << "[System] Generating randomized hand..." << std::endl;
    Hand hand = HandGenerator::generateHand();

    // 2. Play
    std::cout << "[System] Current Hand:" << std::endl;
    handHandler.ShowCards(hand);

    // 3. Evaluate
    std::cout << "[System] Evaluating hand via Chain of Responsibility..." << std::endl;
    ChosenHand result = handHandler.evaluate(hand);

    // 4. Results & Rewards
    if (result.isValid()) {
        int finalScore = ScoringRule::calculateScore(result.handName, result.baseScore);
        std::cout << ">> Result: " << result.handName << " (Final Score: " << finalScore << ")" << std::endl;
        
        int required = BlindRule::getRequiredScore(1);
        std::cout << "[System] Blind level 1 requires: " << required << std::endl;
        
        int reward = RewardRule::calculateReward(finalScore);
        std::cout << "[System] Rewards Earned: $" << reward << std::endl;
    } else {
        std::cout << ">> Result: No valid hand detected." << std::endl;
    }
    
    std::cout << "--- Session Complete ---" << std::endl;
}
