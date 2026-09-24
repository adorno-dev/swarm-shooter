#pragma once
#include "raylib.h"
#include <string>

class CollisionMap
{
public:
    void Init(const std::string& name);
    bool IsWalkable(float x, float y) const;
private:
    const Image* _img = nullptr;
};