#include "BulletManager.hpp"

#include "raylib.h"


void BulletManager::Spawn(Vector2 pos, float angleDeg)
{
    auto* bullet = spawnInPool();
    bullet->Activate(pos, angleDeg);

    TraceLog(LOG_INFO, "BULLET_MGR: Pool growing (size: %d)", (int)_pool.size());
}