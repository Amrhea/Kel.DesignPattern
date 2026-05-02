#ifndef CARD_H
#define CARD_H

struct Card {
    int rank;  // 2-14 (2 to Ace)
    char suit; // 'C', 'D', 'H', 'S'

    bool operator==(const Card& other) const {
        return rank == other.rank && suit == other.suit;
    }
};

#endif // CARD_H
