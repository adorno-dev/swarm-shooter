#include "DebugOverlay.hpp"

#include "raylib.h"

#include "Player.hpp"
#include "BulletManager.hpp"
#include "EnemyManager.hpp"
#include "GameConfig.hpp"
#include "GameInput.hpp"
#include "MovementState.hpp"


void DebugOverlay::Init
(
    const Player& player,
    const BulletManager& bulletManager,
    const EnemyManager& enemyManager,
    const Camera2D& camera2D
)
{
    _player = &player;
    _bullets = &bulletManager;
    _enemies = &enemyManager;
    _camera = &camera2D;
}

void DebugOverlay::Draw() const
{
    if (!GameConfig::SHOW_DEBUG) return;

    int boxY = GameConfig::BASE_H - 110;
    int lineGap = 24;
    int fontSize = 20;
    int textX = 10;

    const MovementState& input = GI::get().State();

    DrawRectangle(0, boxY - 10, 400, 120, ColorAlpha(BLACK, 0.7f));

    DrawText(TextFormat("Camera: %.0f,%.0f", 
        _camera->target.x, _camera->target.y), textX, boxY, fontSize, YELLOW);

    boxY += lineGap;

    DrawText(TextFormat("HP: %d,%d",
         _player->GetHealth(), _player->GetMaxHealth()), textX, boxY, fontSize, YELLOW);

    boxY += lineGap;

    DrawText(TextFormat("Move: %.1f,%.1f Aim: %.1f", 
        input.moveDir.x, input.moveDir.y, input.aimAngle), textX, boxY, fontSize, YELLOW);

    boxY += lineGap;

    DrawText(TextFormat("Bullets: %d/%d Enemies: %d/%d", 
        _bullets->CountAlive(), _bullets->GetPoolTotal(), 
        _enemies->CountAlive(), _enemies->GetPoolTotal()), textX, boxY, fontSize, YELLOW);
}