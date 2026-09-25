#pragma once
#include "raylib.h"
#include "BulletManager.hpp"
#include "EnemyManager.hpp"
#include "Player.hpp"
#include "CollisionMap.hpp"


enum class GameState { Playing, GameOver };

class Game
{
public:
    Game();
    ~Game();

    bool HandleInput();
    void Update(float delta);
    void Draw(RenderTexture2D& canvas);

private:
    void drawWorld();
    void drawHUD();
    void drawGameOverOverlay();
    void updateEntities(float delta);
    void updateCamera();
    void updateShooting();
    void startWave(int wave);
    void restart();
    void updateWaves();
    void updateCollisions();

    Player _player;
    CollisionMap _collisionMap;
    Camera2D _camera = {};
    BulletManager _bullets;
    EnemyManager _enemies;
    GameState _gameState = GameState::Playing;
    int _wave = 1;
};