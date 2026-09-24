#include <algorithm>
#include "raylib.h"
#include "raymath.h"
#include "GameConfig.hpp"
#include "ResourceManager.hpp"
#include "ResourceKeys.hpp"
#include "GameInput.hpp"
#include "CollisionMap.hpp"
#include "Player.hpp"

int main()
{
    float scale;
    float offsetX;
    float offsetY;

    float dt;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GameConfig::BASE_W, GameConfig::BASE_H, "Swarm");
    SetTargetFPS(60);
    DisableCursor();

    RM::get().Load();
           
    const Texture2D& background = RM::get().GetTexture(RK::GAME_BG);
    
    CollisionMap collisionMap;
    collisionMap.Init(RK::GAME_BG_COLLISION);

    RenderTexture2D canvas = LoadRenderTexture(GameConfig::BASE_W, GameConfig::BASE_H);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);

    float mapW = (float)background.width;
    float mapH = (float)background.height;

    float halfW = GameConfig::BASE_W * 0.5f;
    float halfH = GameConfig::BASE_H * 0.5f;

    Player player(RK::PLAYER);
    player.SetPosition({mapW * 0.5f, mapH * 0.5f});
    player.SetCollisionMap(&collisionMap);

    Camera2D camera = {};
    camera.zoom = 1.0f;
    camera.target = player.GetPosition();
    camera.offset = { halfW, halfH };

    Rectangle src, dst;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F1))
            Sprite::showDebug = !Sprite::showDebug;

        GI::get().Update();

        dt = GetFrameTime();

        player.Update(dt);

        camera.target = player.GetPosition();

        camera.target.x = std::clamp(camera.target.x, halfW, mapW - halfW);
        camera.target.y = std::clamp(camera.target.y, halfH, mapH - halfH);
        
        BeginTextureMode(canvas);
            ClearBackground(BLACK);
            BeginMode2D(camera);
                DrawTexture(background, 0, 0, WHITE);
                player.Draw();
                DrawTexture(RM::get().GetTexture(RK::GAME_FG), 0, 0, WHITE);
            EndMode2D();

            DrawRectangle(0, GameConfig::BASE_H - 32, GameConfig::BASE_W, 32, ColorAlpha(DARKBLUE, 0.6f));

            DrawText(TextFormat("Player: %.0f,%.0f", player.GetPosition().x, player.GetPosition().y), 12, GameConfig::BASE_H - 24, 20, LIME);
            DrawText(TextFormat("Camera: %.0f,%.0f", camera.target.x, camera.target.y), 256, GameConfig::BASE_H - 24, 20, LIME);
            DrawText(TextFormat("Aim: %.1f", GI::get().State().aimAngle), 512, GameConfig::BASE_H - 24, 20, LIME);

        EndTextureMode();

        scale = std::min(
            (float)GetScreenWidth() / GameConfig::BASE_W,
            (float)GetScreenHeight() / GameConfig::BASE_H
        );

        offsetX = (GetScreenWidth() - GameConfig::BASE_W * scale) * 0.5f;
        offsetY = (GetScreenHeight() - GameConfig::BASE_H * scale) * 0.5f;

        src = { 0, 0, (float)GameConfig::BASE_W, -(float)GameConfig::BASE_H };
        dst = { offsetX, offsetY, GameConfig::BASE_W * scale, GameConfig::BASE_H * scale };

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