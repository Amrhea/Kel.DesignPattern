#pragma once

#include "HandGenerator.h"
#include "HandPlayer.h"
#include "ScoringRule.h"
#include "BlindRule.h"
#include "RewardRule.h"


class GameManager
{
    private:
        // HandPlayer could be added here later as a non-pointer or unique_ptr
        // For now, removing unused null pointers to ensure safety.

    public:
        GameManager();
        ~GameManager();
        void RunSession();
};
