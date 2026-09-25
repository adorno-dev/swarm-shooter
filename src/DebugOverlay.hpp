#pragma once
#include "raylib.h"


class Player;
class BulletManager;
class EnemyManager;


class DebugOverlay
{
public:
    void Init
    (
        const Player& player,
        const BulletManager& bullets,
        const EnemyManager& enemies,
        const Camera2D& camera
    );

    void Draw() const;

private:
    const Player* _player = nullptr;
    const BulletManager* _bullets = nullptr;
    const EnemyManager* _enemies = nullptr;
    const Camera2D* _camera = nullptr;
};