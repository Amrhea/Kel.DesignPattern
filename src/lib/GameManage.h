#pragma once

#include "Subject.h"
#include "HandGenerator.h"
#include "HandPlayer.h"
#include "ScoringRule.h"
#include "BlindRule.h"
#include "RewardRule.h"
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
