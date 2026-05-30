#pragma once
#include "reward/RewardCommand.h"
#include "poker_evaluation/PokerHandType.h"
#include "session/RuntimeSession.h"

class PlanetCardCommand : public RewardCommand {
private:
    PokerHandType targetHand;
    int chipsBonus;
    int multBonus;

public:
    PlanetCardCommand(PokerHandType hand, int chips, int mult)
        : targetHand(hand), chipsBonus(chips), multBonus(mult) {}

    std::string getName() const override { 
        std::string handName;
        switch (targetHand) {
            case PokerHandType::HighCard: handName = "High Card"; break;
            case PokerHandType::Pair: handName = "Pair"; break;
            case PokerHandType::TwoPair: handName = "Two Pair"; break;
            case PokerHandType::ThreeOfKind: handName = "Three of a Kind"; break;
            case PokerHandType::Straight: handName = "Straight"; break;
            case PokerHandType::Flush: handName = "Flush"; break;
            case PokerHandType::FullHouse: handName = "Full House"; break;
            case PokerHandType::FourOfKind: handName = "Four of a Kind"; break;
            case PokerHandType::StraightFlush: handName = "Straight Flush"; break;
            default: handName = "Special Hand"; break;
        }
        return "Planet Card (" + handName + ")"; 
    }
    std::string getTiming() const override { return "Immediate"; }
    void execute(RuntimeSession& session) override {
        session.upgradeHand(targetHand, chipsBonus, multBonus);
    }
};
