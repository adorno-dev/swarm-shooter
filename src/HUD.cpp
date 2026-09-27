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
    float yPosition = (BAR_HEIGHT - scaledH) * 0.5f;
    float step = scaledW + 10.0f;

    for (int i = 0; i < GameConfig::PLAYER_MAX_HEALTH; i++)
    {
        _lifeDst.push_back({
            GameConfig::BASE_W - step * (i + 1), yPosition, scaledW, scaledH
        });
    }
}

void HUD::Draw(const HUDInfo& info) const
{
    DrawRectangle(0, 0, GameConfig::BASE_W, BAR_HEIGHT, ColorAlpha(DARKBLUE, 0.8f));

    Color waveColor = WHITE;

    int fontSize = 24;

    const char* waveText = nullptr;

    if (info.waveRunning)
    {
        waveColor = info.waveTime < 8.0f ? RED : WHITE;
        
        waveText = TextFormat("Wave %d %0.1fs | Books: %d/%d", 
            info.wave, 
            info.waveTime, 
            info.booksTotal - info.booksAlive, 
            info.booksTotal);
    }
    else if (info.wave > 0) waveText = TextFormat("Wave %d Completed!   +%d", info.wave, info.lastWaveBonus);

    if (waveText)
        DrawCenteredText(waveText, 18, fontSize, waveColor);

    for (int i = 0; i < _player->GetHealth(); i++)
        DrawTexturePro(*_lifeTex, _lifeSrc, _lifeDst[i], {0,0}, 0.0f, WHITE);
}