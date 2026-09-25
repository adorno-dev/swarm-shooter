#include "EnemyManager.hpp"

#include "raylib.h"
#include "raymath.h"

#include "GameConfig.hpp"
#include "SwarmUtils.hpp"
#include "Player.hpp"


void EnemyManager::Init(Player* player)
{
    _player = player;
}

Vector2 EnemyManager::pickSpawnPoint() const
{
    constexpr float MIN_DIST = 200.0f;
    constexpr float MIN_DIST_SQ = MIN_DIST * MIN_DIST;

    Vector2 playerPos = _player->GetPosition();
    Vector2 candidate;

    do
    {
        candidate.x = RandomFloat(0.0f, GameConfig::MAP_W);
        candidate.y = RandomFloat(0.0f, GameConfig::MAP_H);
    } 
    while (Vector2DistanceSqr(candidate, playerPos) < MIN_DIST_SQ);

    return candidate;
}

void EnemyManager::SpawnBatch(int count)
{
    _batchRemaining = count;
    _staggerTimer = 0.0f;

    TraceLog(LOG_INFO, "ENEMY_MGR: Batch of %d enemies queued.", count);
}

void EnemyManager::Spawn(Vector2 pos)
{
    auto* enemy = spawnInPool();
    enemy->Activate(pos);
    enemy->SetPlayer(_player);

    TraceLog(LOG_INFO, "ENEMY_MGR: Pool growing (size: %d)", (int)_pool.size());
}

void EnemyManager::Update(float delta)
{
    if (_batchRemaining > 0)
    {
        // Spawn(pickSpawnPoint());
        // _batchRemaining--;

        _staggerTimer -= delta;
        if (_staggerTimer < 0.0f)
        {
            _staggerTimer = _staggerInterval;
            _batchRemaining--;

            Spawn(pickSpawnPoint());
        }
    }

    PoolObjectManager<Enemy>::Update(delta);
}