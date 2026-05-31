#include "session/RuntimeSession.h"
#include "run/AnteManager.h"
#include "blind/BlindState.h"
#include "card/Card.h"
#include "card/Deck.h"
#include "hand_selection/Hand.h"
#include "hand_selection/HandServices.h"
#include "poker_evaluation/PokerHandEvaluator.h"
#include "joker/JokerManager.h"
#include "scoring/ScoreContext.h"
#include "scoring/ConcreteScoreCalculators.h"
#include "reward/ShopSystem.h"
#include "joker/ConcreteJokers.h"
#include <emscripten/bind.h>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

class WasmGame {
private:
    PokerHandType parseHandType(const std::string& name) const {
        if (name == "High Card") return PokerHandType::HighCard;
        if (name == "Pair") return PokerHandType::Pair;
        if (name == "Two Pair") return PokerHandType::TwoPair;
        if (name == "Three of a Kind") return PokerHandType::ThreeOfKind;
        if (name == "Straight") return PokerHandType::Straight;
        if (name == "Flush") return PokerHandType::Flush;
        if (name == "Full House") return PokerHandType::FullHouse;
        if (name == "Four of a Kind") return PokerHandType::FourOfKind;
        if (name == "Straight Flush") return PokerHandType::StraightFlush;
        if (name == "Royal Flush") return PokerHandType::RoyalFlush;
        return PokerHandType::HighCard;
    }

public:
    std::shared_ptr<RuntimeSession> session;
    std::shared_ptr<AnteManager> anteManager;
    std::shared_ptr<ScoringRule> scoringRule;
    ShopSystem shop;
    
    // Round runtime state
    bool inRound;
    int roundScore;
    int roundTargetScore;
    int roundHandsRemaining;
    int roundDiscardsRemaining;
    std::vector<Card> currentHand;
    Deck roundDeck;
    
    std::vector<std::string> logs;

    WasmGame() {
        inRound = false;
        roundScore = 0;
        roundTargetScore = 0;
        roundHandsRemaining = 0;
        roundDiscardsRemaining = 0;
        scoringRule = std::make_unique<ScoringRule>(std::make_unique<BaseScoringRule>());
        startNewRun();
    }

    void startNewRun() {
        session = std::make_shared<RuntimeSession>();
        anteManager = std::make_shared<AnteManager>(*session);
        
        // Initial jokers
        session->addJoker(std::make_shared<ChipsBoostJoker>(20));
        session->addJoker(std::make_shared<MultiplierJoker>(2));
        session->addJoker(std::make_shared<ConditionalJoker>(PokerHandType::Flush, 15));

        // Register starting jokers
        JokerManager& jokerManager = JokerManager::GetInstance();
        jokerManager.ClearObservers();
        for (auto& joker : session->jokers) {
            jokerManager.RegisterObserver(joker.get());
        }
        
        inRound = false;
        logs.clear();
        addLog("New run started! Ante 1.");
    }

    void addLog(const std::string& msg) {
        logs.push_back(msg);
        if (logs.size() > 50) {
            logs.erase(logs.begin());
        }
    }

    // Getters for general session state
    int getAnte() const { return session->ante; }
    int getGold() const { return session->gold; }
    int getDeckSize() const { return session->persistentDeck.size(); }
    bool getInRound() const { return inRound; }
    int getRoundScore() const { return roundScore; }
    int getRoundTargetScore() const { return roundTargetScore; }
    int getRoundHandsRemaining() const { return roundHandsRemaining; }
    int getRoundDiscardsRemaining() const { return roundDiscardsRemaining; }
    
    // Blind details
    std::string getBlindName() const { return anteManager->GetBlindName(); }
    int getBlindTargetScore() const { return anteManager->GetTargetScore(); }
    int getBlindRewardMoney() const { return anteManager->GetRewardMoney(); }

    // Hand score live state
    int getHandLevel(std::string name) const {
        auto it = session->handScores.find(parseHandType(name));
        if (it != session->handScores.end()) return it->second.level;
        return 1;
    }

    int getHandBaseChips(std::string name) const {
        auto it = session->handScores.find(parseHandType(name));
        if (it != session->handScores.end()) return it->second.baseChips;
        return 0;
    }

    int getHandBaseMult(std::string name) const {
        auto it = session->handScores.find(parseHandType(name));
        if (it != session->handScores.end()) return it->second.baseMult;
        return 0;
    }

    // Actions
    void skipBlind() {
        if (inRound) return;
        addLog("Skipping blind: " + getBlindName());
        auto skipLogs = anteManager->Skip();
        for (const auto& l : skipLogs) {
            addLog(l);
        }
    }

    void startRound() {
        if (inRound) return;
        inRound = true;
        roundScore = 0;
        roundTargetScore = getBlindTargetScore();
        roundHandsRemaining = 4;
        roundDiscardsRemaining = 3;
        
        // Setup round deck
        roundDeck = Deck();
        for (const auto& card : session->persistentDeck) {
            roundDeck.AddCard(card);
        }
        roundDeck.Shuffle();
        
        // Draw initial 8 cards
        currentHand.clear();
        refillHand();
        addLog("Round started! Target score: " + std::to_string(roundTargetScore));
    }

    void refillHand() {
        int cardsNeeded = 8 - currentHand.size();
        for (int i = 0; i < cardsNeeded; ++i) {
            if (roundDeck.RemainingCount() > 0) {
                currentHand.push_back(roundDeck.Draw());
            }
        }
    }

    std::vector<Card> getHand() const {
        return currentHand;
    }

    // Play cards (indices refers to positions in currentHand)
    std::string playHand(std::vector<int> indices) {
        if (!inRound) return "Not in round";
        if (indices.empty()) return "No cards selected";
        
        // Construct Hand
        Hand playedHand;
        for (int idx : indices) {
            if (idx >= 0 && idx < (int)currentHand.size()) {
                playedHand.AddCard(currentHand[idx]);
            }
        }
        
        PokerHandEvaluator handEvaluator;
        HandEvaluation eval = handEvaluator.Evaluate(playedHand);
        if (!eval.isValid()) {
            return "Not a valid poker hand";
        }
        
        JokerManager& jokerManager = JokerManager::GetInstance();
        jokerManager.ClearObservers();
        for (auto& joker : session->jokers) {
            jokerManager.RegisterObserver(joker.get());
        }
        
        PlayedHandResult baseResult = scoringRule->calculateScore(eval.handType, session->handScores);
        ScoreContext context(eval.handType, eval.handName, baseResult.chips, baseResult.mult, playedHand.GetCards());
        
        // Apply jokers
        jokerManager.NotifyObservers(context);
        int finalScore = context.calculateFinalScore();
        
        roundScore += finalScore;
        roundHandsRemaining--;
        
        // Remove played cards from hand
        std::vector<Card> newHand;
        for (int i = 0; i < (int)currentHand.size(); ++i) {
            if (std::find(indices.begin(), indices.end(), i) == indices.end()) {
                newHand.push_back(currentHand[i]);
            }
        }
        currentHand = newHand;
        refillHand();
        
        std::string resLog = "Played: " + context.handName + " (Level " + std::to_string(baseResult.level) + "). Scored: " + std::to_string(finalScore);
        addLog(resLog);
        
        // Check win/lose
        if (roundScore >= roundTargetScore) {
            inRound = false;
            // Won! Reward money
            int reward = getBlindRewardMoney();
            session->addGold(reward);
            addLog("Round won! Gained $" + std::to_string(reward));
            // Advance to next blind
            auto advanceLogs = anteManager->Skip();
            for (const auto& l : advanceLogs) {
                addLog(l);
            }
            shop.GenerateInventory(); // Generate shop items
        } else if (roundHandsRemaining <= 0) {
            inRound = false;
            addLog("Game Over! Failed to reach target score.");
        }
        
        return resLog;
    }

    void discardCards(std::vector<int> indices) {
        if (!inRound) return;
        if (roundDiscardsRemaining <= 0) {
            addLog("No discards remaining!");
            return;
        }
        if (indices.empty()) return;
        
        roundDiscardsRemaining--;
        
        std::vector<Card> newHand;
        for (int i = 0; i < (int)currentHand.size(); ++i) {
            if (std::find(indices.begin(), indices.end(), i) == indices.end()) {
                newHand.push_back(currentHand[i]);
            }
        }
        currentHand = newHand;
        refillHand();
        addLog("Discarded " + std::to_string(indices.size()) + " cards.");
    }

    // Shop operations
    void generateShop() {
        shop.GenerateInventory();
    }

    int getShopItemCount() const {
        return shop.GetInventory().size();
    }

    std::string getShopItemDescription(int idx) const {
        const auto& inv = shop.GetInventory();
        if (idx >= 0 && idx < (int)inv.size()) {
            return inv[idx].description + " (Price: $" + std::to_string(inv[idx].price) + ")";
        }
        return "";
    }
    
    int getShopItemPrice(int idx) const {
        const auto& inv = shop.GetInventory();
        if (idx >= 0 && idx < (int)inv.size()) {
            return inv[idx].price;
        }
        return 999;
    }

    bool buyShopItem(int idx) {
        bool success = shop.BuyItem(idx, *session);
        if (success) {
            addLog("Bought shop item!");
        } else {
            addLog("Failed to buy shop item.");
        }
        return success;
    }

    // Logs management
    int getLogCount() const { return logs.size(); }
    std::string getLog(int idx) const {
        if (idx >= 0 && idx < (int)logs.size()) return logs[idx];
        return "";
    }
    void clearLogs() { logs.clear(); }
};

EMSCRIPTEN_BINDINGS(wasm_game) {
    emscripten::register_vector<int>("IntVector");
    emscripten::register_vector<std::string>("StringVector");
    emscripten::register_vector<Card>("CardVector");

    emscripten::value_object<Card>("Card")
        .field("id", &Card::id)
        .field("rank", &Card::rank)
        .field("suit", &Card::suit);

    emscripten::class_<WasmGame>("WasmGame")
        .constructor<>()
        .method("startNewRun", &WasmGame::startNewRun)
        .method("getAnte", &WasmGame::getAnte)
        .method("getGold", &WasmGame::getGold)
        .method("getDeckSize", &WasmGame::getDeckSize)
        .method("getInRound", &WasmGame::getInRound)
        .method("getRoundScore", &WasmGame::getRoundScore)
        .method("getRoundTargetScore", &WasmGame::getRoundTargetScore)
        .method("getRoundHandsRemaining", &WasmGame::getRoundHandsRemaining)
        .method("getRoundDiscardsRemaining", &WasmGame::getRoundDiscardsRemaining)
        .method("getBlindName", &WasmGame::getBlindName)
        .method("getBlindTargetScore", &WasmGame::getBlindTargetScore)
        .method("getBlindRewardMoney", &WasmGame::getBlindRewardMoney)
        .method("getHandLevel", &WasmGame::getHandLevel)
        .method("getHandBaseChips", &WasmGame::getHandBaseChips)
        .method("getHandBaseMult", &WasmGame::getHandBaseMult)
        .method("skipBlind", &WasmGame::skipBlind)
        .method("startRound", &WasmGame::startRound)
        .method("getHand", &WasmGame::getHand)
        .method("playHand", &WasmGame::playHand)
        .method("discardCards", &WasmGame::discardCards)
        .method("generateShop", &WasmGame::generateShop)
        .method("getShopItemCount", &WasmGame::getShopItemCount)
        .method("getShopItemDescription", &WasmGame::getShopItemDescription)
        .method("getShopItemPrice", &WasmGame::getShopItemPrice)
        .method("buyShopItem", &WasmGame::buyShopItem)
        .method("getLogCount", &WasmGame::getLogCount)
        .method("getLog", &WasmGame::getLog)
        .method("clearLogs", &WasmGame::clearLogs);
}
