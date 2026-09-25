#pragma once
#include "raylib.h"
#include "BulletManager.hpp"
#include "EnemyManager.hpp"
#include "Player.hpp"
#include "CollisionMap.hpp"
#include "Minimap.hpp"


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
    void drawGetReadyOverlay();
    void updateEntities(float delta);
    void updateCamera();
    void updateShooting();
    void startWave(int wave);
    void restart();
    void updateWaves(float delta);
    void updateCollisions();

    Player _player;
    CollisionMap _collisionMap;
    Camera2D _camera = {};
    BulletManager _bullets;
    EnemyManager _enemies;
    GameState _gameState = GameState::Playing;
    Minimap _minimap;
    int _wave = 0;
    float _waveTime = 0.0f;
    bool _waveRunning = false;
    float _pauseTimer = 0.0f;
};