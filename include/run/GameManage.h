#pragma once

#include "run/RunState.h"
#include "run/AnteManager.h"
#include "run/RoundManager.h"
#include "session/RuntimeSession.h"
#include <memory>

class GameManager
{
    private:
        static GameManager* instance;

        RunState state;
        std::unique_ptr<RuntimeSession> session;
        std::unique_ptr<AnteManager> anteManager;
        std::unique_ptr<RoundManager> roundManager;

        GameManager();

        void HandleMainMenu();
        void HandleStartRun();
        void HandleSelectBlind();
        void HandlePlayHand();
        void HandleResolveScore();
        void HandleReward();
        void HandleShop();
        void HandleGameOver();

    public:
        static GameManager* GetInstance();
        ~GameManager();
        void RunSession();
};
