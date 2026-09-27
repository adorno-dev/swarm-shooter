#pragma once
#include "raylib.h"

#include <string>


class MusicPlayer
{
public:
    void Init(const std::string& key);
    void Start(float volume = 0.5f);
    void Update();
    void Stop();

private:
    Music* _track = nullptr;
    bool _playing = false;
};