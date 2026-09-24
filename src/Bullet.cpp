#include "Bullet.hpp"
#include "ResourceKeys.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "GameConfig.hpp"
#include "SwarmUtils.hpp"

Bullet::Bullet()
{
    _sprite.Init(RK::BULLET, 32, 17, 2, 5.0f);
    _transform.scale = 2.0f;
}

void Bullet::Activate(Vector2 pos, float angleDeg, float speed)
{
    _alive = true;
    _transform.position = pos;
    _transform.rotation = angleDeg;
    _velocity = Direction(angleDeg) * 30.0f;
    _sprite.Reset();
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
    _sprite.Update(dt);

    if (GameConfig::IsOutsiteMap(_transform.position))
        Deactivate();
}

void Bullet::Draw()
{
    if (!_alive) return;

    _sprite.Draw(_transform);
}