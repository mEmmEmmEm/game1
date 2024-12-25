#include "Pumba.h"
#include "Map.h"




Pumba::Pumba() : Character(45*3, 45*3){
    if (!texture.loadFromFile("pumbafinish.png")) {
        throw std::runtime_error("Failed to load pumbafinish.png");
    }
    sprite.setTexture(texture);
    isPumba = true;
    for (int i = 0; i < 6; i++) {   
        frames.emplace_back(i * 55 - 2.5, 0, 55, 55);
    }
    for (int i = 0; i < 2; i++) {
        collisionFrames.emplace_back(i * 55, 220, 55, 55); 
    }
    for (int i = 0; i < 21; i++) {
        die.emplace_back(i * 60, 275, 55, 55);
    }
    for (int i = 0; i < 24; i++) {
        victory.emplace_back(i * 55, 332, 55, 55);
    }

    sprite.setTextureRect(frames[0]);
    sprite.setPosition(shape.getPosition().x, shape.getPosition().y - 18.f);
    sprite.setScale(3.0f, 3.0f);
}

void Pumba::move(float dx, Map& map) {
    dx *= 2.f;
    shape.move(dx, 0); 
    isColliding = false;

    if (map.interactWithCharacter(shape, dx, true, false)) { 
        
        shape.move(-dx, 0); 
    }
    collisionDirection = 0; 

    sf::FloatRect bounds = shape.getGlobalBounds();
    sf::Vector2f position = shape.getPosition();
    int x = static_cast<int>(position.x / 50);
    int y = static_cast<int>(position.y / 50);

    
    if (y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == '#' || y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == 'g' || y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == 'r' || y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == '0') {
        isColliding = true;
        collisionDirection = (dx > 0) ? 1 : -1; 
        shape.move(-dx, 0); 
    }

    if (dx != 0 && !isColliding) { 
        isMoving = true;
        Character::move(dx, map); 
        sprite.setScale((dx < 0) ? -3.f : 3.f, 3.f); 
        sprite.setOrigin((dx < 0) ? 55.f : 0, 0);
    }
    else {
        isMoving = false;
    }

    sprite.setPosition(shape.getPosition()); 
}

void Pumba::reset() {
    lives = 3;
    shape.setPosition(1000, 650 - shape.getSize().y + 500);
    
    
    velocity = sf::Vector2f(0, 0);
    flag = false;
    sprite.setTextureRect(frames[0]);
}

void Pumba::jump() {
    if (isOnGround) {
        Character::jump();
        isJumping = true;
    }
}

void Pumba::update(Map& map, float deltaTime) {
    Character::update(map, deltaTime);
    if (isOnGround) {
        isJumping = false;
    }
    updateAnimation(deltaTime);
    sprite.setPosition(shape.getPosition().x, shape.getPosition().y - 18.f);
}

void Pumba::checkGameOver(bool& isGameOver) {
    if (lives <= 0 && !isGameOver) {
        isGameOver = true;
        
    }
}

void Pumba::updateAnimation_die(float deltaTime) {
    sprite.setPosition(shape.getPosition().x, shape.getPosition().y);
    
    if (flag) {
        return;  
    }

    
    animationTimer += deltaTime;

    
    while (animationTimer >= frameTime) {
        animationTimer -= frameTime;  
        currentFrame++;               

        
        if (currentFrame >= die.size()) {
            currentFrame = die.size() - 1;  
            flag = true;                    
            break;
        }

        
        sprite.setTextureRect(die[currentFrame]);
    }

    
    if (currentFrame < die.size() && sprite.getTextureRect() != die[currentFrame]) {
        sprite.setTextureRect(die[currentFrame]);
    }
}

void Pumba::updateAnimation_victory(float deltaTime) {


    
    if (flag) {
        return;  
    }

    
    animationTimer += deltaTime;

    
    while (animationTimer >= frameTime) {
        animationTimer -= frameTime;  
        currentFrame++;               

        
        if (currentFrame >= victory.size()) {
            currentFrame = victory.size() - 1;  
            flag = true;                        
            break;
        }

        
        sprite.setTextureRect(victory[currentFrame]);
    }

    
    if (currentFrame < victory.size() && sprite.getTextureRect() != victory[currentFrame]) {
        sprite.setTextureRect(victory[currentFrame]);
    }
}

void Pumba::setColliding(bool colliding) {
    isColliding = colliding;
}

void Pumba::updateAnimation(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        animationTimer += deltaTime;
        if (animationTimer >= collisionFrameTime) {
            animationTimer -= collisionFrameTime;
            currentFrame = (currentFrame + 1) % collisionFrames.size();
            sprite.setTextureRect(collisionFrames[currentFrame]);
        }
        if (collisionDirection < 0) { 
            sprite.setScale(-3.f, 3.f);
            sprite.setOrigin(55.f, 0);
        }
        else if (collisionDirection > 0) { 
            sprite.setScale(3.f, 3.f);
            sprite.setOrigin(0, 0);
        }
    }
    
 
    else if (isJumping) {
        sprite.setTextureRect(sf::IntRect(0, 55, 55, 55));
    }
    else if (!isOnGround) {
        sprite.setTextureRect(sf::IntRect(0, 110, 55, 55)); 
    }
    else if (isHitAnimation) {
        sprite.setTextureRect(sf::IntRect(0, 165, 55, 55));
    }
    else if (isMoving) {
        animationTimer += deltaTime;
        if (animationTimer >= frameTime) {
            animationTimer -= frameTime;
            currentFrame = (currentFrame + 1) % frames.size();
            sprite.setTextureRect(frames[currentFrame]);
        }
    }
    else {
        currentFrame = 0;
        sprite.setTextureRect(frames[currentFrame]);
    }
}

