#ifndef PUMBA_H
#define PUMBA_H


#include "Character.h"
class Game;


class Pumba : public Character {
public:
    Pumba();
    sf::Sprite sprite;
    sf::Texture texture;
    std::vector<sf::IntRect> frames;
    std::vector<sf::IntRect> collisionFrames;
    std::vector<sf::IntRect> die;
    std::vector<sf::IntRect> victory;
    void updateAnimation_victory(float deltaTime);
    bool isStone = false;
    bool isColliding = false;
    bool isCollidingRight = false;
    bool isCollidingLeft = false;
    bool isJumping = false;
    bool isMoving = false;
    float animationTimer = 0.0f;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float collisionFrameTime = 0.2f;
    int collisionDirection = 0;
    void checkGameOver(bool& isGameOver);
    void updateAnimation_die(float deltaTime);
    bool flag = false;
    void reset();

    void move(float dx, Map& map) override;
    void jump() override;
    void update(Map& map, float deltaTime);
    void setColliding(bool colliding);

private:
    
    void updateAnimation(float deltaTime);

};

#endif 

