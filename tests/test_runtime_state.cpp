#include "catch.hpp"
#include "state/BlindRuntimeState.h"

TEST_CASE("Subsystem I: BlindRuntimeState Verification", "[runtime_state]") {
    BlindRuntimeState rState;

    SECTION("initial values") {
        REQUIRE(rState.blindScore == 0);
        REQUIRE(rState.remainingHands == 4);
        REQUIRE(rState.remainingDiscards == 3);
    }

    SECTION("reset successfully reinitializes variables") {
        rState.blindScore = 1000;
        rState.remainingHands = 1;
        rState.remainingDiscards = 0;

        rState.reset(5, 4);

        REQUIRE(rState.blindScore == 0);
        REQUIRE(rState.remainingHands == 5);
        REQUIRE(rState.remainingDiscards == 4);
    }

    SECTION("score accumulation and decrementing actions") {
        rState.blindScore += 150;
        REQUIRE(rState.blindScore == 150);

        rState.remainingHands--;
        REQUIRE(rState.remainingHands == 3);

        rState.remainingDiscards--;
        REQUIRE(rState.remainingDiscards == 2);
    }
}
