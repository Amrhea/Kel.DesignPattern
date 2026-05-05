#include "lib/HandGenerator.h"
#include "lib/Card.h"
#include <vector>
#include <algorithm>
#include <random>

HandGenerator::HandGenerator() {
}

Hand HandGenerator::generateHand() {
    std::vector<Card> deck;
    std::vector<char> suits = {'H', 'D', 'C', 'S'};

    for (char suit : suits) {
        for (int rank = 2; rank <= 14; ++rank) {
            Card c;
            c.rank = rank;
            c.suit = suit;
            deck.push_back(c);
        }
    }

    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);

    Hand hand;
    for (int i = 0; i < 5; ++i) {
        hand.AddCard(deck[i]);
    }

    return hand;
}
