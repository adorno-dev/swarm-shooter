#include "HealthPotionManager.hpp"

#include "raylib.h"



void HealthPotionManager::Spawn(Vector2 pos)
{
    auto* healthPotion = spawnInPool();
    healthPotion->Activate(pos);

    TraceLog(LOG_INFO, "HEALTH_POTION_MGR: Pool growing (size: %d)", (int)_pool.size());
}