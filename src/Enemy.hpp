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

class Enemy : public PoolObject
{
public:
    Enemy();

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

    void Retarget();
};