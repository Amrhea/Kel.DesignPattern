#pragma once

#include <vector>

class Hand
{
private:
    std::vector<int> cards;  // Representasi kartu (misal: 0-51 untuk deck standar)

public:
    Hand();
    ~Hand();

    // Method untuk menambah kartu
    void AddCard(int card);

    // Method untuk menghapus kartu
    void RemoveCard(int card);

    // Method untuk mendapatkan kartu di index tertentu
    int GetCard(int index) const;

    // Method untuk mendapatkan jumlah kartu
    int GetCardCount() const;

    // Method untuk membersihkan hand
    void Clear();

    // Method untuk menampilkan kartu (untuk debugging)
    void ShowCards() const;
};
