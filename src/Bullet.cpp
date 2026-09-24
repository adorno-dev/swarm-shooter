#include "Bullet.hpp"
#include "ResourceKeys.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "GameConfig.hpp"
#include "SwarmUtils.hpp"

Bullet::Bullet()
{
    _sprite.Init(RK::BULLET);
    _sprite.frameWidth = 32;
    _sprite.frameHeight = 17;
    _transform.scale = 0.5f;
}

void Bullet::Activate(Vector2 pos, float angleDeg, float speed)
{
    _alive = true;
    _transform.position = pos;
    _transform.rotation = angleDeg;
    _velocity = Direction(angleDeg) * speed;
}

void Bullet::Deactivate()
{
    _alive = false;
    _transform.position = GameConfig::OFFSCREEN_POSITION;
    _velocity = { 0.0f, 0.0f };
}

void Bullet::Update(float dt)
{
    if (!_alive) return;

    _transform.position += _velocity * dt;

    if (GameConfig::IsOutsiteMap(_transform.position))
        Deactivate();
}

void Bullet::Draw()
{
    if (!_alive) return;

    _sprite.Draw(_transform);
}