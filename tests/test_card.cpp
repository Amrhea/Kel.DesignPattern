#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "card/Card.h"
#include "card/Deck.h"
#include "hand_selection/Hand.h"
#include "hand_selection/HandServices.h"
#include <set>

TEST_CASE("Subsystem A: Deck and Card Tests", "[subsystem_a]") {
    SECTION("52 Unique Cards in Standard Deck") {
        Deck deck = DeckFactory::CreateStandardDeck();
        REQUIRE(deck.RemainingCount() == 52);
        
        std::set<int> ids;
        for (int i = 0; i < 52; ++i) {
            Card card = deck.Draw();
            REQUIRE(card.id != -1);
            ids.insert(card.id);
        }
        REQUIRE(ids.size() == 52);
    }

    SECTION("Empty Draw Safety") {
        Deck deck; // Empty deck
        Card card = deck.Draw();
        REQUIRE(card.id == -1);
    }

    SECTION("Draw Count Reduction") {
        Deck deck = DeckFactory::CreateStandardDeck();
        Hand hand;
        DrawService::DrawToHand(deck, hand, 8);
        REQUIRE(hand.GetCardCount() == 8);
        REQUIRE(deck.RemainingCount() == 44);
    }

    SECTION("Discard and Redraw Refill") {
        Deck deck = DeckFactory::CreateStandardDeck();
        Hand hand;
        std::vector<Card> discardPile;

        DrawService::DrawToHand(deck, hand, 8);
        REQUIRE(hand.GetCardCount() == 8);
        
        // Discard 3 cards
        DiscardService::Discard(hand, {0, 1, 2}, discardPile);
        REQUIRE(hand.GetCardCount() == 5);
        REQUIRE(discardPile.size() == 3);
        
        // Redraw to 8
        DrawService::DrawToHand(deck, hand, 3);
        REQUIRE(hand.GetCardCount() == 8);
        REQUIRE(deck.RemainingCount() == 41);
    }

    SECTION("Partial Draw when Deck is Low") {
        Deck deck = DeckFactory::CreateStandardDeck();
        Hand hand;

        // Draw 50 cards
        DrawService::DrawToHand(deck, hand, 50);
        REQUIRE(deck.RemainingCount() == 2);
        
        // Try to draw 5 more
        DrawService::DrawToHand(deck, hand, 5);
        REQUIRE(hand.GetCardCount() == 52);
        REQUIRE(deck.RemainingCount() == 0);
    }
}
