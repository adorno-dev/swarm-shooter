#include "ResourceManager.hpp"

#include "raylib.h"

#include "ResourceKeys.hpp"

#include <stdexcept>


void ResourceManager::Unload()
{
    for (auto& [name, tex] : _textures)
        UnloadTexture(tex);

    _textures.clear();

    for (auto& [name, img] : _images)
        UnloadImage(img);

    _images.clear();

    TraceLog(LOG_INFO, "ResourceManager: all resources unloaded.");
}

void ResourceManager::Load()
{
    ChangeDirectory(TextFormat("%s/../assets/images", GetApplicationDirectory()));

    loadTexture(RK::PLAYER, "survivor-idle_shotgun_0.png");
    loadTexture(RK::GAME_BG, "Floor.png");
    loadTexture(RK::GAME_FG, "Walls.png");
    loadTexture(RK::BULLET, "bullet.png");

    loadTexture(RK::COCKROACH_MOVE, "cockroach-move.png");
    loadTexture(RK::COCKROACH_DEATH, "cockroach-death.png");
    loadTexture(RK::SCORPION_MOVE, "scorpion-move.png");
    loadTexture(RK::SCORPION_DEATH, "scorpion-death-0.png");
    loadTexture(RK::KLIVER_MOVE, "kliver-move.png");
    loadTexture(RK::KLIVER_DEATH, "kliver-death.png");

    loadTexture(RK::HEALTH_POTION, "healthPotion.png");
    loadTexture(RK::BOOK_1, "book1.png");
    loadTexture(RK::BOOK_2, "book2.png");
    loadTexture(RK::BOOK_3, "book3.png");
    loadTexture(RK::BOOK_4, "book4.png");

    loadImage(RK::GAME_BG_COLLISION, "gameBgCollision.png");

    TraceLog(LOG_INFO, "ResourceManager: loaded %d textures", (int)_textures.size());
    TraceLog(LOG_INFO, "ResourceManager: loaded %d images", (int)_images.size());
}

void ResourceManager::loadTexture(const std::string& name, const std::string& path)
{
    Texture2D tex = LoadTexture(path.c_str());

    if (tex.id == 0)
        throw std::runtime_error("Failed to load texture: " + path);
    
        _textures.emplace(name, std::move(tex));
}

void ResourceManager::loadImage(const std::string& name, const std::string& path)
{
    Image img = LoadImage(path.c_str());

    if (img.data == nullptr)
        throw std::runtime_error("Failed to load image: " + path);
    
        _images.emplace(name, std::move(img));
}

const Texture2D& ResourceManager::GetTexture(const std::string& name) const
{
    auto it = _textures.find(name);
    if (it == _textures.end())
        throw std::runtime_error("Texture not found: '" + name + "'");
    return it->second;
}

const Image& ResourceManager::GetImage(const std::string& name) const
{
    auto it = _images.find(name);
    if (it == _images.end())
        throw std::runtime_error("Image not found: '" + name + "'");
    return it->second;
}