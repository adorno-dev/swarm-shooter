#pragma once
#include "raylib.h"
#include <cmath>

inline Vector2 Direction(float angleDeg)
{
    float rad = angleDeg * DEG2RAD;
    return { cosf(rad), sinf(rad) };
}