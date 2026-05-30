#include "hand_selection/Hand.h"
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
    return Card();  // Invalid card
}

int Hand::GetCardCount() const {
    return static_cast<int>(cards.size());
}

void Hand::Clear() {
    cards.clear();
}


