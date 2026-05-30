#include "catch.hpp"
#include "joker/ChipsBoostJoker.h"
#include "joker/MultBoostJoker.h"
#include "joker/FlushMultJoker.h"
#include "state/RunSessionState.h"
#include "joker/JokerManager.h"
#include <type_traits>

TEST_CASE("Subsystem I: State Boundary Enforcement", "[boundary]") {
    SECTION("Joker apply signature only accepts ScoreContext reference") {
        // Compile-time check: Joker apply method accepts precisely ScoreContext&
        // chips boost joker
        static_assert(std::is_invocable<decltype(&ChipsBoostJoker::apply), ChipsBoostJoker, ScoreContext&>::value, "apply must accept ScoreContext&");
        static_assert(!std::is_invocable<decltype(&ChipsBoostJoker::apply), ChipsBoostJoker, RunPersistentState&>::value, "apply must NOT accept RunPersistentState&");
        static_assert(!std::is_invocable<decltype(&ChipsBoostJoker::apply), ChipsBoostJoker, BlindRuntimeState&>::value, "apply must NOT accept BlindRuntimeState&");

        // mult boost joker
        static_assert(std::is_invocable<decltype(&MultBoostJoker::apply), MultBoostJoker, ScoreContext&>::value, "apply must accept ScoreContext&");
        
        // flush mult joker
        static_assert(std::is_invocable<decltype(&FlushMultJoker::apply), FlushMultJoker, ScoreContext&>::value, "apply must accept ScoreContext&");
    }

    SECTION("Temporary state does not leak into persistent or runtime state during scoring") {
        RunSessionState sessionState;
        sessionState.persistentState.money = 10;
        sessionState.runtimeState.blindScore = 100;

        std::vector<Card> cards;
        ScoreContext context(PokerHandType::HighCard, "High Card", 5, 2, cards);

        // Apply some joker
        ChipsBoostJoker joker(50);
        joker.apply(context);

        // Assert session/persistent/runtime remains unmutated by the scoring application
        REQUIRE(sessionState.persistentState.money == 10);
        REQUIRE(sessionState.runtimeState.blindScore == 100);
        
        // Final score should only be calculated from the temporary context
        REQUIRE(context.calculateFinalScore() == 110);
    }
}
