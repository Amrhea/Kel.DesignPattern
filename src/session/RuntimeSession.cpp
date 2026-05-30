#include "session/RuntimeSession.h"
#include "blind/BlindState.h"
#include "blind/SmallBlindState.h"
#include "reward/RewardCommand.h"
#include "card/Deck.h"

RuntimeSession::RuntimeSession() 
    : ante(1), remainingPlays(4), gold(4), currentBlind(std::make_shared<SmallBlindState>()) {
    
    // Initialize standard deck
    Deck stdDeck = DeckFactory::CreateStandardDeck();
    while (stdDeck.RemainingCount() > 0) {
        persistentDeck.push_back(stdDeck.Draw());
    }

    // Default Hand Scores
    handScores[PokerHandType::HighCard] = HandScoreData(5, 1);
    handScores[PokerHandType::Pair] = HandScoreData(10, 2);
    handScores[PokerHandType::TwoPair] = HandScoreData(20, 2);
    handScores[PokerHandType::ThreeOfKind] = HandScoreData(30, 3);
    handScores[PokerHandType::Straight] = HandScoreData(30, 4);
    handScores[PokerHandType::Flush] = HandScoreData(35, 4);
    handScores[PokerHandType::FullHouse] = HandScoreData(40, 4);
    handScores[PokerHandType::FourOfKind] = HandScoreData(60, 7);
    handScores[PokerHandType::StraightFlush] = HandScoreData(100, 8);
}

RuntimeSession::~RuntimeSession() {}

void RuntimeSession::addGold(int amount) {
    gold += amount;
}

void RuntimeSession::addJoker(std::shared_ptr<Observer> joker) {
    if (joker) {
        jokers.push_back(joker);
    }
}

void RuntimeSession::upgradeHand(PokerHandType handType, int chipsBonus, int multBonus) {
    auto it = handScores.find(handType);
    if (it != handScores.end()) {
        it->second.level++;
        it->second.baseChips += chipsBonus;
        it->second.baseMult += multBonus;
    }
}

std::vector<std::string> RuntimeSession::executePendingCommands(const std::string& timing) {
    std::vector<std::string> logs;
    std::vector<std::shared_ptr<RewardCommand>> remaining;
    for (auto& cmd : pendingCommands) {
        if (cmd && cmd->getTiming() == timing) {
            cmd->execute(*this);
            std::string logMsg = "→ " + cmd->getName() + " executed.";
            if (cmd->getName() == "Bonus Hand") {
                logMsg += " remainingPlays: " + std::to_string(remainingPlays);
            } else if (cmd->getName() == "Free Playing Card") {
                logMsg += " deck size: " + std::to_string(deck.size());
            }
            logs.push_back(logMsg);
        } else {
            remaining.push_back(cmd);
        }
    }
    pendingCommands = std::move(remaining);
    return logs;
}

std::vector<std::string> RuntimeSession::skipBlind() {
    std::vector<std::string> logs;
    if (!currentBlind) return logs;
    auto cmd = currentBlind->createSkipRewardCommand();
    if (cmd) {
        pendingCommands.push_back(cmd);
        logs.push_back("[Reward queued: " + cmd->getName() + " (timing: " + cmd->getTiming() + ")]");
    }
    int oldAnte = ante;
    logs.push_back("Advancing to next blind..."); // General advancement message
    currentBlind = currentBlind->nextState(ante);
    
    logs.push_back("[Executing NextBlind commands...]");
    auto nextBlindLogs = executePendingCommands("NextBlind");
    logs.insert(logs.end(), nextBlindLogs.begin(), nextBlindLogs.end());

    if (ante > oldAnte) {
        logs.push_back("[Executing NextAnte commands...]");
        auto nextAnteLogs = executePendingCommands("NextAnte");
        logs.insert(logs.end(), nextAnteLogs.begin(), nextAnteLogs.end());
    }
    return logs;
}

std::vector<std::string> RuntimeSession::playBlind() {
    std::vector<std::string> logs;
    if (!currentBlind) return logs;
    int oldAnte = ante;
    logs.push_back("Advancing to next blind..."); // General advancement message
    currentBlind = currentBlind->nextState(ante);

    logs.push_back("[Executing NextBlind commands...]");
    auto nextBlindLogs = executePendingCommands("NextBlind");
    logs.insert(logs.end(), nextBlindLogs.begin(), nextBlindLogs.end());

    if (ante > oldAnte) {
        logs.push_back("[Executing NextAnte commands...]");
        auto nextAnteLogs = executePendingCommands("NextAnte");
        logs.insert(logs.end(), nextAnteLogs.begin(), nextAnteLogs.end());
    }
    return logs;
}
