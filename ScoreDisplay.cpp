#include "ScoreDisplay.h"
#include <stdexcept>

ScoreDisplay::ScoreDisplay()
    : maxScore(20) { 
    if (!bugTexture.loadFromFile("bugicon.png")) {
        throw std::runtime_error("Failed to load bug icon texture");
    }
    if (!font.loadFromFile("alphabet.otf")) {
        throw std::runtime_error("Failed to load font for score display");
    }

    
    bugIcon.setTexture(bugTexture);
    bugIcon.setScale(2.5f, 2.5f); 

    
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
}

void ScoreDisplay::update(int currentScore, int maxScore) {
    this->maxScore = maxScore; 
    scoreText.setString(std::to_string(currentScore) + "/" + std::to_string(maxScore));
}

void ScoreDisplay::reset() {
    scoreText.setString("0/" + std::to_string(maxScore));  
}

void ScoreDisplay::draw(sf::RenderWindow& window, const sf::View& camera) {
    sf::Vector2f cameraPosition = camera.getCenter();

    
    bugIcon.setPosition(cameraPosition.x, cameraPosition.y - 500.f); 
    scoreText.setPosition(bugIcon.getPosition().x + 65.f, bugIcon.getPosition().y + 20.0f); 

    
    window.draw(bugIcon);
    window.draw(scoreText);
}
