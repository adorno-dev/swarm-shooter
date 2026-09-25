#pragma once
#include "raylib.h"

#include "Bullet.hpp"
#include "PoolObjectManager.hpp"

#include <vector>
#include <memory>


class BulletManager : public PoolObjectManager<Bullet>
{
public:
    void Spawn(Vector2 pos, float angleDeg);
};