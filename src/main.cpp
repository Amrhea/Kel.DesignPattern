#include "run/GameManage.h"

int main() {
    GameManager* gameManager = GameManager::GetInstance();
    gameManager->RunSession();
    
    // Cleanup singleton
    delete gameManager;
    return 0;
}
