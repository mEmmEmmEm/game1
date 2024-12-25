#ifndef BUG_H
#define BUG_H

#include <SFML/Graphics.hpp>

class Bug {
public:
    enum BugType { TYPE1, TYPE2, TYPE3, TYPE4, TYPE5 }; 
    
    Bug(const sf::Texture& texture, float x, float y, BugType type);

    
    void draw(sf::RenderWindow& window);
    void update(float deltaTime);

    
    sf::Sprite& getSprite() { return sprite; }
    sf::Vector2f getPosition() const { return position; }

    
    bool checkCollision(const sf::FloatRect& rect);
    

private:
    int frameIndex;
    sf::RectangleShape shape;
    sf::Sprite sprite;  
    sf::Vector2f position;  
    sf::Texture texture;
    sf::IntRect currentFrame;      
    float animationTime;           
    float frameDuration;
    int initialFrameLeft;
    int frameCount;                

    void setAnimationType(BugType type); 
};


#endif 
