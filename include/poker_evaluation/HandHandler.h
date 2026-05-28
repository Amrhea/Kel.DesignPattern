#pragma once

#include "poker_evaluation/IPokerHandChecker.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include "poker_evaluation/checker/FiveOfKindChecker.h"
#include "poker_evaluation/checker/FourOfKindChecker.h"
#include "poker_evaluation/checker/FlushChecker.h"
#include "poker_evaluation/checker/FullHouseChecker.h"
#include "poker_evaluation/checker/HighCardChecker.h"
#include "poker_evaluation/checker/PairChecker.h"
#include "poker_evaluation/checker/RoyalFlushChecker.h"
#include "poker_evaluation/checker/StraightChecker.h"
#include "poker_evaluation/checker/StraightFlushChecker.h"
#include "poker_evaluation/checker/ThreeOfKindChecker.h"
#include "poker_evaluation/checker/TwoPairChecker.h"
#include "poker_evaluation/checker/FlushHouseChecker.h"

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
