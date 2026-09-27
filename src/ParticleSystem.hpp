#pragma once
#include "raylib.h"

#include <vector>


struct Particle
{
    Vector2 position = { 0.0f, 0.0f };
    Vector2 velocity = { 0.0f, 0.0f };
    float lifetime = 0.0f;
    float maxLifetime = 0.0f;
    float scale = 0.0f;
    bool active = false;
};

struct ParticleConfig
{
    const char* textureKey = nullptr;

    int count = 1;

    float speedMin = 80.0f;
    float speedMax = 200.0f;

    float lifetimeMin = 0.3f;
    float lifetimeMax = 1.5f;

    float scaleMin = 0.3;
    float scaleMax = 1.0f;

    Color tint = WHITE;

    float drag = 2.5f;
    
    bool fadeOut = false;
};

class ParticleSystem
{
public:
    void Init(const ParticleConfig& config);
    void Update(float delta);
    void Draw() const;
    void Emit(Vector2 origin);
    bool IsIdle() const;

private:
    const Texture2D* _texture = nullptr;
    ParticleConfig _config;
    std::vector<Particle> _pool;

    static constexpr int POOL_SIZE = 64;
};