#include "Enemy.hpp"
#include "ResourceKeys.hpp"
#include "Player.hpp"

Enemy::Enemy()
{
    _sprite.Init(RK::COCKROACH_MOVE, 64, 64, 8, 8.0f);
    _sprite.rotationOffset = 90.0f;
    _transform.scale = 1.0f;
    _transform.rotation = 0.0f;
}

void Enemy::Retarget()
{
    _transform.LookAt(_player->GetPosition());
    _retargetTimer = RandomFloat(_retargetMin, _retargetMax);
}

void Enemy::Update(float dt)
{
    _retargetTimer -= dt;
    if (_retargetTimer < 0.0f)
        Retarget();
        
    _transform.MoveForward(_speed * dt);
    _sprite.Update(dt);
}

void Enemy::Draw()
{
    _sprite.Draw(_transform);
}

void Enemy::SetPosition(Vector2 position)
{
    _transform.position = position;
}

void Enemy::SetPlayer(const Player* player)
{
    _player = player;
}