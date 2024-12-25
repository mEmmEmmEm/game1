#ifndef LIFEDISPLAY_H
#define LIFEDISPLAY_H

#include <SFML/Graphics.hpp>
#include "Timon.h"
#include "Pumba.h"



class LifeDisplay {
public:
    LifeDisplay(Timon* timon, Pumba* pumba);

    void setActiveCharacter(bool isTimonActive);
    void update(const Character& timon, const Character& pumba); 
    void draw(sf::RenderWindow& window, const sf::View& camera); 

private:
    Timon* timon;
    Pumba* pumba;
    bool isTimonActive = true;  

    
    sf::Texture timonTexture;
    sf::Texture pumbaTexture;
    sf::Texture heartTexture;

    
    sf::Sprite timonIcon;
    sf::Sprite pumbaIcon;
    sf::Sprite heartSprite;

    sf::Vector2f heartOffset;  

    void drawHearts(sf::RenderWindow& window, const sf::Vector2f& position, int lives); 
};

#endif
