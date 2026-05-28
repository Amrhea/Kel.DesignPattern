#include "reward/BonusHandCommand.h"
#include "session/RuntimeSession.h"

std::string BonusHandCommand::getName() const {
    return "Bonus Hand";
}

std::string BonusHandCommand::getTiming() const {
    return "NextBlind";
}

void BonusHandCommand::execute(RuntimeSession& session) {
    session.remainingPlays += 1;
}
