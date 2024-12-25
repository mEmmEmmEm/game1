#ifndef TIMON_H
#define TIMON_H


#include "Character.h"

class EnemyHyena;

class Timon : public Character {
public:
    Timon();
    void reset();
    void updateAnimation_victory(float deltaTime);
    sf::Sprite sprite;
    sf::Texture texture;
    std::vector<sf::IntRect> frames;
    std::vector<sf::IntRect> gblock;
    std::vector<sf::IntRect> die;
    std::vector<sf::IntRect> victory;
    std::vector<sf::IntRect> collisionFrames;
    bool wasCrawling = false;
    bool isColliding = false;
    bool isCollidingRight = false;
    bool isCollidingLeft = false;
    bool isJumping = false;
    bool isMoving = false;
    float animationTimer = 0.0f;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float frameTime1 = 100.0f;
    float collisionFrameTime = 0.2f;
    int collisionDirection = 0;
    void move(float dx, Map& map) override;
    void jump() override;
    void update(Map& map, float deltaTime);
    void setColliding(bool colliding);
    void updateAnimation_die(float deltaTime);
    void checkGameOver(bool& isGameOver);
    void toggleCrawl();
    int ismove = 0;
    bool flag = false;
    bool checkBlockAbove(Map& map);
    
    
    void onHitByEnemy(sf::Vector2f enemyDirection);
private:
    bool wasLookingLeft = false;
    float crawlOffset = 0.f;
    bool isHit = false;
    bool crawlKeyPressed = false;
     
    sf::Vector2f originalSize; 
    sf::Vector2f crawlSize; 

    void updateAnimation(float deltaTime, Map& map);
};

#endif 

