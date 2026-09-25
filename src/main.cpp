#include <algorithm>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "GameConfig.hpp"
#include "ResourceManager.hpp"
#include "ResourceKeys.hpp"
#include "GameInput.hpp"
#include "CollisionMap.hpp"
#include "BulletManager.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "EnemyManager.hpp"

enum class GameState { Playing, GameOver };

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GameConfig::BASE_W, GameConfig::BASE_H, "Swarm");
    SetTargetFPS(60);
    DisableCursor();
    SetExitKey(KEY_NULL);

    RM::get().Load();

    // #pragma region +Monitor Settings
    // Vector2 pos = GetMonitorPosition(1);
    // SetWindowPosition(pos.x + 320, pos.y + 180);
    // SetWindowSize(1920, 1080);
    // #pragma endregion
           
    const Texture2D& background = RM::get().GetTexture(RK::GAME_BG);
    GameConfig::MAP_H = (float)background.height;
    GameConfig::MAP_W = (float)background.width;
    
    CollisionMap collisionMap;
    collisionMap.Init(RK::GAME_BG_COLLISION);

    RenderTexture2D canvas = LoadRenderTexture(GameConfig::BASE_W, GameConfig::BASE_H);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);

    float halfW = GameConfig::BASE_W * 0.5f;
    float halfH = GameConfig::BASE_H * 0.5f;

    Player player(RK::PLAYER);
    player.SetPosition({GameConfig::MAP_W * 0.5f, GameConfig::MAP_H * 0.5f});
    player.SetCollisionMap(&collisionMap);

    Camera2D camera = {};
    camera.zoom = 1.0f;
    camera.target = player.GetPosition();
    camera.offset = { halfW, halfH };

    BulletManager bullets;

    int wave = 1;

    EnemyManager enemies;
    enemies.Init(&player);
    // enemies.Spawn({ GameConfig::MAP_W * 0.5f + 200.0f, GameConfig::MAP_H * 0.5f });
    enemies.SpawnBatch(GameConfig::WAVE_ENEMY_BASE + GameConfig::WAVE_ENEMY_RAMP * wave);

    GameState gameState = GameState::Playing;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_Q))
            break;
        
        if (IsKeyPressed(KEY_ESCAPE))
            IsCursorHidden()
                ? EnableCursor()
                : DisableCursor();

        if (IsKeyPressed(KEY_F1))
            GameConfig::SHOW_DEBUG = !GameConfig::SHOW_DEBUG;

        if (enemies.IsBatchComplete())
        {
            wave++;
            enemies.SpawnBatch(GameConfig::WAVE_ENEMY_BASE + GameConfig::WAVE_ENEMY_RAMP * wave);
        }

        if (gameState == GameState::GameOver && IsKeyPressed(KEY_R))
        {
            player.Reset();
            player.SetPosition({GameConfig::MAP_W * 0.5f, GameConfig::MAP_H * 0.5f});
            bullets.DeactivateAll();
            enemies.DeactivateAll();

            gameState = GameState::Playing;
            wave = 1;
            enemies.SpawnBatch(GameConfig::WAVE_ENEMY_BASE + GameConfig::WAVE_ENEMY_RAMP * wave);
        }

        GI::get().Update();

        float dt = GetFrameTime();

        if (gameState == GameState::Playing)
        {
            if (GI::get().State().shoot)
                bullets.Spawn(player.GetFiringPosition(), GI::get().State().aimAngle);

            player.Update(dt);
            bullets.Update(dt);
            enemies.Update(dt);

            for (auto& bullet : bullets.GetPool())
            {
                if (!bullet->IsAlive()) continue;

                for (auto& enemy : enemies.GetPool())
                {
                    if (!enemy->IsAlive() || !enemy->CanBeHit()) continue;

                    if (bullet->GetCollider().IsCollidingWith(enemy->GetCollider()))
                    {
                        TraceLog(LOG_INFO, "HIT! Bullet hit enemy");
                        bullet->Deactivate();
                        enemy->Kill();
                        break;
                    }
                }
            }

            for (auto& enemy : enemies.GetPool())
            {
                if (!enemy->IsAlive() || !enemy->CanBeHit()) continue;

                if (player.GetCollider().IsCollidingWith(enemy->GetCollider()))
                {
                    player.Hit();
                }
            }

            camera.target = player.GetPosition();
            camera.target.x = std::clamp(camera.target.x, halfW, GameConfig::MAP_W - halfW);
            camera.target.y = std::clamp(camera.target.y, halfH, GameConfig::MAP_H - halfH);

            if (player.IsDead())
                gameState = GameState::GameOver;
        }
        
        BeginTextureMode(canvas);
            ClearBackground(BLACK);
            BeginMode2D(camera);
                DrawTexture(background, 0, 0, WHITE);
                player.Draw();
                bullets.Draw();
                enemies.Draw();
                DrawTexture(RM::get().GetTexture(RK::GAME_FG), 0, 0, WHITE);
            EndMode2D();

            DrawRectangle(0, GameConfig::BASE_H - 32, GameConfig::BASE_W, 32, ColorAlpha(DARKBLUE, 0.6f));

            DrawText(TextFormat("Player: %.0f,%.0f", player.GetPosition().x, player.GetPosition().y), 12, GameConfig::BASE_H - 24, 20, LIME);
            DrawText(TextFormat("Camera: %.0f,%.0f", camera.target.x, camera.target.y), 256, GameConfig::BASE_H - 24, 20, LIME);
            DrawText(TextFormat("Aim: %.1f", GI::get().State().aimAngle), 512, GameConfig::BASE_H - 24, 20, LIME);
            DrawText(TextFormat("WV: %d  HP: %d/%d  Bullets: %d/%d  Enemies: %d/%d", 
                wave,
                player.GetHealth(),
                player.GetMaxHealth(),
                bullets.CountAlive(), 
                bullets.GetPoolTotal(),
                enemies.CountAlive(), 
                enemies.GetPoolTotal()), 
                700, GameConfig::BASE_H - 24, 20, LIME);

        if (gameState == GameState::GameOver)
        {
            DrawRectangle(0, 0, GameConfig::BASE_W, GameConfig::BASE_H, ColorAlpha(BLACK, 0.7f));

            const char* title = "GAME OVER";
            int titleW = MeasureText(title, 60);

            DrawText(title, (GameConfig::BASE_W - titleW) / 2, GameConfig::BASE_H / 2 - 60, 60, RED);

            const char* prompt = "Press R to restart";
            int promptW = MeasureText(prompt, 32);

            DrawText(prompt, (GameConfig::BASE_W - promptW) / 2, GameConfig::BASE_H / 2 + 12, 32, RAYWHITE);
        }

        EndTextureMode();

        float scale = std::min(
            (float)GetScreenWidth() / GameConfig::BASE_W,
            (float)GetScreenHeight() / GameConfig::BASE_H
        );

        float offsetX = (GetScreenWidth() - GameConfig::BASE_W * scale) * 0.5f;
        float offsetY = (GetScreenHeight() - GameConfig::BASE_H * scale) * 0.5f;

        Rectangle src = { 0, 0, (float)GameConfig::BASE_W, -(float)GameConfig::BASE_H };
        Rectangle dst = { offsetX, offsetY, GameConfig::BASE_W * scale, GameConfig::BASE_H * scale };

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(canvas.texture, src, dst, {0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    RM::get().Unload();
    CloseWindow();

    return EXIT_SUCCESS;
}