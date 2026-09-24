#pragma once
#include "Transform2D.hpp"
#include "Sprite.hpp"

class Player;

class Enemy
{
public:
    Enemy();
    void Update(float dt);
    void Draw();
    void SetPosition(Vector2 position);
    void SetPlayer(const Player* player);
private:
    Transform2D _transform;
    Sprite _sprite;
    const Player* _player = nullptr;
    float _speed = 80.0f;
    float _retargetTimer = 0.0f;
    float _retargetMin = 1.0f;
    float _retargetMax = 2.0f;

    void Retarget();
};