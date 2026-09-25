#pragma once
#include "raylib.h"

#include "HealthPotion.hpp"
#include "PoolObjectManager.hpp"

#include <vector>
#include <memory>


class HealthPotionManager : public PoolObjectManager<HealthPotion>
{
public:
    void Spawn(Vector2 pos);
};