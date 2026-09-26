#pragma once
#include "raylib.h"

#include "PoolObject.hpp"
#include "Transform2D.hpp"
#include "Sprite.hpp"
#include "CircleCollider.hpp"


class Player;

enum class EnemyState
{
    Moving,
    Dying
};

enum class EnemyBehavior
{
    Retarget,
    Homing
};

struct EnemyDef
{
    SpriteDef move;
    SpriteDef death;

    float scale = 1.0f;
    float speed = 80.0f;
    float colliderRadius = 30.0f;
    float retargetMin = 1.0f;
    float retargetMax = 2.0f;

    EnemyBehavior behaviour = EnemyBehavior::Retarget;
    float turnSpeed = 90.0f;
};


class Enemy : public PoolObject
{
public:
    Enemy() = default;
    void Init(const EnemyDef& def);
    void Update(float delta) override;
    void Draw() override;
    void Deactivate() override;

    void Activate(Vector2 pos);
    void Kill();
    bool CanBeHit() const { return _state == EnemyState::Moving; }

    void SetPosition(Vector2 position);
    void SetPlayer(const Player* player);
    Vector2 GetPosition() const { return _transform.position; }
    const CircleCollider& GetCollider() const { return _collider; }
    
private:
    Transform2D _transform;
    Sprite _spriteMove;
    Sprite _spriteDeath;
    CircleCollider _collider;
    const Player* _player = nullptr;

    EnemyState _state = EnemyState::Moving;

    float _speed = 80.0f;
    float _retargetTimer = 0.0f;
    float _retargetMin = 1.0f;
    float _retargetMax = 2.0f;

    EnemyBehavior _behaviour = EnemyBehavior::Retarget;
    float _turnSpeed = 90.0f;

    void Retarget();
    void homingSteer(float delta);
};