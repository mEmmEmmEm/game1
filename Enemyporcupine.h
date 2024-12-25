#ifndef ENEMY_PORCUPINE_H
#define ENEMY_PORCUPINE_H


#include <SFML/Graphics.hpp>

class Map;
class Character;


class EnemyPorcupine {
public:
    EnemyPorcupine();
    EnemyPorcupine(float x, float y);
    void setPosition(float x, float y) {
        shape.setPosition(x, y);
        sprite.setPosition(x, y);      }
    void update(Map& map, float deltaTime);

    void updateAnimation(float deltaTime);

    void draw(sf::RenderWindow& window);

    bool checkCollision(Character& character);
    sf::Vector2f velocity;

    sf::Vector2f getVelocity() const { return velocity; }

private:
   

    sf::RectangleShape shape;
    sf::Sprite sprite; 
    sf::Texture texture; 
    std::vector<sf::IntRect> frames1; 
    int currentFrame; 
    float animationTimer;    
    float frameTime; 
    float speed;
};

#endif #pragma once
