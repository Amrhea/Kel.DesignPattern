#include "lib/HandHandler.h"
#include <memory>

HandHandler::HandHandler() : head(nullptr) {
    // Inisialisasi Chain of Responsibility
    // Urutan dari checker paling spesifik ke paling umum
    auto fiveOfKind = std::make_unique<FiveOfKindChecker>();
    auto royalFlush = std::make_unique<RoyalFlushChecker>();
    auto straightFlush = std::make_unique<StraightFlushChecker>();
    auto fourOfKind = std::make_unique<FourOfKindChecker>();
    auto flushHouse = std::make_unique<FlushHouseChecker>();
    auto fullHouse = std::make_unique<FullHouseChecker>();
    auto flush = std::make_unique<FlushChecker>();
    auto straight = std::make_unique<StraightChecker>();
    auto threeOfKind = std::make_unique<ThreeOfKindChecker>();
    auto twoPair = std::make_unique<TwoPairChecker>();
    auto pair = std::make_unique<PairChecker>();
    auto highCard = std::make_unique<HighCardChecker>();

    // Membangun chain dari paling rare sampai common (backwards to set next correctly)
    pair->SetNext(std::move(highCard));
    twoPair->SetNext(std::move(pair));
    threeOfKind->SetNext(std::move(twoPair));
    straight->SetNext(std::move(threeOfKind));
    flush->SetNext(std::move(straight));
    fullHouse->SetNext(std::move(flush));
    flushHouse->SetNext(std::move(fullHouse));
    fourOfKind->SetNext(std::move(flushHouse));
    straightFlush->SetNext(std::move(fourOfKind));
    royalFlush->SetNext(std::move(straightFlush));
    fiveOfKind->SetNext(std::move(royalFlush));

    head = std::move(fiveOfKind);

    checkerOrder = {
        "High Card",
        "Pair",
        "Two Pair",
        "Three of a Kind",
        "Straight",
        "Flush",
        "Full House",
        "Flush House",
        "Four of a Kind",
        "Straight Flush",
        "Royal Flush",
        "Five of a Kind"
    };
}

HandHandler::~HandHandler() {
    // Smart pointers handle deletion automatically
}

// Method untuk menambahkan checker ke dalam chain
void HandHandler::AddChecker(std::unique_ptr<IPokerHandChecker> checker) {
    if (head == nullptr) {
        head = std::move(checker);
        return;
    }

    IPokerHandChecker* current = head.get();
    while (current->GetNext() != nullptr) {
        current = current->GetNext();
    }
    current->SetNext(std::move(checker));
}

// Method untuk menangani permintaan pengecekan hand
ChosenHand HandHandler::Handle(const Hand& hand) {
    if (head != nullptr) {
        return head->Handle(hand);
    }
    return ChosenHand();
}

ChosenHand HandHandler::evaluate(const Hand& hand) {
    return Handle(hand);
}

// Method untuk menampilkan semua kartu
void HandHandler::ShowCards(const Hand& hand) const {
    for (int i = 0; i < hand.GetCardCount(); ++i) {
        Card card = hand.GetCard(i);
        std::cout << "Card[" << i << "]: Rank " << card.rank << ", Suit " << card.suit << std::endl;
    }
}

void HandHandler::ShowCheckerOrder() const {
    for (size_t i = 0; i < checkerOrder.size(); ++i) {
        std::cout << (i + 1) << ". " << checkerOrder[i] << std::endl;
    }
}

std::string HandHandler::GetCheckerNameByOrder(int order) const {
    if (order < 1 || order > static_cast<int>(checkerOrder.size())) {
        return "";
    }

    return checkerOrder[order - 1];
}
