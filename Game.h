#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Timon.h"
#include "Pumba.h"
#include "Map.h"
#include "Enemyporcupine.h"
#include "EnemyHyena.h"
#include "LifeDisplay.h"
#include "Bug.h"
#include "ScoreDisplay.h"
#include <iostream>
#include "Menu.h"

class Game {
public:
    Game();
    void run();



private:
    Menu menu;
    sf::Music menuMusic;
    sf::Music level1Music;
    sf::Music level2Music;

    sf::SoundBuffer pointSoundBuffer;
    sf::SoundBuffer damageSoundBuffer;
    sf::SoundBuffer deathSoundBuffer;
    sf::SoundBuffer victorySoundBuffer;

    sf::Sound pointSound;
    sf::Sound damageSound;
    sf::Sound deathSound;
    sf::Sound victorySound;

    void loadResources();
    void stopAllMusic();
    void startLevel(int level);

    sf::Clock deathAnimationClock; 
    bool deathAnimationPlaying = false; 
    bool gameOverStarted = false; 
    sf::Clock endGameClock; 
    void processEvents();
    void update(float deltaTime);
    void render();
    void checkCollisions();
    void handleGameOver(float deltaTime);
    sf::RenderWindow window;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    bool gameOverTextDisplayed = true;
    sf::Font font;
    sf::Text gameOverText;
    sf::Clock clock;
    sf::View camera;
    sf::Vector2f cameraPosition;
    std::vector<Bug> bugs;
    ScoreDisplay scoreDisplay; 
    int score = 0;            
    int maxScore = 20;
    void spawnBugs1();
    void spawnBugs2();
    sf::Texture bugTexture;
    Timon timon;
    Pumba pumba;
    EnemyPorcupine porcupine1;
    EnemyPorcupine porcupine2;
    EnemyPorcupine porcupine3;
    EnemyPorcupine porcupine4;
    EnemyHyena hyena1;
    EnemyHyena hyena2;
    LifeDisplay lifeDisplay;
    void processEvents(sf::Event& event);//
    Map gameMap;
    bool isGameOver;
    bool gameStarted = false;
    void showRules();
    bool isShowingRules = false;
    void showMainMenu();
    bool isMenuActive = true;
    bool checkFinish(sf::RectangleShape& timon, sf::RectangleShape& pumba, Map& map, int currentScore, int maxScore);
    Character* currentCharacter;
    void spawnEnemiesForLevel(int level);
    void loadLevel(int level);
    int currentLevel = 1;


    std::vector<std::string> levelBackgrounds;
    std::vector<std::vector<sf::Vector2f>> levelEnemyPositions;
};

#endif 
