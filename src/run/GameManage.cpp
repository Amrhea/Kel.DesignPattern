#include "run/GameManage.h"
#include "hand_selection/Hand.h"
#include "poker_evaluation/HandHandler.h"
#include "scoring/ConcreteScoreCalculators.h"
#include <iostream>

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::GetInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

GameManager::GameManager() {
    handGenerator = new HandGenerator();
    // Default strategies
    scoringRule = std::make_unique<ScoringRule>(std::make_unique<BaseScoringRule>());
    blindRule = std::make_unique<BlindRule>(std::make_unique<SmallBlind>());
    rewardRule = std::make_unique<RewardRule>(std::make_unique<StandardReward>());
}

GameManager::~GameManager() {
    delete handGenerator;
}

void GameManager::RunSession() {
    HandHandler handHandler;
    std::cout << "--- Poker Game Session Started ---" << std::endl;

    // 1. Setup Player and Jokers
    HandPlayer player;
    player.AddJoker(std::make_shared<JokerCard>("Ice Joker", 15));
    player.AddJoker(std::make_shared<JokerCard>("Fire Joker", 30));

    // Register Player's Jokers as Observers
    for (auto& joker : player.GetJokers()) {
        RegisterObserver(joker.get());
    }

    // 2. Generate
    std::cout << "[System] Generating randomized hand..." << std::endl;
    Hand hand = handGenerator->generateHand();

    // 3. Play
    std::cout << "[System] Current Hand:" << std::endl;
    handHandler.ShowCards(hand);

    // 4. Evaluate using Template Method pattern
    std::cout << "[System] Evaluating hand via Template Method pattern..." << std::endl;
    BonusScoreCalculator calculator;
    int finalScore = calculator.CalculateScore(hand, handHandler);

    ChosenHand result = handHandler.evaluate(hand);

    // 5. Results, Observers notification & Rewards
    if (result.isValid()) {
        std::cout << ">> Checked Hand: " << result.handName << std::endl;
        std::cout << ">> Score before Jokers: " << finalScore << std::endl;

        // Notify observers to modify score
        NotifyObservers(result.handName, finalScore);
        std::cout << ">> Score after Jokers: " << finalScore << std::endl;
        
        // Strategy usage: Blind target
        int required = blindRule->getRequiredScore(1);
        std::cout << "[System] Blind level 1 requires: " << required << std::endl;
        
        bool isWin = finalScore >= required;
        std::cout << "[System] Win Condition Met? " << (isWin ? "YES" : "NO") << std::endl;

        // Strategy usage: Reward calculation
        int reward = rewardRule->calculateReward(finalScore);
        player.AddGold(reward);
        std::cout << "[System] Rewards Earned: $" << reward << std::endl;
        std::cout << "[System] Player Current Gold: $" << player.GetGold() << std::endl;
    } else {
        std::cout << ">> Result: No valid hand detected." << std::endl;
    }
    
    // Clean up observers for this session
    observers.clear();
    std::cout << "--- Session Complete ---" << std::endl;
}
