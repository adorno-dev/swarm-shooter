#include "Bullet.hpp"
#include "ResourceKeys.hpp"
#include "raylib.h"
#include <cmath>

Bullet::Bullet(Vector2 pos, float angleDeg, float speed)
{
    _sprite.Init(RK::BULLET);
    _sprite.frameWidth = 32;
    _sprite.frameHeight = 17;
    _transform.position = pos;
    _transform.rotation = angleDeg;
    _transform.scale = 0.5f;

    float rad = angleDeg * DEG2RAD;
    _velocity = { cosf(rad) * speed, sinf(rad) * speed };
}

void Bullet::Update(float dt)
{
    _transform.position.x += _velocity.x * dt;
    _transform.position.y += _velocity.y * dt;
}

void Bullet::Draw()
{
    _sprite.Draw(_transform);
}