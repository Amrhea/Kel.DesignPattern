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

    public:
        GameManager();
        ~GameManager();
        void RunSession();
};
