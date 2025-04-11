#pragma once

struct Location {
    int x;
    int y;
    int z;

    Location(int x, int y, int z)
        : x(x), y(y), z(z)
    {}
};

class Player {
public:
    Player(int health, Location location);

    int getPlayerHealth() const;
    Location getPlayerLocation() const;

    void updatePlayerXLocation(const int x);
    void updatePlayerYLocation(const int y);
    void updatePlayerZLocation(const int z);

    void setPlayerXLocation(const int x);
    void setPlayerYLocation(const int y);
    void setPlayerZLocation(const int z);

    void setPlayerHealth(const int health);
private:
    int health;
    Location location;
};