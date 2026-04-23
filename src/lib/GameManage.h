#pragma once

#include "HandGenerator.h"
#include "HandPlayer.h"
#include "ScoringRule.h"
#include "BlindRule.h"
#include "RewardRule.h"


class GameManager
{
    private:
        HandGenerator* handGenerator;
        HandPlayer* handPlayer;
        ScoringRule* scoringRule;
        BlindRule* blindRule;
        RewardRule* rewardRule;

    public:
        GameManager();
        ~GameManager();
        void RunSession();
};
