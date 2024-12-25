
#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <SFML/Audio.hpp>


class Character {
public:

    sf::RectangleShape shape;
    sf::RectangleShape legs;
    float gravity = 0.5f;
    float jumpVelocity = -10.0f;
    float velocityY = 0;
    bool isOnGround = false;
    Character(float width, float height);
    virtual void move(float dx, Map& map);
    virtual void jump();
    void checkCollisionsWithEnemies(Map& map);
    virtual void update(Map& map, float deltaTime);
    void updateLegs();
    sf::Vector2f velocity;     
    void onHitByEnemy(sf::Vector2f enemyDirection);
    int lives = 3;     
    void takeDamage();     
    sf::Vector2f getPosition() const {
        return shape.getPosition();     }
    bool isGame_Over;
    bool isCrawling = false;
    bool isPumba = true;
protected:
        bool isHitAnimation = false;
    void handleHorizontalCollisions(Map& map);
    void handleVerticalCollisions(Map& map);

private:
    sf::Sound damageSound;
    sf::SoundBuffer damageSoundBuffer;
    
    bool isHit = false;     
    float invulnerabilityTime = 0.5f;     
    float hitTimer = 0.0f; };

#endif 