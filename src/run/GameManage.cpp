#include "run/GameManage.h"
#include "joker/ConcreteJokers.h"
#include "joker/JokerManager.h"
#include "reward/ShopSystem.h"
#include "blind/BlindManager.h"
#include <iostream>
#include <string>

GameManager* GameManager::instance = nullptr;

GameManager* GameManager::GetInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

GameManager::GameManager() : state(RunState::MainMenu) {}

GameManager::~GameManager() {}

void GameManager::RunSession() {
    state = RunState::MainMenu;
    bool running = true;
    while (running) {
        switch (state) {
            case RunState::MainMenu:
                HandleMainMenu();
                break;
            case RunState::StartRun:
                HandleStartRun();
                break;
            case RunState::SelectBlind:
                HandleSelectBlind();
                break;
            case RunState::PlayHand:
                HandlePlayHand();
                break;
            case RunState::ResolveScore:
                HandleResolveScore();
                break;
            case RunState::Reward:
                HandleReward();
                break;
            case RunState::Shop:
                HandleShop();
                break;
            case RunState::GameOver:
                HandleGameOver();
                break;
            default:
                running = false;
                break;
        }
    }
}

void GameManager::HandleMainMenu() {
    std::cout << "\n=========================================" << std::endl;
    std::cout << "===          BALATRO CLI GAME         ===" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "1. Start Run" << std::endl;
    std::cout << "2. Exit" << std::endl;
    std::cout << "Enter choice: ";
    
    std::string choice;
    if (!std::getline(std::cin, choice)) {
        exit(0);
    }

    choice.erase(0, choice.find_first_not_of(" \t\r\n"));
    choice.erase(choice.find_last_not_of(" \t\r\n") + 1);

    if (choice == "1" || choice == "Start" || choice == "Start Run") {
        state = RunState::StartRun;
    } else if (choice == "2" || choice == "Exit") {
        std::cout << "Goodbye!" << std::endl;
        exit(0);
    } else {
        std::cout << "Invalid choice. Please select 1 or 2." << std::endl;
    }
}

void GameManager::HandleStartRun() {
    std::cout << "\nStarting a new Balatro Run..." << std::endl;
    session = std::make_unique<RuntimeSession>();
    anteManager = std::make_unique<AnteManager>(*session);
    roundManager = std::make_unique<RoundManager>();

    // Initial starting jokers
    session->addJoker(std::make_shared<ChipsBoostJoker>(20));
    session->addJoker(std::make_shared<MultiplierJoker>(2));
    session->addJoker(std::make_shared<ConditionalJoker>(PokerHandType::Flush, 15));

    // Register starting jokers
    JokerManager& jokerManager = JokerManager::GetInstance();
    jokerManager.ClearObservers();
    for (auto& joker : session->jokers) {
        jokerManager.RegisterObserver(joker.get());
    }

    state = RunState::SelectBlind;
}

void GameManager::HandleSelectBlind() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "=== ANTE " << anteManager->GetAnte() << " - " << anteManager->GetBlindName() << " ===" << std::endl;
    std::cout << "Target Score: " << anteManager->GetTargetScore() << std::endl;
    std::cout << "Reward Money: $" << anteManager->GetRewardMoney() << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Action?\n(1) Play Blind\n(2) Skip Blind\n(3) View Deck/Stats\nEnter choice: ";

    std::string choiceStr;
    if (!std::getline(std::cin, choiceStr)) {
        state = RunState::GameOver;
        return;
    }

    choiceStr.erase(0, choiceStr.find_first_not_of(" \t\r\n"));
    choiceStr.erase(choiceStr.find_last_not_of(" \t\r\n") + 1);

    if (choiceStr == "1" || choiceStr == "Play" || choiceStr == "Play Blind") {
        state = RunState::PlayHand;
    } else if (choiceStr == "2" || choiceStr == "Skip" || choiceStr == "Skip Blind") {
        std::cout << "Skipping blind..." << std::endl;
        auto logs = anteManager->Skip();
        for (const auto& log : logs) {
            std::cout << log << std::endl;
        }
        state = RunState::SelectBlind;
    } else if (choiceStr == "3" || choiceStr == "View Deck" || choiceStr == "View Deck/Stats") {
        std::cout << "\n--- Player Stats & Deck ---" << std::endl;
        std::cout << "Gold: $" << session->gold << std::endl;
        std::cout << "Active Jokers (" << session->jokers.size() << "):" << std::endl;
        for (auto& observer : session->jokers) {
            if (dynamic_cast<ChipsBoostJoker*>(observer.get())) {
                std::cout << "- ChipsBoostJoker" << std::endl;
            } else if (dynamic_cast<MultiplierJoker*>(observer.get())) {
                std::cout << "- MultiplierJoker" << std::endl;
            } else if (dynamic_cast<ConditionalJoker*>(observer.get())) {
                std::cout << "- ConditionalJoker" << std::endl;
            } else {
                std::cout << "- Custom/Unknown Joker Observer" << std::endl;
            }
        }
        std::cout << "Deck Size: " << session->persistentDeck.size() << " cards" << std::endl;
    } else {
        std::cout << "Invalid choice. Please select 1, 2, or 3." << std::endl;
    }
}

void GameManager::HandlePlayHand() {
    // Convert current blind state to BlindData
    std::string name = anteManager->GetBlindName();
    int target = anteManager->GetTargetScore();
    int reward = anteManager->GetRewardMoney();
    
    BlindType type = BlindType::SMALL;
    if (name == "Big Blind") type = BlindType::BIG;
    else if (name == "Boss Blind") type = BlindType::BOSS;

    // Standard hand/discard limits: 4/3
    BlindData blindData(type, name, target, session->remainingPlays, 3, reward);

    bool won = roundManager->RunRound(*session, blindData);
    if (won) {
        state = RunState::ResolveScore;
    } else {
        state = RunState::GameOver;
    }
}

void GameManager::HandleResolveScore() {
    std::cout << "\nResolving score... Target met!" << std::endl;
    state = RunState::Reward;
}

void GameManager::HandleReward() {
    int rewardMoney = anteManager->GetRewardMoney();
    session->addGold(rewardMoney);
    std::cout << "[Reward] Earned $" << rewardMoney << " for completing the blind." << std::endl;
    std::cout << "[Reward] Total Gold: $" << session->gold << std::endl;
    state = RunState::Shop;
}

void GameManager::HandleShop() {
    std::cout << "\n--- Entering Shop ---" << std::endl;
    ShopSystem shop;
    shop.GenerateInventory();
    
    while (true) {
        std::cout << "\nPlayer Gold: $" << session->gold << std::endl;
        const auto& inventory = shop.GetInventory();
        if (inventory.empty()) {
            std::cout << "The shop is empty!" << std::endl;
        } else {
            for (size_t i = 0; i < inventory.size(); ++i) {
                std::cout << "[" << i << "] " << inventory[i].description << " ($" << inventory[i].price << ")" << std::endl;
            }
        }
        std::cout << "Enter item index to buy (or -1 to leave shop): ";
        std::string shopChoiceStr;
        if (!std::getline(std::cin, shopChoiceStr)) {
            break;
        }

        shopChoiceStr.erase(0, shopChoiceStr.find_first_not_of(" \t\r\n"));
        shopChoiceStr.erase(shopChoiceStr.find_last_not_of(" \t\r\n") + 1);

        int shopChoice = -1;
        try {
            if (!shopChoiceStr.empty()) {
                shopChoice = std::stoi(shopChoiceStr);
            }
        } catch (...) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        if (shopChoice == -1) {
            std::cout << "Leaving shop." << std::endl;
            break;
        }

        if (shopChoice < 0 || shopChoice >= static_cast<int>(inventory.size())) {
            std::cout << "Invalid index. Try again." << std::endl;
            continue;
        }

        if (shop.BuyItem(shopChoice, *session)) {
            std::cout << "[System] Item purchased!" << std::endl;
            
            // Re-register observer list
            JokerManager& jokerManager = JokerManager::GetInstance();
            jokerManager.ClearObservers();
            for (auto& joker : session->jokers) {
                jokerManager.RegisterObserver(joker.get());
            }
        } else {
            std::cout << "[System] Not enough gold or invalid item." << std::endl;
        }
    }

    std::cout << "\nAdvancing to next blind..." << std::endl;
    auto logs = anteManager->Advance();
    for (const auto& log : logs) {
        std::cout << log << std::endl;
    }

    state = RunState::SelectBlind;
}

void GameManager::HandleGameOver() {
    std::cout << "\n=========================================" << std::endl;
    std::cout << "===             GAME OVER             ===" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Returning to Main Menu..." << std::endl;
    
    session.reset();
    anteManager.reset();
    roundManager.reset();

    state = RunState::MainMenu;
}
