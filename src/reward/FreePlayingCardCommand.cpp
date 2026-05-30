#include "reward/FreePlayingCardCommand.h"
#include "session/RuntimeSession.h"

std::string FreePlayingCardCommand::getName() const {
    return "Free Playing Card";
}

std::string FreePlayingCardCommand::getTiming() const {
    return "NextAnte";
}

void FreePlayingCardCommand::execute(RuntimeSession& session) {
    session.deck.push_back("PlayingCard");
    session.persistentDeck.push_back(Card(static_cast<int>(session.persistentDeck.size()), 14, 'S'));
}
