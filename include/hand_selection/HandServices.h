#pragma once

#include "card/Card.h"
#include "card/Deck.h"
#include "hand_selection/Hand.h"
#include <vector>

class DrawService {
public:
    static void DrawToHand(Deck& deck, Hand& hand, int n) {
        for (int i = 0; i < n; ++i) {
            Card card = deck.Draw();
            if (card.id != -1) {
                hand.AddCard(card);
            } else {
                break; // No more cards in deck
            }
        }
    }
};

class DiscardService {
public:
    static void Discard(Hand& hand, const std::vector<int>& indices, std::vector<Card>& discardPile) {
        // Sort indices in descending order to avoid index shift issues during removal
        std::vector<int> sortedIndices = indices;
        std::sort(sortedIndices.rbegin(), sortedIndices.rend());

        for (int index : sortedIndices) {
            if (index >= 0 && index < hand.GetCardCount()) {
                discardPile.push_back(hand.GetCard(index));
                hand.RemoveCard(index);
            }
        }
    }
};
