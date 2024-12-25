
#include "Character.h"
#include <iostream>
#include "Map.h"




const float blockWidth = 50.0f;
const float blockHeight = 50.0f;

Character::Character(float width, float height):  lives(3), isHitAnimation(false), isGame_Over(false) {

    
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(0, 0 - height);
    legs.setSize(sf::Vector2f(width, 10)); 
    legs.setPosition(shape.getPosition().x, shape.getPosition().y + height); 
}

void Character::move(float dx, Map& map) {
    
        shape.move(dx, 0);  
        handleHorizontalCollisions(map); 
 
}

void Character::takeDamage() {
    if (!isHit) {
        if (!damageSoundBuffer.loadFromFile("song_hit.ogg"))
            throw std::runtime_error("Failed to load damage sound");
        damageSound.setBuffer(damageSoundBuffer);
        damageSound.play();
        lives--; 
        isHit = true;
        hitTimer = invulnerabilityTime;

        
        if (lives <= 0) {

        }
    }
}


void Character::onHitByEnemy(sf::Vector2f enemyDirection) {
    if (!isHit) { 
        takeDamage();
        if (lives > 0) {
            
            velocity.x = (enemyDirection.x > 0) ? -500.0f : 500.0f;
            velocity.y = -100.0f;
            shape.setPosition(shape.getPosition().x + velocity.x, shape.getPosition().y + velocity.y);
            isHitAnimation = true;
        }
    }
}

void Character::jump() {
    if (isOnGround) {
        velocityY = jumpVelocity;
        isOnGround = false;
    }
}

void Character::checkCollisionsWithEnemies(Map& map) {
    for (EnemyPorcupine& porcupine : map.getEnemiesP()) {  
        if (porcupine.checkCollision(*this)) {
            
            onHitByEnemy(porcupine.getVelocity());  
        }
    }for (EnemyHyena& hyena : map.getEnemiesH()) {
        if (hyena.checkCollisionWithTimon(*this)) {
            onHitByEnemy(hyena.getPosition());  
        }
        if (hyena.checkCollisionWithPumba(*this)) {
            onHitByEnemy(hyena.getPosition());  
        }
    }

}

void Character::update(Map& map, float deltaTime) {
    velocityY += gravity;
    shape.move(0, velocityY);
    handleVerticalCollisions(map);
    if (isHit) {
        hitTimer -= deltaTime; 
        if (hitTimer <= 0) {
            isHit = false; 
            hitTimer = 0.0f;
            isHitAnimation = false;
        }
    }
    checkCollisionsWithEnemies(map);
}

void Character::updateLegs() {
    legs.setPosition(shape.getPosition().x, shape.getPosition().y + shape.getSize().y); 
}



void Character::handleHorizontalCollisions(Map& map) {
    for (size_t row = 0; row < map.getLayout().size(); ++row) {
        for (size_t col = 0; col < map.getLayout()[row].size(); ++col) {
            char cell = map.getLayout()[row][col];
            if (cell == '#' || cell == 'g' || cell == 'k' || cell == '0' || cell == 'r' || cell == 'i' || cell == 'l') {
                sf::FloatRect blockBounds(col * blockWidth, row * blockHeight, blockWidth, blockHeight);
                if (shape.getGlobalBounds().intersects(blockBounds)) {
                    if (shape.getPosition().x + shape.getSize().x > blockBounds.left &&
                        shape.getPosition().x < blockBounds.left) {
                        shape.setPosition(blockBounds.left - shape.getSize().x, shape.getPosition().y);
                    }
                    else if (shape.getPosition().x < blockBounds.left + blockWidth &&
                        shape.getPosition().x + shape.getSize().x > blockBounds.left + blockWidth) {
                        shape.setPosition(blockBounds.left + blockWidth, shape.getPosition().y);
                    }
                }
            }
        }
    }
}

void Character::handleVerticalCollisions(Map& map) {
    
    for (size_t row = 0; row < map.getLayout().size(); ++row) {
        for (size_t col = 0; col < map.getLayout()[row].size(); ++col) {
            char cell = map.getLayout()[row][col];
            if (cell == '#' || cell == 'g' || cell == 'k' || cell == '0' || cell == 'r' || cell == 'i' || cell == 'l') {
                sf::FloatRect blockBounds(col * blockWidth, row * blockHeight, blockWidth, blockHeight);
                if (shape.getGlobalBounds().intersects(blockBounds)) {
                    if (velocityY > 0 && shape.getPosition().y + shape.getSize().y > blockBounds.top) {
                        shape.setPosition(shape.getPosition().x, blockBounds.top - shape.getSize().y);
                        velocityY = 0;
                        isOnGround = true; 
                    }
                    else if (velocityY < 0 && shape.getPosition().y < blockBounds.top + blockHeight) {
                        shape.setPosition(shape.getPosition().x, blockBounds.top + blockHeight);
                        velocityY = 0;
                    }
                }
            }
        }
    }
}


