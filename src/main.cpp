#include <algorithm>
#include "raylib.h"

#define EXIT_SUCCESS 0

constexpr int BASE_W = 1280;
constexpr int BASE_H = 720;

int main()
{
    float scale;
    float offsetX;
    float offsetY;

    float dt;

    float moveX;
    float moveY;

    float mapW;
    float mapH;

    float halfW;
    float halfH = BASE_H * 0.5f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(BASE_W, BASE_H, "Swarm");
    SetTargetFPS(60);
    
    // Vector2 pos = GetMonitorPosition(0);
    // SetWindowSize(pos.x + 320, pos.y + 180);
    // SetWindowSize(1920, 1080);
    
    ChangeDirectory(TextFormat("%s/../assets/images", GetApplicationDirectory()));

    Texture2D background = LoadTexture("Floor.png");
    Texture2D walls = LoadTexture("Walls.png");
    // Texture2D playerTexture = LoadTexture("survivor-idle_shotgun_0.png");

    RenderTexture2D canvas = LoadRenderTexture(BASE_W, BASE_H);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);

    mapW = (float)background.width;
    mapH = (float)background.height;

    halfW = BASE_W * 0.5f;
    halfH = BASE_H * 0.5f;

    Camera2D camera = {};
    camera.zoom = 1.0f;
    camera.target = {400.0f, 400.0f};
    camera.offset = { halfW, halfH };

    const char* cameraLabelText;

    while (!WindowShouldClose())
    {
        dt = GetFrameTime();
        moveX = (IsKeyDown(KEY_D) ? 1.0f : 0.0f) - (IsKeyDown(KEY_A) ? 1.0f : 0.0f);
        moveY = (IsKeyDown(KEY_S) ? 1.0f : 0.0f) - (IsKeyDown(KEY_W) ? 1.0f : 0.0f);
        camera.target.x += dt * 300.0f * moveX;
        camera.target.y += dt * 300.0f * moveY;
        camera.target.x = std::clamp(camera.target.x, halfW, mapW - halfW);
        camera.target.y = std::clamp(camera.target.y, halfH, mapH - halfH);
        cameraLabelText = TextFormat("Camera: %.0f, %.0f", camera.target.x, camera.target.y);

        BeginTextureMode(canvas);
            ClearBackground(BLACK);
            BeginMode2D(camera);
                DrawTexture(background, 0, 0, WHITE);
                // DrawTextureEx(playerTexture, {400.0f, 400.0f}, 0.0f, 0.3f, WHITE);
                DrawTexture(walls, 0, 0, WHITE);
            EndMode2D();
            DrawRectangle(0, BASE_H - 32, BASE_W, 32, ColorAlpha(DARKBLUE, 0.6f));
            DrawText(cameraLabelText, 12, BASE_H - 24, 20, LIME);
        EndTextureMode();

        scale = std::min(
            (float)GetScreenWidth() / BASE_W,
            (float)GetScreenHeight() / BASE_H
        );

        offsetX = (GetScreenWidth() - BASE_W * scale) * 0.5f;
        offsetY = (GetScreenHeight() - BASE_H * scale) * 0.5f;

        Rectangle src = { 0, 0, (float)BASE_W, -(float)BASE_H };
        Rectangle dst = { offsetX, offsetY, BASE_W * scale, BASE_H * scale };

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(canvas.texture, src, dst, {0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(walls);
    // UnloadTexture(playerTexture);
    UnloadRenderTexture(canvas);
    CloseWindow();

    return EXIT_SUCCESS;
}