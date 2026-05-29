#include "hand_selection/HandGenerator.h"
#include "card/Deck.h"
#include "hand_selection/HandServices.h"

HandGenerator::HandGenerator() {
}

Hand HandGenerator::generateHand() {
    Deck deck = DeckFactory::CreateStandardDeck();
    deck.Shuffle();
    Hand hand;
    DrawService::DrawToHand(deck, hand, 5);
    return hand;
}
