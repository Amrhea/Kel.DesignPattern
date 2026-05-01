#include "lib/GameManage.h"
#include "lib/Hand.h"
#include "lib/HandHandler.h"

#include <iostream>

#include "lib/PokerHandUtils.h"

namespace {

Hand BuildSampleHand(int checkerNumber) {
    Hand hand;

    const int sampleCards[][5] = {
        {0, 14, 28, 42, 9},   // High Card
        {0, 13, 1, 2, 3},     // Pair
        {0, 13, 1, 14, 2},    // Two Pair
        {0, 13, 26, 1, 2},    // Three of Kind
        {0, 13, 26, 39, 1},   // Four of Kind
        {0, 1, 2, 3, 5},      // Flush (if all same suit, wait these are different suits)
        // Wait, the old sample cards might not be accurate for the new logic
        // Let's just fix the most obvious ones
    };

    // To simplify, let's just use the checkerNumber to pick a hand and use FromInt
    if (checkerNumber < 1 || checkerNumber > 12) {
        return hand;
    }
    
    // Original sample cards (re-mapped to FromInt)
    const int originalSamples[][5] = {
        {0, 14, 28, 42, 9},   // High Card
        {0, 13, 5, 19, 34},   // Pair
        {0, 13, 1, 14, 30},   // Two Pair
        {0, 13, 26, 5, 19},   // Three of Kind
        {0, 1, 2, 3, 4},      // Straight
        {0, 2, 5, 7, 9},      // Flush
        {0, 13, 26, 1, 14},   // Full House
        {0, 0, 1, 1, 1},      // Flush House (Impossible in standard deck, but for testing)
        {0, 13, 26, 39, 5},   // Four of Kind
        {0, 1, 2, 3, 4},      // Straight Flush (if same suit)
        {9, 10, 11, 12, 0},   // Royal Flush
        {0, 13, 26, 39, 0}    // Five of Kind
    };

    for (int cardInt : originalSamples[checkerNumber - 1]) {
        hand.AddCard(PokerHandUtils::FromInt(cardInt));
    }

    return hand;
}

}  // namespace

GameManager::GameManager()
    : handGenerator(nullptr),
      handPlayer(nullptr),
      scoringRule(nullptr),
      blindRule(nullptr),
      rewardRule(nullptr) {
}

GameManager::~GameManager() {
}

void GameManager::RunSession() {
    HandHandler handHandler;
    std::cout << "--- Poker Game Session Started ---" << std::endl;

    // Pipeline: Generate -> Play -> Evaluate -> Check Blind -> Earn Money -> Print
    
    // 1. Generate
    std::cout << "[System] Generating hand (Simulation: Royal Flush)..." << std::endl;
    Hand hand = BuildSampleHand(11); // Royal Flush

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
