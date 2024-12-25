#include "Enemyporcupine.h"
#include "Map.h"
#include "Character.h"


EnemyPorcupine::EnemyPorcupine(float x, float y) : speed(20.0f) {
    if (!texture.loadFromFile("porcuine-finish.png")) {
        throw std::runtime_error("Failed to load timonfinish.png");
    }
    sprite.setTexture(texture);

    for (int i = 0; i < 6; i++) {
        frames1.emplace_back(i * 25 + 1, 0, 25, 20);
    }
    
    shape.setSize(sf::Vector2f(53.0f, 20.0f));  
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red);
    sprite.setScale(2.5f, 2.5f);

    
    velocity.x = speed;
    velocity.y = 0;
    frameTime = 0.2f;
}

void EnemyPorcupine::update(Map& map, float deltaTime) {
    
    shape.move(velocity * deltaTime);

    sprite.setPosition(shape.getPosition());

    
    updateAnimation(deltaTime);

    
    for (const auto& block : map.getBlocks()) {
        
        int x = static_cast<int>(block.getPosition().x / block.getSize().x); 
        int y = static_cast<int>(block.getPosition().y / block.getSize().y); 

        
        

        
        if (map.getLayout()[y][x] == '0') {
            
            if (shape.getGlobalBounds().intersects(block.getGlobalBounds())) {
                
                velocity.x = -velocity.x;

                
                if (velocity.x <= 0) {
                    sprite.setScale(-2.5f, 2.5f); 
                    sprite.setOrigin(25.f, 0);
                }
                else {
                    sprite.setScale(2.5f, 2.5f); 
                    sprite.setOrigin(0.f, 0);
                }

                
                shape.move(velocity.x * deltaTime, 0);

                
                break;
            }
        }

    }
}


void EnemyPorcupine::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= frameTime) {
        animationTimer -= frameTime;
        currentFrame = (currentFrame + 1) % frames1.size();
        sprite.setTextureRect(frames1[currentFrame]);
    }

}

void EnemyPorcupine::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    

}


bool EnemyPorcupine::checkCollision(Character& character) {
    if (shape.getGlobalBounds().intersects(character.shape.getGlobalBounds())) {
        
        character.onHitByEnemy(velocity);  
        return true;
    }
    return false;
}

