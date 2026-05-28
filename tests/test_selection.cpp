#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "hand_selection/Hand.h"
#include "hand_selection/SelectionManager.h"

TEST_CASE("Subsystem B: Selection Tests", "[subsystem_b]") {
    Hand hand;
    for (int i = 0; i < 8; ++i) {
        hand.AddCard(Card(i, 2 + i, 'C'));
    }

    SelectionManager manager;

    SECTION("Select and Unselect") {
        manager.Select(0);
        REQUIRE(manager.GetSelection().Count() == 1);
        REQUIRE(manager.GetSelection().GetIndices()[0] == 0);

        manager.Unselect(0);
        REQUIRE(manager.GetSelection().Count() == 0);
    }

    SECTION("Duplicate Selection Handling") {
        manager.Select(1);
        manager.Select(1);
        REQUIRE(manager.GetSelection().Count() == 1);
    }

    SECTION("Max-5 Enforcement") {
        for (int i = 0; i < 5; ++i) {
            manager.Select(i);
        }
        REQUIRE(manager.Confirm(hand) == true);

        manager.Select(5);
        REQUIRE(manager.Confirm(hand) == false);
    }

    SECTION("Invalid Index Rejection") {
        manager.Select(10); // Hand only has 8 cards
        REQUIRE(manager.Confirm(hand) == false);
    }

    SECTION("Undo Behavior") {
        manager.Select(2);
        REQUIRE(manager.GetSelection().Count() == 1);
        manager.Undo();
        REQUIRE(manager.GetSelection().Count() == 0);
    }

    SECTION("Confirm Behavior (Empty Selection)") {
        REQUIRE(manager.Confirm(hand) == false);
    }
}
