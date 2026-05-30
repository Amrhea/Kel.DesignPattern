#pragma once

#include "card/Card.h"
#include <vector>

class Hand
{
private:
    std::vector<Card> cards;

public:
    Hand();
    ~Hand();

    // Method untuk menambah kartu
    void AddCard(Card card);

    // Method untuk menghapus kartu (berdasarkan index atau objek)
    void RemoveCard(int index);

    // Method untuk mendapatkan kartu di index tertentu
    Card GetCard(int index) const;

    // Method untuk mendapatkan jumlah kartu
    int GetCardCount() const;

    // Method untuk membersihkan hand
    void Clear();

    // Method untuk mendapatkan semua kartu
    const std::vector<Card>& GetCards() const { return cards; }
};
