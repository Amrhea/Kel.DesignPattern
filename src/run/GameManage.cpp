#include "run/GameManage.h"
#include "hand_selection/Hand.h"
#include "poker_evaluation/HandHandler.h"
#include "scoring/ConcreteScoreCalculators.h"
#include "session/RuntimeSession.h"
#include "blind/BlindState.h"
#include "joker/JokerCard.h"
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
    std::cout << "--- Balatro CLI Run Session Started ---" << std::endl;

    // Initialize RuntimeSession Model
    RuntimeSession session;
    HandHandler handHandler;

    // Setup player, gold, jokers (as observers)
    HandPlayer player;
    player.AddJoker(std::make_shared<JokerCard>("Ice Joker", 15));
    player.AddJoker(std::make_shared<JokerCard>("Fire Joker", 30));
    for (auto& joker : player.GetJokers()) {
        RegisterObserver(joker.get());
    }

    // Run session loop (e.g. for Ante 1)
    while (session.ante == 1) {
        std::cout << "=== Ante " << session.ante << " ===" << std::endl;
        std::cout << "Current Blind: " << session.currentBlind->getName() << std::endl;
        int target = session.currentBlind->getTargetScore(session.ante);
        std::cout << "Target Score: " << target << std::endl;
        
        std::cout << "Action? (play/skip): ";
        std::string choice;
        if (!(std::cin >> choice)) {
            choice = "skip";
            std::cout << choice << std::endl;
        }

        if (choice == "skip") {
            auto logs = session.skipBlind();
            // Output logs cleanly
            for (const auto& log : logs) {
                if (log == "Advancing to next blind...") {
                    std::cout << "Advancing to " << session.currentBlind->getName() << "..." << std::endl;
                } else {
                    std::cout << log << std::endl;
                }
            }
        } else {
            // PLAY action logic
            std::cout << "[System] Generating randomized hand..." << std::endl;
            Hand hand = handGenerator->generateHand();
            std::cout << "[System] Current Hand:" << std::endl;
            handHandler.ShowCards(hand);

            std::cout << "[System] Evaluating hand via Template Method pattern..." << std::endl;
            BonusScoreCalculator calculator;
            int finalScore = calculator.CalculateScore(hand, handHandler);

            ChosenHand result = handHandler.evaluate(hand);

            if (result.isValid()) {
                std::cout << ">> Checked Hand: " << result.handName << std::endl;
                std::cout << ">> Score before Jokers: " << finalScore << std::endl;

                // Notify observers to modify score
                NotifyObservers(result.handName, finalScore);
                std::cout << ">> Score after Jokers: " << finalScore << std::endl;
                
                bool isWin = finalScore >= target;
                std::cout << "[System] Win Condition Met? " << (isWin ? "YES" : "NO") << std::endl;

                // Strategy usage: Reward calculation
                int rewardMoney = session.currentBlind->getRewardMoney();
                player.AddGold(rewardMoney);
                std::cout << "[System] Rewards Earned: $" << rewardMoney << std::endl;
                std::cout << "[System] Player Current Gold: $" << player.GetGold() << std::endl;
            } else {
                std::cout << ">> Result: No valid hand detected." << std::endl;
            }

            auto logs = session.playBlind();
            for (const auto& log : logs) {
                if (log == "Advancing to next blind...") {
                    std::cout << "Advancing to " << session.currentBlind->getName() << "..." << std::endl;
                } else {
                    std::cout << log << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }

    // Show Ante 2 entry
    std::cout << "=== Ante " << session.ante << " ===" << std::endl;
    std::cout << "Current Blind: " << session.currentBlind->getName() << std::endl;
    std::cout << "Target Score: " << session.currentBlind->getTargetScore(session.ante) << std::endl;

    // Clean up observers
    observers.clear();
    std::cout << "--- Session Complete ---" << std::endl;
}
