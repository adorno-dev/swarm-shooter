#include "Sprite.hpp"

bool Sprite::showDebug = false;

void Sprite::Init(Texture2D* initTexture)
{
    texture = initTexture;
}

void Sprite::Draw(const Transform2D& transform) const
{
    if (!texture) return;

    float w = texture->width * transform.scale;
    float h = texture->height * transform.scale;

    Rectangle src = {
        0, 0, (float)texture->width, (float)texture->height
    };

    Rectangle dst = {
        transform.position.x,
        transform.position.y,
        w, h
    };

    Vector2 origin = {
        w * pivot.x,
        h * pivot.y,
    };

    DrawTexturePro(*texture, src, dst, origin, transform.rotation, WHITE);

    if (showDebug)
        DrawCircle((int)transform.position.x, (int)transform.position.y, 2.0f, RED);
}