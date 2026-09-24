#include "Player.hpp"
#include "GameConfig.hpp"
#include "GameInput.hpp"

Player::Player(const std::string& textureName)
{
    _sprite.Init(textureName);
    _sprite.pivot = GameConfig::PLAYER_PIVOT;
    _transform.scale = GameConfig::PLAYER_SCALE;
    _movement.speed = GameConfig::PLAYER_SPEED;
}

void Player::Update(float delta)
{
    _movement.Update(_transform, GI::get().State(), delta, _collisionMap);
}

void Player::SetPosition(Vector2 position)
{
    _transform.position = position;
}

void Player::SetCollisionMap(const CollisionMap* collisionMap)
{
    _collisionMap = collisionMap;
}

Vector2 Player::GetPosition() const
{
    return _transform.position;
}

void Player::Draw() const
{
    _sprite.Draw(_transform);
}