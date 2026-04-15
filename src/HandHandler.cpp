#include "HandHandler.h"
#include <iostream>

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

HandHandler::HandHandler() : head(nullptr) {
    // Inisialisasi Chain of Responsibility
    // Urutan dari checker paling spesifik ke paling umum
    IPokerHandChecker* fiveOfKind = new FiveOfKindChecker();
    IPokerHandChecker* royalFlush = new RoyalFlushChecker();
    IPokerHandChecker* straightFlush = new StraightFlushChecker();
    IPokerHandChecker* fourOfKind = new FourOfKindChecker();
    IPokerHandChecker* flushHouse = new FlushHouseChecker();
    IPokerHandChecker* fullHouse = new FullHouseChecker();
    IPokerHandChecker* flush = new FlushChecker();
    IPokerHandChecker* straight = new StraightChecker();
    IPokerHandChecker* threeOfKind = new ThreeOfKindChecker();
    IPokerHandChecker* twoPair = new TwoPairChecker();
    IPokerHandChecker* pair = new PairChecker();
    IPokerHandChecker* highCard = new HighCardChecker();

    // Membangun chain
    head = fiveOfKind;
    fiveOfKind->SetNext(royalFlush);
    royalFlush->SetNext(straightFlush);
    straightFlush->SetNext(fourOfKind);
    fourOfKind->SetNext(flushHouse);
    flushHouse->SetNext(fullHouse);
    fullHouse->SetNext(flush);
    flush->SetNext(straight);
    straight->SetNext(threeOfKind);
    threeOfKind->SetNext(twoPair);
    twoPair->SetNext(pair);
    pair->SetNext(highCard);
}

HandHandler::~HandHandler() {
    // Membersihkan semua checker dalam chain
    IPokerHandChecker* current = head;
    while (current != nullptr) {
        IPokerHandChecker* next = current->GetNext();
        delete current;
        current = next;
    }
}

// Method untuk menambahkan checker ke dalam chain (untuk dinamis)
void HandHandler::AddChecker(IPokerHandChecker* checker) {
    if (head == nullptr) {
        head = checker;
        return;
    }

    IPokerHandChecker* current = head;
    while (current->GetNext() != nullptr) {
        current = current->GetNext();
    }
    current->SetNext(checker);
}

// Method untuk menangani permintaan pengecekan hand
bool HandHandler::Handle(const Hand& hand) {
    if (head != nullptr) {
        return head->Handle(hand);
    }
    return false;
}

// Method untuk menampilkan semua kartu
void HandHandler::ShowCards(const Hand& hand) const {
    for (int i = 0; i < hand.GetCardCount(); ++i) {
        std::cout << "Card[" << i << "]: " << hand.GetCard(i) << std::endl;
    }
}
