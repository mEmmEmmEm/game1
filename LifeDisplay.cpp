#include "LifeDisplay.h"
#include "Timon.h"
#include "Pumba.h"

LifeDisplay::LifeDisplay(Timon* timon, Pumba* pumba)
    : timon(timon), pumba(pumba) {
    
    timonTexture.loadFromFile("timon_icon_finish.png");
    pumbaTexture.loadFromFile("pumba_icon_finish.png");
    heartTexture.loadFromFile("heart_icon.png");

    
    timonIcon.setTexture(timonTexture);
    timonIcon.setPosition(10, 10); 
    timonIcon.setScale(5.5f, 5.5f);

    pumbaIcon.setTexture(pumbaTexture);
    pumbaIcon.setPosition(10, 60); 
    pumbaIcon.setScale(5.5f, 5.5f);

    
    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(0.4f, 0.4f); 

    heartOffset = sf::Vector2f(40.f, 0.f); 
}

void LifeDisplay::setActiveCharacter(bool isTimonActive) {
    this->isTimonActive = isTimonActive;
}

void LifeDisplay::update(const Character& timon, const Character& pumba) {
    
}

void LifeDisplay::draw(sf::RenderWindow& window, const sf::View& camera) {
    
    sf::Vector2f cameraPosition = camera.getCenter();

    
    timonIcon.setPosition(cameraPosition.x - 900.f, cameraPosition.y - 540.f); 
    pumbaIcon.setPosition(cameraPosition.x - 900.f, cameraPosition.y - 440.f); 

    
    window.draw(timonIcon);
    window.draw(pumbaIcon);

    
    sf::Vector2f heartPosition_timon(cameraPosition.x - 800.f, cameraPosition.y - 530.f);
    sf::Vector2f heartPosition_pumba(cameraPosition.x - 800.f, cameraPosition.y - 400.f);
    drawHearts(window, heartPosition_timon, timon->lives);
    drawHearts(window, heartPosition_pumba, pumba->lives);
    if (isTimonActive) {
        
        timonIcon.setScale(6.5f, 6.5f);
        pumbaIcon.setScale(5.5f, 5.5f);
    }
    else {
        
        timonIcon.setScale(5.5f, 5.5f);
        pumbaIcon.setScale(6.5f, 6.5f);
    }
}

void LifeDisplay::drawHearts(sf::RenderWindow& window, const sf::Vector2f& position, int lives) {
    for (int i = 0; i < 3; ++i) {
        if (i < lives) {
            
            heartSprite.setPosition(position + sf::Vector2f(heartOffset.x * i, heartOffset.y));
            window.draw(heartSprite);
        }
    }
}
