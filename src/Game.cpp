#include "Game.hpp"

#include "raylib.h"

#include "GameInput.hpp"
#include "GameConfig.hpp"
#include "ResourceKeys.hpp"
#include "ResourceManager.hpp"

#include <algorithm>


Game::Game() : _player(RK::PLAYER)
{
    const Texture2D &background = RM::get().GetTexture(RK::GAME_BG);
    GameConfig::MAP_W = (float)background.width;
    GameConfig::MAP_H = (float)background.height;

    _minimap.Init(_player, _enemies, _books);
    _debugOverlay.Init(_player, _bullets, _enemies, _camera);
    _hud.Init(_player);

    _collisionMap.Init(RK::GAME_BG_COLLISION);

    _player.SetPosition(GameConfig::MapCenter());
    _player.SetCollisionMap(&_collisionMap);

    _camera.zoom = 1.0f;
    _camera.offset = { GameConfig::HALF_BASE_W, GameConfig::HALF_BASE_H };
    _camera.target = GameConfig::MapCenter();

    _enemies.Init(&_player);
    _books.Init(&_player);

    _music.Init(RK::MUSIC_MAIN);
    _music.Start(0.4f);
}

Game::~Game() {}

bool Game::HandleInput()
{
    if (IsKeyPressed(KEY_Q)) return true;

    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (IsCursorHidden()) EnableCursor();
        else DisableCursor();
    }

    if (IsKeyPressed(KEY_F1))
        GameConfig::SHOW_DEBUG = !GameConfig::SHOW_DEBUG;

    if (_gameState == GameState::GameOver && IsKeyPressed(KEY_R))
        restart();
    else if (_gameState == GameState::Menu && IsKeyPressed(KEY_ENTER))
    {
        restart();
        _gameState = GameState::Playing;

        DisableCursor();
    }
    else if ((_gameState == GameState::Playing || _gameState == GameState::GameOver) && IsKeyPressed(KEY_M))
    {
        _gameState = GameState::Menu;

        EnableCursor();
    }

    return false;
}

void Game::Update(float delta)
{
    _music.Update();

    if (_gameState != GameState::Playing) return;

    updateEntities(delta);
    updateShooting();
    updateCollisions();
    updateWaves(delta);
    updateCamera();

    if (_player.IsDead()) _gameState = GameState::GameOver;
}

void Game::Draw(RenderTexture2D& canvas)
{
    BeginTextureMode(canvas);
    ClearBackground(BLACK);
    if (_gameState == GameState::Menu)
        drawStartScreen();
    else 
    {
        drawWorld();
        HUDInfo hudInfo = { _wave, _waveTime, _waveRunning, _books.CountAlive(), _waveBookCount, _lastWaveBonus };
        _hud.Draw(hudInfo);
        _minimap.Draw();
        _debugOverlay.Draw();
        if (_gameState == GameState::Playing && !_waveRunning)  drawGetReadyOverlay();
        if (_gameState == GameState::GameOver) drawGameOverOverlay();
    }
    EndTextureMode();
}

float Game::accuracy() const
{
    if (_shotsFired == 0)
        return 0.0f;
    return ((float)_killCount / (float)_shotsFired) * 100.0f;
}

void Game::completeWave()
{
    int timeBonus = (int)_waveTime;
    int waveBonus = _wave * GameConfig::WAVE_BONUS_BASE;
    int accuracyBonus = (int)accuracy();
    _lastWaveBonus = timeBonus + waveBonus + accuracyBonus;
    _score = _lastWaveBonus;
    _waveRunning = false;
    _pauseTimer = 0.0f;

    TraceLog(LOG_INFO, "Game ave %d complete: _wavetime: %.1f timeBonus: %d waveBonus: %d accuracyBonus: %d _score: %d",
        _wave, _waveTime, timeBonus, waveBonus, accuracyBonus, _score);
}

void Game::startWave(int wave)
{
    _wave = wave;
    _waveTime = GameConfig::WAVE_TIME_LIMIT;
    _waveRunning = true;
    _pauseTimer = 0.0f;
    _healthPotions.DeactivateAll();
    _books.DeactivateAll();
    _enemies.SpawnBatch(
        GameConfig::WAVE_ENEMY_BASE + 
        GameConfig::WAVE_ENEMY_RAMP * wave);

    int idx = std::clamp(_wave - 1, 0, (int)GameConfig::WAVE_BOOK_COUNTS.size() - 1);
    _waveBookCount = GameConfig::WAVE_BOOK_COUNTS[idx];
    _books.SpawnBatch(_waveBookCount);
}

void Game::updateWaves(float delta)
{
    if (_waveRunning)
    {
        _waveTime -= delta;
        if (_waveTime < 0.0f)
        {
            _gameState = GameState::GameOver;
            _waveTime = 0.0f;
            return;
        }

        if (_enemies.IsBatchComplete() && _books.CountAlive() == 0)
            completeWave();
    }
    else
    {
        _pauseTimer += delta;

        if (_pauseTimer >= GameConfig::WAVE_PAUSE)
            startWave(_wave + 1);
    }
}

void Game::updateShooting()
{
    if (GI::get().State().shoot)
    {
        _bullets.Spawn(_player.GetFiringPosition(), GI::get().State().aimAngle);
        _shotsFired++;
    }
}

void Game::updateEntities(float delta)
{
    GI::get().Update();

    _player.Update(delta);
    _bullets.Update(delta);
    _enemies.Update(delta);
    _healthPotions.Update(delta);
}

void Game::updateCamera()
{
    _camera.target = _player.GetPosition();

    _camera.target.x = std::clamp(
        _camera.target.x, 
        GameConfig::HALF_BASE_W, 
        GameConfig::MAP_W - GameConfig::HALF_BASE_W);
    
    _camera.target.y = std::clamp(
        _camera.target.y, 
        GameConfig::HALF_BASE_H, 
        GameConfig::MAP_H - GameConfig::HALF_BASE_H);
}

void Game::updateCollisions()
{
    for (auto& bullet : _bullets.GetPool())
    {
        if (!bullet->IsAlive()) continue;

        for (auto &enemy: _enemies.GetPool())
        {
            if (!enemy->IsAlive() || !enemy->CanBeHit()) continue;

            if (bullet->GetCollider().IsCollidingWith(enemy->GetCollider()))
            {
                bullet->Deactivate();
                enemy->Kill();
                _killCount++;
                if (GetRandomValue(0, 100) <= GameConfig::HEALTH_DROP_CHANCE)
                    _healthPotions.Spawn(enemy->GetPosition());
                break;
            }
        }
    }

    for (auto& enemy : _enemies.GetPool())
    {
        if (!enemy->IsAlive() || !enemy->CanBeHit()) continue;

        if (_player.GetCollider().IsCollidingWith(enemy->GetCollider()))
            _player.Hit();
    }

    for (auto& healthPotion : _healthPotions.GetPool())
    {
        if (!healthPotion->IsAlive()) continue;

        if (_player.GetCollider().IsCollidingWith(healthPotion->GetCollider()))
        {
            _player.Heal(1);
            healthPotion->Deactivate();
        }
    }

    for (auto& book : _books.GetPool())
    {
        if (!book->IsAlive()) continue;

        if (_player.GetCollider().IsCollidingWith(book->GetCollider()))
            book->Deactivate();
    }
}

void Game::drawWorld()
{
    BeginMode2D(_camera);
    DrawTexture(RM::get().GetTexture(RK::GAME_BG), 0, 0, WHITE);
    _player.Draw();
    _bullets.Draw();
    _enemies.Draw();
    _healthPotions.Draw();
    _books.Draw();
    DrawTexture(RM::get().GetTexture(RK::GAME_FG), 0, 0, WHITE);
    EndMode2D();
}

void Game::drawStartScreen()
{
    DrawCenteredText("SWARM", GameConfig::BASE_H / 2 - 60, 60, RAYWHITE);
    DrawCenteredText("Press ENTER to start", GameConfig::BASE_H / 2 + 12, 32, GRAY);
}

void Game::drawGameOverOverlay()
{
    DrawRectangle(0, 0, GameConfig::BASE_W, GameConfig::BASE_H, ColorAlpha(BLACK, 0.7f));

    const char* title = "GAME OVER";
    DrawCenteredText(title, GameConfig::BASE_H / 2 - 60, 60, RED);

    const char* stats = TextFormat("Wave %d | Score: %d | Killed: %d | Acc.: %.1f%%",
        _wave, _score, _killCount, accuracy());
    DrawCenteredText(stats, GameConfig::BASE_H / 2 + 12, 20, WHITE);

    const char* prompt = "Press R to restart";
    DrawCenteredText(prompt, GameConfig::BASE_H / 2 + 48, 32, RAYWHITE);
}

void Game::drawGetReadyOverlay()
{
    const char* title = "Wave";
    DrawCenteredText(title, GameConfig::BASE_H / 4 - 60, 60, RAYWHITE);

    const char* prompt = "Get ready...";
    DrawCenteredText(prompt, GameConfig::BASE_H / 4 + 12, 32, GRAY);
}

void Game::restart()
{
    _player.Reset();
    _player.SetPosition(GameConfig::MapCenter());
    _bullets.DeactivateAll();
    _enemies.DeactivateAll();
    _healthPotions.DeactivateAll();
    _books.DeactivateAll();
    _gameState = GameState::Playing;
    _waveRunning = false;
    _wave = 0;
    _pauseTimer = 0.0f;
    _shotsFired = 0;
    _killCount = 0;
    _score = 0;
    _lastWaveBonus = 0;
}