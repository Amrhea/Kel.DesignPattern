#include "session/RuntimeSession.h"
#include "blind/BlindState.h"
#include <iostream>
#include <string>

int main() {
    RuntimeSession session;
    
    // Demonstrate loop for Ante 1
    while (session.ante == 1) {
        std::cout << "=== Ante " << session.ante << " ===" << std::endl;
        std::cout << "Current Blind: " << session.currentBlind->getName() << std::endl;
        std::cout << "Target Score: " << session.currentBlind->getTargetScore(session.ante) << std::endl;
        std::cout << "Action? (play/skip): ";
        
        std::string choice;
        if (!(std::cin >> choice)) {
            choice = "skip";
            std::cout << choice << std::endl;
        }
        
        if (choice == "skip") {
            session.skipBlind();
        } else {
            session.playBlind();
        }
        std::cout << std::endl;
    }
    
    // Show Ante 2 entry
    std::cout << "=== Ante " << session.ante << " ===" << std::endl;
    std::cout << "Current Blind: " << session.currentBlind->getName() << std::endl;
    std::cout << "Target Score: " << session.currentBlind->getTargetScore(session.ante) << std::endl;
    
    return 0;
}
