#include "HealthPotionManager.hpp"

#include "raylib.h"



void HealthPotionManager::Spawn(Vector2 pos)
{
    auto* healthPotion = spawnInPool();
    healthPotion->Activate(pos);
}