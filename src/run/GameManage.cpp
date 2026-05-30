#include "run/GameManage.h"
#include "hand_selection/Hand.h"
#include "poker_evaluation/PokerHandEvaluator.h"
#include "poker_evaluation/PokerHandUtils.h"
#include "scoring/ConcreteScoreCalculators.h"
#include "session/RuntimeSession.h"
#include "blind/BlindState.h"
#include "joker/JokerCard.h"
#include "joker/JokerManager.h"
#include "joker/ConcreteJokers.h"
#include "reward/RewardManager.h"
#include "reward/ShopSystem.h"
#include "scoring/ScoreContext.h"
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
    PokerHandEvaluator handEvaluator;

    // Setup Jokers in session and register to JokerManager
    JokerManager& jokerManager = JokerManager::GetInstance();
    jokerManager.ClearObservers();
    
    session.addJoker(std::make_shared<ChipsBoostJoker>(20));
    session.addJoker(std::make_shared<MultiplierJoker>(2));
    session.addJoker(std::make_shared<ConditionalJoker>(PokerHandType::Flush, 15));

    for (auto& joker : session.jokers) {
        jokerManager.RegisterObserver(joker.get());
    }

    ShopSystem shop;

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
            PokerHandUtils::ShowCards(hand);

            HandEvaluation eval = handEvaluator.Evaluate(hand);

            if (eval.isValid()) {
                // Get base score from ScoringRule (PlayedHandResult)
                PlayedHandResult baseResult = scoringRule->calculateScore(eval.handType, session.handScores);

                // Create ScoreContext (Mutable)
                ScoreContext context(eval.handType, eval.handName, baseResult.chips, baseResult.mult, hand.GetCards());

                std::cout << ">> Checked Hand: " << context.handName << " (Level " << baseResult.level << ")" << std::endl;
                std::cout << ">> Base Score: " << context.chips << " x " << context.mult << " = " << context.calculateFinalScore() << std::endl;

                // Notify JokerManager to apply Joker effects
                jokerManager.NotifyObservers(context);
                
                int finalScore = context.calculateFinalScore();
                std::cout << ">> Final Score after Jokers: " << context.chips << " x " << context.mult << " = " << finalScore << std::endl;
                
                bool isWin = finalScore >= target;
                std::cout << "[System] Win Condition Met? " << (isWin ? "YES" : "NO") << std::endl;

                // Strategy usage: Reward calculation
                int rewardMoney = session.currentBlind->getRewardMoney();
                session.addGold(rewardMoney);
                std::cout << "[System] Rewards Earned: $" << rewardMoney << std::endl;
                std::cout << "[System] Player Current Gold: $" << session.gold << std::endl;

                // Open Shop after winning
                std::cout << "--- Entering Shop ---" << std::endl;
                shop.GenerateInventory();
                const auto& inventory = shop.GetInventory();
                for (size_t i = 0; i < inventory.size(); ++i) {
                    std::cout << i << ": " << inventory[i].description << " ($" << inventory[i].price << ")" << std::endl;
                }
                std::cout << "Enter item index to buy (or -1 to leave): ";
                int shopChoice;
                if (std::cin >> shopChoice && shopChoice >= 0) {
                    if (shop.BuyItem(shopChoice, session)) {
                        std::cout << "[System] Item purchased!" << std::endl;
                        // Register new joker as observer if it was a joker reward
                        for (auto& joker : session.jokers) {
                            jokerManager.RegisterObserver(joker.get());
                        }
                    } else {
                        std::cout << "[System] Not enough gold or invalid item." << std::endl;
                    }
                }
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

    // Clean up or reset session if needed
    jokerManager.ClearObservers();
    std::cout << "--- Session Complete ---" << std::endl;
}
