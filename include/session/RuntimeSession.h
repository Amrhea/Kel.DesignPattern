#pragma once
#include <vector>
#include <string>
#include <memory>
#include "scoring/HandScore.h"
#include "joker/JokerCard.h"

#include "card/Card.h"
#include "state/RunSessionState.h"

class BlindState;
class RewardCommand;

class RuntimeSession {
public:
    RunSessionState sessionState;

    int& ante;
    int& remainingPlays;
    int& gold;
    std::vector<std::string> deck;
    std::vector<Card> persistentDeck;
    std::shared_ptr<BlindState>& currentBlind;
    std::vector<std::shared_ptr<RewardCommand>>& pendingCommands;
    
    HandScoreTable handScores;
    std::vector<std::shared_ptr<Observer>>& jokers;

    RuntimeSession();
    ~RuntimeSession();

    std::vector<std::string> executePendingCommands(const std::string& timing);
    std::vector<std::string> skipBlind();
    std::vector<std::string> playBlind();

    void addGold(int amount);
    void addJoker(std::shared_ptr<Observer> joker);
    void upgradeHand(PokerHandType handType, int chipsBonus, int multBonus);
};
