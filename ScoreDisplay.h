#ifndef SCOREDISPLAY_H
#define SCOREDISPLAY_H

#include <SFML/Graphics.hpp>

class ScoreDisplay {
public:
    ScoreDisplay(); 
    void update(int currentScore, int maxScore); 
    void draw(sf::RenderWindow& window, const sf::View& camera); 
    void reset();
private:
    sf::Texture bugTexture;       
    sf::Sprite bugIcon;           
    sf::Font font;                
    sf::Text scoreText;           
    int maxScore;                 
};

#endif 
#pragma once
