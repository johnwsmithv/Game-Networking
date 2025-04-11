#include "Player.hpp"

Player::Player(int health, Location location)
    : health(health), location(location)
{}

int Player::getPlayerHealth() const
{
    return this->health;
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
