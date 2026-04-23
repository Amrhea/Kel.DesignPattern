#include "lib/GameManage.h"
#include "lib/Hand.h"
#include "lib/HandHandler.h"

#include <iostream>

namespace {

Hand BuildSampleHand(int checkerNumber) {
    Hand hand;

    const int sampleCards[][5] = {
        {0, 14, 28, 42, 9},
        {0, 13, 5, 19, 34},
        {0, 13, 1, 14, 30},
        {0, 13, 26, 5, 19},
        {0, 14, 28, 42, 4},
        {0, 2, 5, 7, 9},
        {0, 13, 26, 1, 14},
        {0, 0, 1, 1, 1},
        {0, 13, 26, 39, 5},
        {0, 1, 2, 3, 4},
        {9, 10, 11, 12, 0},
        {0, 13, 26, 39, 0}
    };

    if (checkerNumber < 1 || checkerNumber > 12) {
        return hand;
    }

    for (int card : sampleCards[checkerNumber - 1]) {
        hand.AddCard(card);
    }

    return hand;
}

}  // namespace

GameManager::GameManager()
    : handGenerator(nullptr),
      handPlayer(nullptr),
      scoringRule(nullptr),
      blindRule(nullptr),
      rewardRule(nullptr) {
}

GameManager::~GameManager() {
}

void GameManager::RunSession() {
    HandHandler handHandler;
    int checkerNumber = 0;

    std::cout << "Game session started." << std::endl;
    std::cout << "Checker order from easiest to hardest to appear:" << std::endl;
    handHandler.ShowCheckerOrder();
    std::cout << "\nInput one checker number (1-12): ";

    while (!(std::cin >> checkerNumber)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. Please enter a number between 1 and 12: ";
    }

    const std::string checkerName = handHandler.GetCheckerNameByOrder(checkerNumber);
    if (checkerName.empty()) {
        std::cout << "Number not found. This project currently has 12 checker classes, so use 1-12." << std::endl;
        return;
    }

    const Hand hand = BuildSampleHand(checkerNumber);

    // std::cout << "\nSample hand for checker " << checkerNumber << ":" << std::endl;
    // handHandler.ShowCards(hand);

    std::cout << "\nRunning chain of responsibility..." << std::endl;
    if (!handHandler.Handle(hand)) {
        std::cout << "No checker handled the hand." << std::endl;
    }

    std::cout << "Expected checker: " << checkerName << std::endl;
}
