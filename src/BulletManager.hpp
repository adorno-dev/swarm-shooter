#pragma once
#include "raylib.h"

#include "Bullet.hpp"
#include "PoolObjectManager.hpp"


class BulletManager : public PoolObjectManager<Bullet>
{
public:
    void Spawn(Vector2 pos, float angleDeg);
};