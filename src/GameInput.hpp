#pragma once
#include "raylib.h"

#include "MovementState.hpp"


class GameInput
{
public:

    static GameInput& instance()
    {
        static GameInput _instance;
        return _instance;
    }

    GameInput(const GameInput&) = delete;
    GameInput& operator=(const GameInput&) = delete;

    const MovementState& State() const { return _state; }

    void Update();

private:
    GameInput() = default;
    MovementState _state = {};
};

// GI::get()
namespace GI
{
    inline GameInput& get() { return GameInput::instance(); }
}