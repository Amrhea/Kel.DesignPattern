#pragma once

#include "card/Card.h"
#include <vector>
#include <algorithm>
#include <random>

class Deck {
private:
    std::vector<Card> cards;

public:
    void AddCard(const Card& card) {
        cards.push_back(card);
    }

    void Shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    Card Draw() {
        if (cards.empty()) return Card();
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    int RemainingCount() const {
        return static_cast<int>(cards.size());
    }

    void Clear() {
        cards.clear();
    }
};

class DeckFactory {
public:
    static Deck CreateStandardDeck() {
        Deck deck;
        char suits[] = {'C', 'D', 'H', 'S'};
        int id = 0;
        for (char suit : suits) {
            for (int rank = 2; rank <= 14; ++rank) {
                deck.AddCard(Card(id++, rank, suit));
            }
        }
        return deck;
    }
};
