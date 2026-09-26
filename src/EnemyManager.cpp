#include "EnemyManager.hpp"

#include "raylib.h"
#include "raymath.h"

#include "ResourceKeys.hpp"
#include "GameConfig.hpp"
#include "SwarmUtils.hpp"
#include "Player.hpp"


void EnemyManager::Init(Player* player)
{
    _player = player;

    _defs = {
        {
            { RK::COCKROACH_MOVE, 64, 64, 8, 8.0f, 90.0f, true },
            { RK::COCKROACH_DEATH, 64, 64, 32, 64.0f, 90.0f, false },
            1.0f, 85.0f, 30.0f, 1.0f, 2.0f,
            EnemyBehavior::Retarget, 90.0f
        },
        {
            { RK::SCORPION_MOVE, 64, 64, 4, 4.0f, 90.0f, true },
            { RK::SCORPION_DEATH, 64, 64, 8, 16.0f, 90.0f, false },
            1.0f, 70.0f, 30.0f, 1.7f, 4.0f,
            EnemyBehavior::Retarget, 70.0f
        },
        {
            { RK::KLIVER_MOVE, 64, 64, 8, 8.0f, 90.0f, true },
            { RK::KLIVER_DEATH, 64, 64, 16, 32.0f, 90.0f, false },
            1.0f, 110.0f, 30.0f, 2.0f, 3.0f,
            EnemyBehavior::Retarget, 90.0f
        }
    };
}

Vector2 EnemyManager::pickSpawnPoint() const
{
    return RandomSpawnPoint(_player->GetPosition(), 200.0f);
}

void EnemyManager::SpawnBatch(int count)
{
    _batchRemaining = count;
    _staggerTimer = 0.0f;

    TraceLog(LOG_INFO, "ENEMY_MGR: Batch of %d enemies queued.", count);
}

void EnemyManager::Spawn(const EnemyDef& def, Vector2 pos)
{
    auto* enemy = spawnInPool();
    enemy->Init(def);
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
            
            int typeIndex = GetRandomValue(0, (int)_defs.size() - 1);
            Spawn(_defs[typeIndex], pickSpawnPoint());
        }
    }

    PoolObjectManager<Enemy>::Update(delta);
}