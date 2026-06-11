#include "run/RoundManager.h"
#include "card/Card.h"
#include "card/Deck.h"
#include "hand_selection/Hand.h"
#include "hand_selection/HandServices.h"
#include "poker_evaluation/PokerHandEvaluator.h"
#include "joker/JokerManager.h"
#include "scoring/ScoreContext.h"
#include "scoring/ConcreteScoreCalculators.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RoundManager::RoundManager() {
    scoringRule = std::make_unique<ScoringRule>(std::make_unique<BaseScoringRule>());
}

RoundManager::~RoundManager() {}

// Round == Blind
bool RoundManager::RunRound(RuntimeSession& session, const BlindData& blindData) {
    std::cout << "\n=== ROUND START: " << blindData.name << " ===" << std::endl;
    std::cout << "Target Score: " << blindData.targetScore << std::endl;

    // 1. Initialize round deck from persistent deck and shuffle
    Deck deck;
    for (const auto& card : session.persistentDeck) {
        deck.AddCard(card);
    }
    deck.Shuffle();

    // 2. Initialize RoundState and hand
    RoundState round(blindData, session.remainingPlays);
    Hand hand;
    std::vector<Card> discardPile;

    // 3. Draw initial hand
    DrawService::DrawToHand(deck, hand, 8);

    PokerHandEvaluator handEvaluator;
    JokerManager& jokerManager = JokerManager::GetInstance();

    while (round.GetStatus() == RoundStatus::PLAYING) {
        std::cout << "\n----------------------------------------" << std::endl;
        std::cout << "Current Score: " << round.GetCurrentScore() << " / " << round.GetTargetScore() << std::endl;
        std::cout << "Hands Remaining: " << round.GetHandsRemaining() << " | Discards Remaining: " << round.GetDiscardsRemaining() << std::endl;
        std::cout << "\nYour Hand:" << std::endl;
        for (int i = 0; i < hand.GetCardCount(); ++i) {
            Card c = hand.GetCard(i);
            std::cout << "[" << i << "] Rank " << c.rank << ", Suit " << c.suit << std::endl;
        }

        std::cout << "\nSelect action:\n(1) Play Hand\n(2) Discard\n(3) Give Up\nEnter choice: ";
        std::string choiceStr;
        if (!std::getline(std::cin, choiceStr)) {
            std::cout << "\nEOF detected. Giving up." << std::endl;
            return false;
        }

        // Trim input
        choiceStr.erase(0, choiceStr.find_first_not_of(" \t\r\n"));
        choiceStr.erase(choiceStr.find_last_not_of(" \t\r\n") + 1);

        if (choiceStr == "3" || choiceStr == "Give Up") {
            std::cout << "Giving up the round." << std::endl;
            return false;
        }

        if (choiceStr == "1" || choiceStr == "Play Hand" || choiceStr == "2" || choiceStr == "Discard") {
            std::cout << "Enter card indices to select (space-separated, e.g., 0 2 3): ";
            std::string indicesStr;
            if (!std::getline(std::cin, indicesStr)) {
                return false;
            }

            std::stringstream ss(indicesStr);
            std::vector<int> selectedIndices;
            int idx;
            bool parseError = false;
            while (ss >> idx) {
                if (idx < 0 || idx >= hand.GetCardCount()) {
                    parseError = true;
                }
                selectedIndices.push_back(idx);
            }

            if (selectedIndices.empty() || parseError) {
                std::cout << "Invalid selection. Please try again." << std::endl;
                continue;
            }

            // Verify unique indices
            std::vector<int> sortedIndices = selectedIndices;
            std::sort(sortedIndices.begin(), sortedIndices.end());
            auto uniqueIt = std::unique(sortedIndices.begin(), sortedIndices.end());
            if (uniqueIt != sortedIndices.end()) {
                std::cout << "Duplicate indices selected. Please try again." << std::endl;
                continue;
            }

            if (choiceStr == "1" || choiceStr == "Play Hand") {
                // Play logic
                Hand playedHand;
                for (int index : selectedIndices) {
                    playedHand.AddCard(hand.GetCard(index));
                }

                HandEvaluation eval = handEvaluator.Evaluate(playedHand);
                if (!eval.isValid()) {
                    std::cout << "Not a valid poker hand. Please try again." << std::endl;
                    continue;
                }

                // Register active jokers
                jokerManager.ClearObservers();
                for (auto& joker : session.jokers) {
                    jokerManager.RegisterObserver(joker.get());
                }

                // Calculate base score
                PlayedHandResult baseResult = scoringRule->calculateScore(eval.handType, session.handScores);
                ScoreContext context(eval.handType, eval.handName, baseResult.chips, baseResult.mult, playedHand.GetCards());

                std::cout << "\n>> Played Hand: " << context.handName << " (Level " << baseResult.level << ")" << std::endl;
                std::cout << ">> Base Score: " << context.chips << " x " << context.mult << " = " << context.calculateFinalScore() << std::endl;

                // Apply jokers
                jokerManager.NotifyObservers(context);
                int finalScore = context.calculateFinalScore();
                std::cout << ">> Final Score after Jokers: " << context.chips << " x " << context.mult << " = " << finalScore << std::endl;

                // Commit action to RoundState
                round.UseHand();
                round.AddScore(finalScore);

                // Remove from hand and refill
                std::vector<Card> dummyDiscard;
                DiscardService::Discard(hand, selectedIndices, dummyDiscard);
                DrawService::DrawToHand(deck, hand, 8 - hand.GetCardCount());

            } else {
                // Discard logic
                if (round.GetDiscardsRemaining() <= 0) {
                    std::cout << "No discards remaining!" << std::endl;
                    continue;
                }

                round.UseDiscard();
                DiscardService::Discard(hand, selectedIndices, discardPile);
                DrawService::DrawToHand(deck, hand, 8 - hand.GetCardCount());
                std::cout << "Cards discarded." << std::endl;
            }
        } else {
            std::cout << "Invalid choice. Please select 1, 2, or 3." << std::endl;
        }
    }

    if (round.GetStatus() == RoundStatus::WON) {
        std::cout << "\n=== ROUND WON! ===" << std::endl;
        return true;
    } else {
        std::cout << "\n=== ROUND LOST... Game Over ===" << std::endl;
        return false;
    }
}
