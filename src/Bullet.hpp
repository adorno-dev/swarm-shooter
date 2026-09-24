#pragma once
#include "Transform2D.hpp"
#include "Sprite.hpp"

class Bullet
{
public:
    Bullet();
    void Update(float dt);
    void Draw();
    Vector2 GetPosition() const { return _transform.position; }
    bool IsAlive() const { return _alive; }
    void Deactivate();
    void Activate(Vector2 pos, float angleDeg, float speed);
private:
    Transform2D _transform;
    Sprite _sprite;
    Vector2 _velocity = { 0.0f, 0.0f };
    bool _alive = true;
};