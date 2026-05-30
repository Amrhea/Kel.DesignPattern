#include "catch.hpp"
#include "state/ScoreContext.h"
#include "joker/ChipsBoostJoker.h"
#include "joker/MultBoostJoker.h"

TEST_CASE("Subsystem I: ScoreContext Verification", "[score_context]") {
    std::vector<Card> cards;
    ScoreContext context(PokerHandType::HighCard, "High Card", 10, 2, cards);

    SECTION("finalScore matches chips times mult") {
        REQUIRE(context.calculateFinalScore() == 20);
    }

    SECTION("Jokers modify only temporary context") {
        ChipsBoostJoker joker1(10);
        MultBoostJoker joker2(5);

        joker1.apply(context);
        joker2.apply(context);

        REQUIRE(context.chips == 20);
        REQUIRE(context.mult == 7);
        REQUIRE(context.calculateFinalScore() == 140);
    }

    SECTION("Calculation correctness is independent of application order for commutative additions") {
        ScoreContext context1(PokerHandType::HighCard, "High Card", 10, 2, cards);
        ScoreContext context2(PokerHandType::HighCard, "High Card", 10, 2, cards);

        ChipsBoostJoker j1(10);
        ChipsBoostJoker j2(5);

        // Order 1
        j1.apply(context1);
        j2.apply(context1);

        // Order 2
        j2.apply(context2);
        j1.apply(context2);

        REQUIRE(context1.chips == context2.chips);
        REQUIRE(context1.calculateFinalScore() == context2.calculateFinalScore());
    }
}
