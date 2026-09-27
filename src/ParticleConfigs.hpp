#pragma once
#include "raylib.h"

#include "ParticleSystem.hpp"
#include "ResourceKeys.hpp"


constexpr ParticleConfig SPAWN_PUFF = {
    RK::PARTICLE_DOT,
    32,
    30.0f, 80.0f,
    0.5f, 0.8f,
    0.4f, 0.6f,
    SKYBLUE,
    1.0f,
    true
};

constexpr ParticleConfig DEATH_PUFF = {
    RK::PARTICLE_DOT,
    24,
    120.0f, 150.0f,
    0.25f, 0.4f,
    0.6f, 0.9f,
    ORANGE,
    3.0f,
    true
};