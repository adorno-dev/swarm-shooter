#pragma once
#include "raylib.h"

#include "PoolObject.hpp"
#include "Transform2D.hpp"
#include "Sprite.hpp"
#include "CircleCollider.hpp"


class HealthPotion : public PoolObject
{
public:
    HealthPotion();

    void Update(float delta) override;
    void Draw() override;
    void Deactivate() override;

    void Activate(Vector2 pos);

    Vector2 GetPosition() const { return _transform.position; }
    const CircleCollider& GetCollider() const { return _collider; }

private:
    Transform2D _transform;
    Sprite _sprite;
    CircleCollider _collider;
    
    float _lifeTime = 0.0f;
};