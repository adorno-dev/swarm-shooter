#pragma once
#include "raylib.h"

#include "Enemy.hpp"
#include "PoolObjectManager.hpp"


class Player;

class EnemyManager : public PoolObjectManager<Enemy>
{
public:
    void Init(Player* player);
    void SpawnBatch(int count);
    void Update(float delta) override;

    bool IsBatchComplete() const { return _batchRemaining == 0 && CountAlive() == 0; }

private:
    void Spawn(Vector2 pos);
    Vector2 pickSpawnPoint() const;
    
    Player* _player = nullptr;
    float _staggerInterval = 0.15f;
    float _staggerTimer = 0.0f;
    int _batchRemaining = 0;
};