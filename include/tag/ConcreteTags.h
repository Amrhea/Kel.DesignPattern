#pragma once
#include "tag/Tag.h"
#include "session/RuntimeSession.h"
#include "poker_evaluation/PokerHandType.h"
#include <cstdlib>

class HandyTag : public Tag {
public:
    std::string getName() const override { return "Handy Tag"; }
    std::string getDescription() const override { return "+1 Hands Limit next blind."; }
    TagTrigger getTrigger() const override { return TagTrigger::NEXT_BLIND; }
    void execute(RuntimeSession& session) override {
        session.remainingPlays += 1;
    }
};

class EconomyTag : public Tag {
public:
    std::string getName() const override { return "Economy Tag"; }
    std::string getDescription() const override { return "+$5 Gold in next shop."; }
    TagTrigger getTrigger() const override { return TagTrigger::ENTER_SHOP; }
    void execute(RuntimeSession& session) override {
        session.addGold(5);
    }
};

class OrbitalTag : public Tag {
public:
    std::string getName() const override { return "Orbital Tag"; }
    std::string getDescription() const override { return "Upgrades a random poker hand level."; }
    TagTrigger getTrigger() const override { return TagTrigger::NEXT_ANTE; }
    void execute(RuntimeSession& session) override {
        PokerHandType hands[] = { PokerHandType::Pair, PokerHandType::Straight, PokerHandType::Flush, PokerHandType::FullHouse };
        int idx = std::rand() % 4;
        session.upgradeHand(hands[idx], 15, 2);
    }
};
