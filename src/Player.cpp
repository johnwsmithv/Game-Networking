#include "Player.hpp"

Player::Player(int health, Location location)
    : health(health), location(location), currentGameId(-1)
{}

int Player::getPlayerHealth() const
{
    return this->health;
}

int Player::getCurrentGameId() const {
    return this->currentGameId;
}

Location Player::getPlayerLocation() const
{
    return this->location;
}

void Player::updatePlayerXLocation(const int x)
{
    this->location.x += x;
}

void Player::updatePlayerYLocation(const int y)
{
    this->location.y += y;
}

void Player::updatePlayerZLocation(const int z)
{
    this->location.z += z;
}

void Player::setPlayerXLocation(const int x)
{
    this->location.x = x;
}

void Player::setPlayerYLocation(const int y)
{
    this->location.y = y;
}

void Player::setPlayerZLocation(const int z)
{
    this->location.z = z;
}

void Player::setPlayerHealth(const int health)
{
    this->health = health;
}

void Player::setGameId(const int currentGameId) {
    this->currentGameId = currentGameId;
}