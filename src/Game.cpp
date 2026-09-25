#include <algorithm>
#include "Game.hpp"
#include "GameInput.hpp"
#include "GameConfig.hpp"
#include "ResourceKeys.hpp"
#include "ResourceManager.hpp"


Game::Game() : _player(RK::PLAYER)
{
    const Texture2D &background = RM::get().GetTexture(RK::GAME_BG);
    GameConfig::MAP_W = (float)background.width;
    GameConfig::MAP_H = (float)background.height;

    _minimap.Init(_player, _enemies);

    _collisionMap.Init(RK::GAME_BG_COLLISION);

    _player.SetPosition(GameConfig::MapCenter());
    _player.SetCollisionMap(&_collisionMap);

    _camera.zoom = 1.0f;
    _camera.offset = { GameConfig::HALF_BASE_W, GameConfig::HALF_BASE_H };
    _camera.target = GameConfig::MapCenter();

    _enemies.Init(&_player);

    startWave(_wave);
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

    return false;
}

void Game::Update(float delta)
{
    if (_gameState != GameState::Playing) return;

    updateEntities(delta);
    updateShooting();
    updateCollisions();
    updateWaves();
    updateCamera();

    if (_player.IsDead()) _gameState = GameState::GameOver;
}

void Game::Draw(RenderTexture2D& canvas)
{
    BeginTextureMode(canvas);
    ClearBackground(BLACK);
    drawWorld();
    drawHUD();
    _minimap.Draw();
    if (_gameState == GameState::GameOver) drawGameOverOverlay();
    EndTextureMode();
}

void Game::startWave(int wave)
{
    _wave = wave;

    _enemies.SpawnBatch(
        GameConfig::WAVE_ENEMY_BASE + 
        GameConfig::WAVE_ENEMY_RAMP * wave);
}

void Game::updateWaves()
{
    if (_enemies.IsBatchComplete())
        startWave(_wave + 1);
}

void Game::updateShooting()
{
    if (GI::get().State().shoot)
        _bullets.Spawn(_player.GetFiringPosition(), GI::get().State().aimAngle);
}

void Game::updateEntities(float delta)
{
    GI::get().Update();

    _player.Update(delta);
    _bullets.Update(delta);
    _enemies.Update(delta);
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
}

void Game::drawWorld()
{
    BeginMode2D(_camera);
    DrawTexture(RM::get().GetTexture(RK::GAME_BG), 0, 0, WHITE);
    _player.Draw();
    _bullets.Draw();
    _enemies.Draw();
    DrawTexture(RM::get().GetTexture(RK::GAME_FG), 0, 0, WHITE);
    EndMode2D();
}

void Game::drawHUD()
{
    DrawRectangle(0, GameConfig::BASE_H - 32, GameConfig::BASE_W, 32, ColorAlpha(DARKBLUE, 0.6f));

    DrawText(
        TextFormat(
            "Player: %.0f,%.0f", 
            _player.GetPosition().x, 
            _player.GetPosition().y), 
        12, GameConfig::BASE_H - 24, 20, LIME);

    DrawText(
        TextFormat(
            "Camera: %.0f,%.0f", 
            _camera.target.x, 
            _camera.target.y), 
        256, GameConfig::BASE_H - 24, 20, LIME);

    DrawText(
        TextFormat(
            "Aim: %.1f", 
            GI::get().State().aimAngle), 
        512, GameConfig::BASE_H - 24, 20, LIME);

    DrawText(
        TextFormat(
            "WV: %d  HP: %d/%d  Bullets: %d/%d  Enemies: %d/%d", 
            _wave,
            _player.GetHealth(),
            _player.GetMaxHealth(),
            _bullets.CountAlive(),
            _bullets.GetPoolTotal(),
            _enemies.CountAlive(),
            _enemies.GetPoolTotal()), 
        700, GameConfig::BASE_H - 24, 20, LIME);
}

void Game::drawGameOverOverlay()
{
    DrawRectangle(0, 0, GameConfig::BASE_W, GameConfig::BASE_H, ColorAlpha(BLACK, 0.7f));

    const char* title = "GAME OVER";
    int titleW = MeasureText(title, 60);

    DrawText(title, 
        (GameConfig::BASE_W - titleW) / 2,
         GameConfig::BASE_H / 2 - 60, 60, RED);

    const char* prompt = "Press R to restart";
    int promptW = MeasureText(prompt, 32);

    DrawText(prompt, 
        (GameConfig::BASE_W - promptW) / 2,
         GameConfig::BASE_H / 2 + 12, 32, RAYWHITE);
}

void Game::restart()
{
    _player.Reset();
    _player.SetPosition(GameConfig::MapCenter());
    _bullets.DeactivateAll();
    _enemies.DeactivateAll();
    _gameState = GameState::Playing;
    startWave(1);
}