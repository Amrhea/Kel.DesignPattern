#pragma once
#include "poker_evaluation/PokerHandType.h"
#include "card/Card.h"
#include <vector>
#include <string>

struct ScoreContext {
    PokerHandType handType;
    std::string handName;
    int chips;
    int mult;
    std::vector<Card> playedCards;

    ScoreContext(PokerHandType type, const std::string& name, int c, int m, const std::vector<Card>& cards)
        : handType(type), handName(name), chips(c), mult(m), playedCards(cards) {}

    int calculateFinalScore() const {
        return chips * mult;
    }
};
