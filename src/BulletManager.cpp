#include "BulletManager.hpp"

#include "raylib.h"


void BulletManager::Spawn(Vector2 pos, float angleDeg)
{
    auto* bullet = spawnInPool();
    bullet->Activate(pos, angleDeg);
}