#pragma once

#include "joker/Subject.h"
#include "hand_selection/HandGenerator.h"
#include "run/HandPlayer.h"
#include "scoring/ScoringRule.h"
#include "blind/BlindRule.h"
#include "reward/RewardRule.h"
#include <memory>

class GameManager : public Subject
{
    private:
        static GameManager* instance;

        HandGenerator* handGenerator;
        std::unique_ptr<ScoringRule> scoringRule;
        std::unique_ptr<BlindRule> blindRule;
        std::unique_ptr<RewardRule> rewardRule;

        GameManager();

    public:
        static GameManager* GetInstance();
        ~GameManager();
        void RunSession();
};
