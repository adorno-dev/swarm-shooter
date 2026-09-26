#include "HUD.hpp"

#include "raylib.h"

#include "ResourceKeys.hpp"
#include "ResourceManager.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"


void HUD::Init(const Player& player)
{
    float iconScale = 0.2f;

    _player = &player;

    _lifeTex = &RM::get().GetTexture(RK::PLAYER);
    _lifeSrc = { 0, 0, (float)_lifeTex->width, (float)_lifeTex->height };

    float scaledW = _lifeTex->width * iconScale;
    float scaledH = _lifeTex->height * iconScale;
    float yPosition = (BAR_HEIGHT - scaledH);
    float step = scaledW + 10.0f;

    for (int i = 0; i < GameConfig::PLAYER_MAX_HEALTH; i++)
    {
        _lifeDst.push_back({
            GameConfig::BASE_W - step * (i + 1), yPosition, scaledW, scaledH
        });
    }
}

void HUD::Draw(int wave, float waveTime, bool waveRunning, int booksAlive, int booksTotal) const
{
    DrawRectangle(0, 0, GameConfig::BASE_W, BAR_HEIGHT, ColorAlpha(DARKBLUE, 0.8f));

    Color waveColor = WHITE;

    int fontSize = 24;

    const char* waveText = nullptr;

    if (waveRunning)
    {
        waveColor = waveTime < 8.0f ? RED : WHITE;
        
        waveText = TextFormat("Wave %d %0.1fs | Books: %d/%d", 
            wave, waveTime, booksTotal - booksAlive, booksTotal);
    }
    else if (wave > 0) waveText = TextFormat("Wave %d Completed!", wave);

    if (waveText)
    {
        int textW = MeasureText(waveText, fontSize);

        DrawText(waveText, GameConfig::HALF_BASE_W - textW / 2, 18, fontSize, waveColor);
    }

    for (int i = 0; i < _player->GetHealth(); i++)
        DrawTexturePro(*_lifeTex, _lifeSrc, _lifeDst[i], {0,0}, 0.0f, WHITE);
}