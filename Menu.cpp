#include "Menu.h"
#include <SFML/Window.hpp>
#include <fstream>
#include <sstream>

Menu::Menu(float width, float height)
    : currentSelection(0) {

    
    if (!font.loadFromFile("alphabet.otf")) {
        throw std::runtime_error("Failed to load font");
    }
    if (!backgroundTexture.loadFromFile("menubackground.jpg")) {
        throw std::runtime_error("Failed to load background image");
    }

    
    background.setTexture(backgroundTexture);
    background.setScale(width / background.getLocalBounds().width, height / background.getLocalBounds().height); 

    
    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(width / 2 - startText.getLocalBounds().width / 2 + 20, height / 2 + 160);

    
    startTextRect.setSize(sf::Vector2f(startText.getLocalBounds().width + 227, startText.getLocalBounds().height + 25));

    startTextRect.setFillColor(sf::Color::Transparent);
    startTextRect.setOutlineThickness(7);
    startTextRect.setOutlineColor(sf::Color::Black);
    startTextRect.setPosition(width / 2 - startText.getLocalBounds().width / 2 - 90, height / 2 + 155);

    
    rulesText.setFont(font);
    rulesText.setString("Game Rules");
    rulesText.setCharacterSize(40);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition(width / 2 - rulesText.getLocalBounds().width / 2 + 20, height / 1.75 + 190);

    
    rulesTextRect.setSize(sf::Vector2f(startText.getLocalBounds().width + 227, startText.getLocalBounds().height + 25));
    rulesTextRect.setFillColor(sf::Color::Transparent);
    rulesTextRect.setOutlineThickness(7);
    rulesTextRect.setOutlineColor(sf::Color::Black);
    rulesTextRect.setPosition(width / 2 - startText.getLocalBounds().width / 2 - 90, height / 2 + 270);

    
    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(width / 2 - exitText.getLocalBounds().width / 2 + 10, height / 1.25 + 120);

    
    exitTextRect.setSize(sf::Vector2f(startText.getLocalBounds().width + 227, startText.getLocalBounds().height + 25));
    exitTextRect.setFillColor(sf::Color::Transparent);
    exitTextRect.setOutlineThickness(7);
    exitTextRect.setOutlineColor(sf::Color::Black);
    exitTextRect.setPosition(width / 2 - startText.getLocalBounds().width / 2 - 90, height / 2 + 440);
}

void Menu::draw(sf::RenderWindow& window) {
    
    window.draw(background);

    window.draw(startText);
    window.draw(rulesText);
    window.draw(exitText);

    
    if (currentSelection == 0) {
        window.draw(startTextRect);  
    }
    else if (currentSelection == 1) {
        window.draw(rulesTextRect);  
    }
    else if (currentSelection == 2) {
        window.draw(exitTextRect);  
    }

}

void Menu::processEvents(sf::Event event, bool& gameStarted, bool& isGameOver, bool& showRules) {
    
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            if (currentSelection == 0) {
                gameStarted = true;
            }
            else if (currentSelection == 1) {
                showRules = true;
            }
            else if (currentSelection == 2) {
                isGameOver = true;
            }
        }
        if (event.key.code == sf::Keyboard::Escape) {
            isGameOver = true;
        }
        if (event.key.code == sf::Keyboard::Space && showRules) {
            showRules = false;  
        }
    }

    
    if (keyPressTimer.getElapsedTime().asSeconds() > keyPressDelay) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            currentSelection = (currentSelection > 0) ? currentSelection - 1 : 2;
            keyPressTimer.restart();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            currentSelection = (currentSelection < 2) ? currentSelection + 1 : 0;
            keyPressTimer.restart();
        }
    }
}

