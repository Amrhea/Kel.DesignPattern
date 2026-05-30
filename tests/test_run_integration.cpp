#include "catch.hpp"
#include "run/RoundManager.h"
#include "session/RuntimeSession.h"
#include "poker_evaluation/PokerHandType.h"
#include <sstream>
#include <iostream>

TEST_CASE("Integration: Successful Blind Progression Path", "[integration_run]") {
    // Simulate input: Select option 1 (Play Hand), then select indices 0 1 2 3 4
    std::string input = "1\n0 1 2 3 4\n";
    std::stringstream input_stream(input);
    auto* old_cin = std::cin.rdbuf(input_stream.rdbuf());

    // Capture stdout
    std::stringstream output_stream;
    auto* old_cout = std::cout.rdbuf(output_stream.rdbuf());

    RuntimeSession session;
    // Set a very high score for all hand types to guarantee a win in one play
    for (int i = 0; i <= static_cast<int>(PokerHandType::FlushFive); ++i) {
        session.handScores[static_cast<PokerHandType>(i)] = HandScoreData(1000, 10);
    }
    
    RoundManager rm;
    BlindData blindData(BlindType::SMALL, "Small Blind", 100, 4, 3, 3);
    
    bool won = rm.RunRound(session, blindData);
    
    // Restore stream buffers
    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    REQUIRE(won == true);
}

TEST_CASE("Integration: Loss Path via Give Up", "[integration_run]") {
    // Simulate input: Select option 3 (Give Up)
    std::string input = "3\n";
    std::stringstream input_stream(input);
    auto* old_cin = std::cin.rdbuf(input_stream.rdbuf());

    // Capture stdout
    std::stringstream output_stream;
    auto* old_cout = std::cout.rdbuf(output_stream.rdbuf());

    RuntimeSession session;
    RoundManager rm;
    BlindData blindData(BlindType::SMALL, "Small Blind", 100, 4, 3, 3);
    
    bool won = rm.RunRound(session, blindData);

    // Restore stream buffers
    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);

    REQUIRE(won == false);
}
