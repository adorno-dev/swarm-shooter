#pragma once
#include "Transform2D.hpp"
#include "Sprite.hpp"

class Bullet
{
public:
    Bullet(Vector2 pos, float angleDeg, float speed);
    void Update(float dt);
    void Draw();
private:
    Transform2D _transform;
    Sprite _sprite;
    Vector2 _velocity = { 0.0f, 0.0f };
};