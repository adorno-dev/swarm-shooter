#include "Enemy.hpp"

#include "raylib.h"

#include "ResourceKeys.hpp"
#include "Player.hpp"
#include "GameConfig.hpp"
#include "SwarmUtils.hpp"


void Enemy::Init(const EnemyDef& def)
{

    _spriteMove.Init(
        def.move.textureKey,
        def.move.frameWidth,
        def.move.frameHeight,
        def.move.frameCount,
        def.move.framesPerSecond,
        def.move.loop);

    _spriteMove.rotationOffset = def.move.rotationOffset;

    _spriteDeath.Init(
        def.death.textureKey,
        def.death.frameWidth,
        def.death.frameHeight,
        def.death.frameCount,
        def.death.framesPerSecond,
        def.death.loop);
    
    _spriteDeath.rotationOffset = def.death.rotationOffset;
    _transform.scale = def.scale;
    _transform.rotation = 0.0f;
    _speed = def.speed;
    _collider.Init(def.colliderRadius, _transform);
    _retargetMin = def.retargetMin;
    _retargetMax = def.retargetMax;
    _behaviour = def.behaviour;
    _turnSpeed = def.turnSpeed;
}

void Enemy::Kill()
{
    if (_state == EnemyState::Dying) return;

    _state = EnemyState::Dying;
    _spriteDeath.Reset();
    
    TraceLog(LOG_INFO, "Enemy killed!");
}

void Enemy::Retarget()
{
    _transform.LookAt(_player->GetPosition());
    _retargetTimer = RandomFloat(_retargetMin, _retargetMax);
}

void Enemy::homingSteer(float delta)
{
    float desired = AngleToTargetDeg(_transform.position, _player->GetPosition());
    float diff = desired - _transform.rotation;

    while (diff > 180.0f) diff -= 360.0f;
    while (diff < -180.0f) diff += 360.0f;

    float maxStep = _turnSpeed * delta;
    if (diff > maxStep) diff = maxStep;
    if (diff > -maxStep) diff = -maxStep;

    _transform.rotation += diff;
}

void Enemy::Update(float delta)
{
    if (!_alive) return;

    switch (_state)
    {
        case EnemyState::Moving:
            if (_behaviour == EnemyBehavior::Homing)
                homingSteer(delta);
            else
            {
                _retargetTimer -= delta;
                if (_retargetTimer < 0.0f)
                    Retarget();
            }
            _transform.MoveForward(_speed * delta);
            _spriteMove.Update(delta);
            break;
        case EnemyState::Dying:
            _spriteDeath.Update(delta);
            if (_spriteDeath.finished)
                Deactivate();
        default:
            break;
    }
}

void Enemy::Activate(Vector2 position)
{
    _alive = true;
    _transform.position = position;
    _retargetTimer = 0.0f;
    _state = EnemyState::Moving;
    _spriteDeath.Reset();
    _spriteMove.Reset();

    TraceLog(LOG_INFO, "ENEMY: Activated");
}

void Enemy::Deactivate()
{
    _alive = false;
    _transform.position = GameConfig::OFFSCREEN_POSITION;

    TraceLog(LOG_INFO, "ENEMY: Deactivated");
}

void Enemy::Draw()
{
    if (!_alive) return;

    switch (_state)
    {
        case EnemyState::Moving:
            _spriteMove.Draw(_transform);
            break;;
        case EnemyState::Dying:
            _spriteDeath.Draw(_transform);
            break;;
    }
    
    _collider.DrawDebug();
}

void Enemy::SetPosition(Vector2 position)
{
    _transform.position = position;
}

void Enemy::SetPlayer(const Player* player)
{
    _player = player;
}