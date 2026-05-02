#include "lib/HandHandler.h"

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

    // Membangun chain dari paling rare sampai common
    /**
     * five of kind -> royal flush -> straight flush -> four of kind -> flush house -> full house -> flush -> straight -> three of kind -> two pair -> pair -> high card
     */
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
    // Membersihkan semua checker dalam chain
    IPokerHandChecker* current = head;
    while (current != nullptr) {
        IPokerHandChecker* next = current->GetNext();
        delete current;
        current = next;
    }
}

// Method untuk menambahkan checker ke dalam chain
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
