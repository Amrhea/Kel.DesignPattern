#include "poker_evaluation/PokerHandEvaluator.h"
#include "poker_evaluation/checker/FlushFiveChecker.h"
#include "poker_evaluation/checker/FiveOfKindChecker.h"
#include "poker_evaluation/checker/RoyalFlushChecker.h"
#include "poker_evaluation/checker/StraightFlushChecker.h"
#include "poker_evaluation/checker/FourOfKindChecker.h"
#include "poker_evaluation/checker/FlushHouseChecker.h"
#include "poker_evaluation/checker/FullHouseChecker.h"
#include "poker_evaluation/checker/FlushChecker.h"
#include "poker_evaluation/checker/StraightChecker.h"
#include "poker_evaluation/checker/ThreeOfKindChecker.h"
#include "poker_evaluation/checker/TwoPairChecker.h"
#include "poker_evaluation/checker/PairChecker.h"
#include "poker_evaluation/checker/HighCardChecker.h"

PokerHandEvaluator::PokerHandEvaluator() : head(nullptr) {
    // Initialize default chain in order from most specific to most general
    AddChecker(std::make_unique<FlushFiveChecker>());
    AddChecker(std::make_unique<FiveOfKindChecker>());
    AddChecker(std::make_unique<RoyalFlushChecker>());
    AddChecker(std::make_unique<StraightFlushChecker>());
    AddChecker(std::make_unique<FourOfKindChecker>());
    AddChecker(std::make_unique<FlushHouseChecker>());
    AddChecker(std::make_unique<FullHouseChecker>());
    AddChecker(std::make_unique<FlushChecker>());
    AddChecker(std::make_unique<StraightChecker>());
    AddChecker(std::make_unique<ThreeOfKindChecker>());
    AddChecker(std::make_unique<TwoPairChecker>());
    AddChecker(std::make_unique<PairChecker>());
    AddChecker(std::make_unique<HighCardChecker>());
}

PokerHandEvaluator::~PokerHandEvaluator() {}

void PokerHandEvaluator::AddChecker(std::unique_ptr<IPokerHandChecker> checker) {
    if (!head) {
        head = std::move(checker);
        return;
    }

    IPokerHandChecker* current = head.get();
    while (current->GetNext()) {
        current = current->GetNext();
    }
    current->SetNext(std::move(checker));
}

HandEvaluation PokerHandEvaluator::Evaluate(const Hand& hand) {
    if (head) {
        return head->Handle(hand);
    }
    return HandEvaluation();
}
