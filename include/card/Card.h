#ifndef CARD_H
#define CARD_H

#include <string>

struct Card {
    int id;    // Unique ID
    int rank;  // 2-14 (2 to Ace)
    char suit; // 'C', 'D', 'H', 'S'

    Card() : id(-1), rank(-1), suit(' ') {}
    Card(int id, int rank, char suit) : id(id), rank(rank), suit(suit) {}

    bool operator==(const Card& other) const {
        return id == other.id;
    }
};

#endif // CARD_H
