#pragma once

#include "IPokerHandChecker.h"
#include <string>
#include <vector>


#include <iostream>
#include <memory>

#include "checker/FiveOfKindChecker.h"
#include "checker/FourOfKindChecker.h"
#include "checker/FlushChecker.h"
#include "checker/FullHouseChecker.h"
#include "checker/HighCardChecker.h"
#include "checker/PairChecker.h"
#include "checker/RoyalFlushChecker.h"
#include "checker/StraightChecker.h"
#include "checker/StraightFlushChecker.h"
#include "checker/ThreeOfKindChecker.h"
#include "checker/TwoPairChecker.h"
#include "checker/FlushHouseChecker.h"

// Entry point untuk Chain of Responsibility
class HandHandler
{
private:
    std::unique_ptr<IPokerHandChecker> head;
    std::vector<std::string> checkerOrder;

public:
    HandHandler();
    ~HandHandler();

    // Method untuk menambahkan checker ke dalam chain
    void AddChecker(std::unique_ptr<IPokerHandChecker> checker);

    // Method untuk menangani permintaan pengecekan hand
    ChosenHand Handle(const Hand& hand);

    // Method untuk mengevaluasi hand dan memberikan hasil akhir
    ChosenHand evaluate(const Hand& hand);

    // Method untuk menampilkan semua kartu
    void ShowCards(const Hand& hand) const;

    // Method untuk menampilkan urutan checker dari paling mudah ke paling sulit muncul
    void ShowCheckerOrder() const;

    // Method untuk mendapatkan nama checker berdasarkan nomor urutan
    std::string GetCheckerNameByOrder(int order) const;
};
