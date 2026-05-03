#include "lib/GameManage.h"
#include "lib/Hand.h"
#include "lib/HandHandler.h"

#include <iostream>

#include "lib/PokerHandUtils.h"

GameManager::GameManager()
    : handGenerator(new HandGenerator()),
      handPlayer(nullptr),
      scoringRule(nullptr),
      blindRule(nullptr),
      rewardRule(nullptr) {
}

GameManager::~GameManager() {
    delete handGenerator;
}

void GameManager::RunSession() {
    HandHandler handHandler;
    std::cout << "--- Poker Game Session Started ---" << std::endl;

    // Pipeline: Generate -> Play -> Evaluate -> Check Blind -> Earn Money -> Print
    
    // 1. Generate
    std::cout << "[System] Generating randomized hand..." << std::endl;
    Hand hand = handGenerator->generateHand();

    // 2. Play
    std::cout << "[System] Current Hand:" << std::endl;
    handHandler.ShowCards(hand);

    // 3. Evaluate
    std::cout << "[System] Evaluating hand via Chain of Responsibility..." << std::endl;
    ChosenHand result = handHandler.evaluate(hand);

    // 4. Results & Rewards
    if (result.isValid()) {
        std::cout << ">> Result: " << result.handName << " (Base Score: " << result.baseScore << ")" << std::endl;
    } else {
        std::cout << ">> Result: No valid hand detected." << std::endl;
    }

    std::cout << "[System] Checking Blind rules... (Placeholder)" << std::endl;
    std::cout << "[System] Calculating Rewards... (Placeholder)" << std::endl;
    
    std::cout << "--- Session Complete ---" << std::endl;
}
