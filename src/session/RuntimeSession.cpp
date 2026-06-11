#include "session/RuntimeSession.h"
#include "blind/BlindState.h"
#include "blind/SmallBlindState.h"
#include "reward/RewardCommand.h"
#include "card/Deck.h"
#include "tag/TagFactory.h"

RuntimeSession::RuntimeSession() 
    : sessionState(),
      ante(sessionState.persistentState.ante),
      remainingPlays(sessionState.runtimeState.remainingHands),
      gold(sessionState.persistentState.money),
      currentBlind(sessionState.persistentState.currentBlind),
      pendingCommands(sessionState.persistentState.pendingCommands),
      tagStack(sessionState.persistentState.tagStack),
      jokers(sessionState.persistentState.jokers) {
    
    currentBlind = std::make_shared<SmallBlindState>();
    
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
    
    // Obtain Tag based on current blind skipped
    std::shared_ptr<Tag> tag;
    if (currentBlind->getName() == "Small Blind") {
        tag = TagFactory::CreateTag(TagType::HANDY);
    } else if (currentBlind->getName() == "Big Blind") {
        tag = TagFactory::CreateTag(TagType::ECONOMY);
    } else {
        tag = TagFactory::CreateTag(TagType::ORBITAL);
    }
    
    if (tag) {
        tagStack.push_back(tag);
        logs.push_back("[Tag obtained: " + tag->getName() + " (" + tag->getDescription() + ")]");
    }
    
    int oldAnte = ante;
    logs.push_back("Advancing to next blind..."); // General advancement message
    currentBlind = currentBlind->nextState(ante);
    
    logs.push_back("[Executing NextBlind commands...]");
    auto nextBlindLogs = executePendingCommands("NextBlind");
    logs.insert(logs.end(), nextBlindLogs.begin(), nextBlindLogs.end());
    
    // Trigger NEXT_BLIND tags
    auto tagLogs = triggerTags(TagTrigger::NEXT_BLIND);
    logs.insert(logs.end(), tagLogs.begin(), tagLogs.end());

    if (ante > oldAnte) {
        logs.push_back("[Executing NextAnte commands...]");
        auto nextAnteLogs = executePendingCommands("NextAnte");
        logs.insert(logs.end(), nextAnteLogs.begin(), nextAnteLogs.end());
        
        // Trigger NEXT_ANTE tags
        auto anteTagLogs = triggerTags(TagTrigger::NEXT_ANTE);
        logs.insert(logs.end(), anteTagLogs.begin(), anteTagLogs.end());
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
    
    // Trigger NEXT_BLIND tags
    auto tagLogs = triggerTags(TagTrigger::NEXT_BLIND);
    logs.insert(logs.end(), tagLogs.begin(), tagLogs.end());

    if (ante > oldAnte) {
        logs.push_back("[Executing NextAnte commands...]");
        auto nextAnteLogs = executePendingCommands("NextAnte");
        logs.insert(logs.end(), nextAnteLogs.begin(), nextAnteLogs.end());
        
        // Trigger NEXT_ANTE tags
        auto anteTagLogs = triggerTags(TagTrigger::NEXT_ANTE);
        logs.insert(logs.end(), anteTagLogs.begin(), anteTagLogs.end());
    }
    return logs;
}

std::vector<std::string> RuntimeSession::triggerTags(TagTrigger trigger) {
    std::vector<std::string> logs;
    std::vector<std::shared_ptr<Tag>> remaining;
    for (auto& tag : tagStack) {
        if (tag && tag->getTrigger() == trigger) {
            tag->execute(*this);
            logs.push_back("[Tag Triggered: " + tag->getName() + " - " + tag->getDescription() + "]");
        } else {
            remaining.push_back(tag);
        }
    }
    tagStack = std::move(remaining);
    return logs;
}
