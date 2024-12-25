
#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    
    Menu(float width, float height);

    void draw(sf::RenderWindow& window);
    void processEvents(sf::Event event, bool& gameStarted, bool& isGameOver, bool& showRules);  
    bool isExitSelected() const { return exitSelected; }
    bool isGameStarted() const { return gameStarted; }

private:
    sf::Font font;
    sf::Text title;
    sf::Text startText;
    sf::Text exitText;
    sf::Text rulesText;
    bool gameStarted;
    bool exitSelected;
    int currentSelection;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Clock keyPressTimer;
    float keyPressDelay = 0.2f; 

    sf::RectangleShape startTextRect; 
    sf::RectangleShape rulesTextRect; 
    sf::RectangleShape exitTextRect;  

};
