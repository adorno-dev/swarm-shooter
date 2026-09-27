#pragma once
#include "raylib.h"

#include "BulletManager.hpp"
#include "EnemyManager.hpp"
#include "Player.hpp"
#include "CollisionMap.hpp"
#include "Minimap.hpp"
#include "DebugOverlay.hpp"
#include "HUD.hpp"
#include "HealthPotionManager.hpp"
#include "BookManager.hpp"
#include "MusicPlayer.hpp"


enum class GameState { Menu, Playing, GameOver };

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
    void drawGameOverOverlay();
    void drawGetReadyOverlay();
    void drawStartScreen();
    void updateEntities(float delta);
    void updateCamera();
    void updateShooting();
    void startWave(int wave);
    void restart();
    void updateWaves(float delta);
    void updateCollisions();
    float accuracy() const;
    void completeWave();

    Player _player;
    CollisionMap _collisionMap;
    Camera2D _camera = {};
    BulletManager _bullets;
    EnemyManager _enemies;
    HealthPotionManager _healthPotions;
    BookManager _books;
    Minimap _minimap;
    DebugOverlay _debugOverlay;
    HUD _hud;
    MusicPlayer _music;
    GameState _gameState = GameState::Menu;

    int _wave = 0;
    int _waveBookCount = 0;
    float _waveTime = 0.0f;
    bool _waveRunning = false;
    float _pauseTimer = 0.0f;
    int _shotsFired = 0;
    int _killCount = 0;
    int _score = 0;
    int _lastWaveBonus = 0;
};