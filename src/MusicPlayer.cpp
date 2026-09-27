#include "MusicPlayer.hpp"
#include "ResourceManager.hpp"


void MusicPlayer::Init(const std::string& key)
{
    _track = &RM::get().GetMusic(key);
}

void MusicPlayer::Start(float volume)
{
    if (!_track) return;

    SetMusicVolume(*_track, volume);
    PlayMusicStream(*_track);
    _playing = true;
}

void MusicPlayer::Update()
{
    if (!_playing || !_track) return;
    UpdateMusicStream(*_track);
}

void MusicPlayer::Stop()
{
    if (!_playing || !_track) return;
    StopMusicStream(*_track);
    _playing = true;
}