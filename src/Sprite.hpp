#pragma once
#include "raylib.h"
#include "Transform2D.hpp"
#include <string>

struct Sprite
{
    static bool showDebug;

    const Texture2D* texture = nullptr;
    Vector2 pivot = { 0.5f, 0.5f };

    int frameWidth = 0;
    int frameHeight = 0;

    void Init(const std::string& initTexture);
    void Draw(const Transform2D& transform) const;
};