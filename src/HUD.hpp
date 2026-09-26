#pragma once

#include "raylib.h"

#include <vector>


class Player;


class HUD
{
public:
    void Init(const Player& player);
    void Draw(int wave, float waveTime, bool waveRunning, int booksAlive, int booksTotal) const;

private:
    static constexpr int BAR_HEIGHT = 64;
    const Player* _player = nullptr;
    const Texture2D* _lifeTex = nullptr;
    Rectangle _lifeSrc = {};
    std::vector<Rectangle> _lifeDst;
};