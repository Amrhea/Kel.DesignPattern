#include "lib/Hand.h"
#include <algorithm>
#include <iostream>

Hand::Hand()
    : cards() {
}

Hand::~Hand() {
}

void Hand::AddCard(Card card) {
    cards.push_back(card);
}

void Hand::RemoveCard(int index) {
    if (index >= 0 && index < static_cast<int>(cards.size())) {
        cards.erase(cards.begin() + index);
    }
}

Card Hand::GetCard(int index) const {
    if (index >= 0 && index < static_cast<int>(cards.size())) {
        return cards[index];
    }
    return {-1, ' '};  // Invalid card
}

int Hand::GetCardCount() const {
    return static_cast<int>(cards.size());
}

void Hand::Clear() {
    cards.clear();
}

void Hand::ShowCards() const {
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << "Card[" << i << "]: Rank " << cards[i].rank << ", Suit " << cards[i].suit << std::endl;
    }
}
