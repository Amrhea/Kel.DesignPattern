#pragma once
#include "joker/Observer.h"
#include "scoring/ScoreContext.h"
#include <string>
#include <iostream>

class ChipsBoostJoker : public Observer {
private:
    int chipsBonus;
public:
    ChipsBoostJoker(int bonus) : chipsBonus(bonus) {}
    void apply(ScoreContext& context) override {
        context.chips += chipsBonus;
        std::cout << "[ChipsBoostJoker] Added +" << chipsBonus << " chips." << std::endl;
    }
};

class MultiplierJoker : public Observer {
private:
    int multBonus;
public:
    MultiplierJoker(int bonus) : multBonus(bonus) {}
    void apply(ScoreContext& context) override {
        context.mult += multBonus;
        std::cout << "[MultiplierJoker] Added +" << multBonus << " mult." << std::endl;
    }
};

class ConditionalJoker : public Observer {
private:
    PokerHandType targetHand;
    int multBonus;
public:
    ConditionalJoker(PokerHandType hand, int bonus) : targetHand(hand), multBonus(bonus) {}
    void apply(ScoreContext& context) override {
        if (context.handType == targetHand) {
            context.mult += multBonus;
            std::cout << "[ConditionalJoker] Hand matches! Added +" << multBonus << " mult." << std::endl;
        }
    }
};
