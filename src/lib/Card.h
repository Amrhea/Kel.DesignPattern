#ifndef CARD_H
#define CARD_H

struct Card {
    int rank;  // 0-12 (Ace to King)
    char suit; // 'C', 'D', 'H', 'S'

    bool operator==(const Card& other) const {
        return rank == other.rank && suit == other.suit;
    }
};

#endif // CARD_H
