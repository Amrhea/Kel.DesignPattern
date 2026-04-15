#pragma once

#include "IPokerHandChecker.h"

// Entry point untuk Chain of Responsibility
class HandHandler
{
private:
    IPokerHandChecker* head;

public:
    HandHandler();
    ~HandHandler();

    // Method untuk menambahkan checker ke dalam chain
    void AddChecker(IPokerHandChecker* checker);

    // Method untuk menangani permintaan pengecekan hand
    bool Handle(const Hand& hand);

    // Method untuk menampilkan semua kartu
    void ShowCards(const Hand& hand) const;
};
