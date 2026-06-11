#pragma once
#include <vector>
#include <string>
#include <memory>
#include "scoring/HandScore.h"
#include "joker/JokerCard.h"
#include "tag/Tag.h"

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
    std::vector<std::shared_ptr<Tag>>& tagStack;
    
    HandScoreTable handScores;
    std::vector<std::shared_ptr<Observer>>& jokers;

    RuntimeSession();
    ~RuntimeSession();

    std::vector<std::string> executePendingCommands(const std::string& timing);
    std::vector<std::string> skipBlind();
    std::vector<std::string> playBlind();
    std::vector<std::string> triggerTags(TagTrigger trigger);

    void addGold(int amount);
    void addJoker(std::shared_ptr<Observer> joker);
    void upgradeHand(PokerHandType handType, int chipsBonus, int multBonus);
};
