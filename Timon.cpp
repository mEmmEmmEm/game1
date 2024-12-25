#include "Timon.h"

#include <iostream>
#include "EnemyHyena.h"



Timon::Timon() : Character(50, 100) {
    isPumba = false;
    isGame_Over = false;
    jumpVelocity = -12.0f;
    
    if (!texture.loadFromFile("timonfinish.png")) {
        throw std::runtime_error("Failed to load timonfinish.png");
    }
    sprite.setTexture(texture);

    for (int i = 0; i < 6; i++) {
        frames.emplace_back(i * 40 + 1, 0, 40, 50);
    }
    for (int i = 0; i < 2; i++) {
        collisionFrames.emplace_back(i * 45 + 4, 245, 45, 35); 
    }
    for (int i = 0; i < 21; i++) {
        die.emplace_back(i * 50 + 1, 405, 45, 35);
    }
    for (int i = 0; i < 24; i++) {
        victory.emplace_back(i * 45 + 1, 330, 45, 55);
    }
    for (int i = 0; i < 8; i++) {
        gblock.emplace_back(i * 50, 290, 50, 40);
    }
    originalSize = { 50.0f, 100.0f };
    crawlSize = { 100.0f, 50.0f };
    sprite.setTextureRect(frames[0]);
    sprite.setPosition(shape.getPosition());
    sprite.setScale(2.0f, 2.0f);
}

void Timon::toggleCrawl() {
    if (isCrawling) {
        ismove = 0;
        isCrawling = false;
        shape.setSize(originalSize);    
        sprite.setScale(2.0f, 2.0f);    
    }
    else {
        wasLookingLeft = (sprite.getScale().x < 0);
        isCrawling = true;
        shape.setSize(crawlSize);       
        sprite.setScale(2.0f, 2.0f);  
        sprite.setPosition(shape.getPosition());
        shape.setPosition(shape.getPosition().x, shape.getPosition().y + 50.f);
    }

    
    sprite.setPosition(shape.getPosition());
}

void Timon::reset() {
    lives = 3;
    shape.setSize(originalSize);
    shape.setPosition(1150, 650 - shape.getSize().y + 500);
    
    velocity = sf::Vector2f(0, 0);
    flag = false;
    isCrawling = false;
    sprite.setTextureRect(frames[0]);
}


void Timon::move(float dx, Map& map) {
    if (isCrawling) {
        dx *= 2.f; 
    }
    dx *= 2.f;
    shape.move(dx, 0); 
    isColliding = false;

    if (map.interactWithCharacter(shape, dx, false, isCrawling)) { 

        shape.move(-dx, 0); 
    }
    collisionDirection = 0; 

    sf::FloatRect bounds = shape.getGlobalBounds();
    sf::Vector2f position = shape.getPosition();
    int x = static_cast<int>(position.x / 50);
    int y = static_cast<int>(position.y / 50);

    
    if (y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == '#' || y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == 'k' || y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == 'g' || y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == 'r' || y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size() && map.getLayout()[y][x] == '0') {
        isColliding = true;
        collisionDirection = (dx > 0) ? 1 : -1; 
        shape.move(-dx, 0); 
    }

    if (dx != 0 && !isColliding) { 
        isMoving = true;
        Character::move(dx, map); 
        sprite.setScale((dx < 0) ? -2.f : 2.f, 2.f); 
        sprite.setOrigin((dx < 0) ? 55.f : 0, 0);
    }
    else {
        isMoving = false;
    }

    sprite.setPosition(shape.getPosition()); 
}




void Timon::checkGameOver(bool& isGameOver) {
    if (lives <= 0 && !isGameOver) {
        isGameOver = true;
        
    }
}


void Timon::jump() {
    if (isOnGround) {
        Character::jump();
        isJumping = true;
    }
}

void Timon::update(Map& map, float deltaTime) {
    /*if (lives == 0) {
        updateAnimation_die(deltaTime);
        return;
    }*/
    bool isBlockAbove = checkBlockAbove(map);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isBlockAbove && !crawlKeyPressed) {
        toggleCrawl();  
        crawlKeyPressed = true;  
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        
        crawlKeyPressed = false;
    }
    Character::update(map, deltaTime);
    if (isOnGround) {
        isJumping = false;
    }
    updateAnimation(deltaTime, map);
    
    sprite.setPosition(shape.getPosition());

}



void Timon::setColliding(bool colliding) {
    isColliding = colliding;
}

/*void Timon::updateAnimation_die(float deltaTime) {
    currentFrame = 0;
    while (currentFrame < die.size()) {
        animationTimer += deltaTime + 1;
        if (animationTimer >= frameTime) {
            animationTimer -= frameTime;
            
            
                sprite.setTextureRect(die[currentFrame]);
                currentFrame++;
            
        }
    }
}*/
void Timon::updateAnimation_die(float deltaTime) {
    shape.setSize(crawlSize);
    sprite.setPosition(shape.getPosition().x, shape.getPosition().y + 30);
    
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



void Timon::updateAnimation_victory(float deltaTime) {
    shape.setSize(originalSize);

    
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



bool Timon::checkBlockAbove(Map& map) {
    
    sf::Vector2f position = shape.getPosition();
    int x = static_cast<int>(position.x / 50); 
    int y = static_cast<int>((position.y - shape.getSize().y / 2) / 50); 

    
    if (y >= 0 && y < map.getLayout().size() && x >= 0 && x < map.getLayout()[y].size()) {
        char block = map.getLayout()[y][x];
        if (block == '#' || block == 'k' || block == 'g' || block == 'r' || block == '0') {
            return true; 
        }
    }
    return false;  
}

void Timon::updateAnimation(float deltaTime, Map& map) {
    
    if (isCrawling  ) {
        animationTimer += deltaTime;
        
        
        if (map.isBlockRemoving) {
            animationTimer += deltaTime;
            if (animationTimer >= frameTime) {
                animationTimer -= frameTime;
                currentFrame++;
                if (currentFrame < gblock.size()) {
                    sprite.setTextureRect(gblock[currentFrame]);
                }
                else {
                    map.isBlockRemoving = false;

                }
            }
        }
        else if (isMoving) {
            
            if (animationTimer >= frameTime) {
                animationTimer -= frameTime;
                currentFrame = (currentFrame + 1) % collisionFrames.size();
                sprite.setTextureRect(collisionFrames[currentFrame]);
            }

            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                sprite.setScale(-2.f, 2.0f);
                sprite.setOrigin(50.f, 0);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                sprite.setScale(2.f, 2.0f);
                sprite.setOrigin(0, 0);
            }
        }
        else if (!isMoving) {
            
            if (wasLookingLeft && ismove == 0) { 
                sprite.setScale(-2.0f, 2.f); 
            }
            currentFrame = 0;
            sprite.setTextureRect(collisionFrames[currentFrame]);
        }
        ismove = 1;
        if (isJumping) {
            sprite.setTextureRect(sf::IntRect(0, 55, 55, 55));
        }

      
    }
    else if (lives == 0) {
        animationTimer += deltaTime - 0.012f;
        if (animationTimer >= frameTime) {
            animationTimer -= frameTime;
            currentFrame = (currentFrame + 1) % die.size();
            sprite.setTextureRect(die[currentFrame]);
        }
   
    }
   
    else if (isJumping) {
        sprite.setTextureRect(sf::IntRect(0, 55, 55, 55));
    }
    else if (isHitAnimation) {
        sprite.setTextureRect(sf::IntRect(0, 165, 55, 55));
    }
    else if (!isOnGround) {
        sprite.setTextureRect(sf::IntRect(0, 110, 55, 55)); 
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

