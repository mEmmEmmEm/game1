#include "Bug.h"
#include <stdexcept>

Bug::Bug(const sf::Texture& texture, float x, float y, BugType type)
    : animationTime(0.f), frameDuration(0.2f), frameCount(5) {

    sprite.setTexture(texture);
    shape.setSize(sf::Vector2f(25.0f, 25.0f));  
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red);
    sprite.setScale(2.f, 2.f);

    setAnimationType(type);

    position = { x, y };  
    sprite.setTextureRect(currentFrame);  
    sprite.setPosition(position);  
}

void Bug::setAnimationType(BugType type) {
    switch (type) {
    case BugType::TYPE1:
        initialFrameLeft = 0;
        currentFrame.left = 0;     
        currentFrame.top = 0;
        frameCount = 2;  
        break;
    case BugType::TYPE2:
        initialFrameLeft = 40;
        currentFrame.left = 40;   
        currentFrame.top = 0;
        frameCount = 3;  
        break;
    case BugType::TYPE3:
        initialFrameLeft = 100;
        currentFrame.left = 100;   
        currentFrame.top = 0;
        frameCount = 2;  
        break;
    case BugType::TYPE4:
        initialFrameLeft = 140;
        currentFrame.left = 140;   
        currentFrame.top = 0;
        frameCount = 2;  
        break;
    case BugType::TYPE5:
        initialFrameLeft = 180;
        currentFrame.left = 180;   
        currentFrame.top = 0;
        frameCount = 5;  
        break;
    default:
        throw std::invalid_argument("Invalid BugType");
    }
    currentFrame.width = 20;  
    currentFrame.height = 20; 
}

void Bug::update(float deltaTime) {
    
    animationTime += deltaTime;

    
    if (animationTime >= frameDuration) {
        animationTime = 0.f;  

        
        currentFrame.left += currentFrame.width;  

        
        if (currentFrame.left >= initialFrameLeft + currentFrame.width * frameCount) {
            currentFrame.left = initialFrameLeft;  
        }

        
        sprite.setTextureRect(currentFrame);
    }
}


void Bug::draw(sf::RenderWindow& window) {
    window.draw(sprite);  
}

bool Bug::checkCollision(const sf::FloatRect& rect) {
    return sprite.getGlobalBounds().intersects(rect);  
}
