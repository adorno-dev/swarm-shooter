#include "ParticleSystem.hpp"

#include "raylib.h"
#include "raymath.h"

#include "ResourceManager.hpp"
#include "SwarmUtils.hpp"


void ParticleSystem::Init(const ParticleConfig& config)
{
    _config = config;
    _texture = &RM::get().GetTexture(config.textureKey);
    _pool.resize(POOL_SIZE);
}

void ParticleSystem::Update(float delta)
{
    for (auto& p : _pool)
    {
        if (!p.active) continue;

        p.position.x += p.velocity.x * delta;
        p.position.y += p.velocity.y * delta;

        float dragFactor = 1.0f - _config.drag * delta;
        if (dragFactor < 0.0f) dragFactor = 0.0f;

        p.velocity.x *= dragFactor;
        p.velocity.y *= dragFactor;

        p.lifetime += delta;

        if (p.lifetime > p.maxLifetime)
            p.active = false;
    }
}

void ParticleSystem::Draw() const
{
    for (auto& p : _pool)
    {
        if (!p.active) continue;

        const float w = static_cast<float>(_texture->width) * p.scale;
        const float h = static_cast<float>(_texture->height) * p.scale;

        Rectangle src = { 
            0.0f, 
            0.0f, 
            static_cast<float>(_texture->width),
            static_cast<float>(_texture->height)
        };

        Rectangle dst = { 
            p.position.x, 
            p.position.y, 
            w, 
            h 
        };

        Vector2 origin = { 
            w * 0.5f, 
            h * 0.5f 
        };

        Color colour = _config.tint;
        if (_config.fadeOut)
        {
            float t = p.lifetime / p.maxLifetime;
            float alpha = (t < 0.7f) ? 1.0f : (1.0f - t) / 0.3f;
            colour = ColorAlpha(colour, alpha);
        }
        
        DrawTexturePro(*_texture, src, dst, origin, 0.0f, colour);
    }
}

void ParticleSystem::Emit(Vector2 origin)
{
    int spawned = 0;

    float angle, speed;

    for (auto& p : _pool)
    {
        if (spawned >= _config.count) break;
        if (p.active) continue;

        speed = RandomFloat(_config.speedMin, _config.speedMax);
        angle = RandomFloat(0.0f, 360.0f);

        p.position = origin;
        p.lifetime = 0.0f;
        p.maxLifetime = RandomFloat(_config.lifetimeMin, _config.lifetimeMax);
        p.scale = RandomFloat(_config.scaleMin, _config.scaleMax);
        p.velocity = Direction(angle) * speed;
        p.active = true;
        spawned++;
    }
}

bool ParticleSystem::IsIdle() const
{
    for (const auto& p : _pool)
        if (p.active)
            return false;

    return true;
}