#ifndef ENEMY_HYENA_H
#define ENEMY_HYENA_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>

class Map;
class Character;

class EnemyHyena {
public:
    enum AnimationState {
        Idle,               
        Chase,              
        Death           
    };
    AnimationState state;

    sf::Vector2f velocity; 
    EnemyHyena(float x, float y);
    void setPosition(float x, float y) {
        shape.setPosition(x, y);
        sprite.setPosition(x, y);      }
    sf::Vector2f spriteOffset{ 0.f, 30.f }; 
    void update(Map& map, Character& character, float deltaTime);
    void draw(sf::RenderWindow& window);
    void playDeathAnimation(float deltaTime);
    void reset(float x, float y);
    bool checkCollisionWithPumba(Character& pumba);
    bool checkCollisionWithTimon(Character& timon);
    bool checkCollisionWithPumbaHit(Character& pumba);
    sf::Vector2f getPosition() const {
        return shape.getPosition();      }
    
private:
    sf::Sound hitSound;
    sf::SoundBuffer hitSoundBuffer;

    sf::RectangleShape shape;
    sf::Sprite sprite;
    sf::Texture texture;
    std::vector<sf::IntRect> frames;
    std::vector<sf::IntRect> chaseFrames;
    std::vector<sf::IntRect> deathFrames;
    int currentFrame;
    float animationTimer;
    float frameTime;                 
    float speed = 20.0f;     
    float speed1 = 50.0f;
    bool isDead;                     
    void updateAnimation(float deltaTime);
    void checkCollisionWithMap(Map& map, float deltaTime, bool isInRadius);

    bool isCharacterInRadius(Character& character, float radius);
};

#endif #pragma once
