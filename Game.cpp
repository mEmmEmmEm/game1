#include "Game.h"
#include <fstream> 

#include <sstream> 

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "Timon & Pumba Game", sf::Style::Fullscreen),
    porcupine1(3800.0f, 1000.0f), porcupine2(13000.0f, 1200.0f), porcupine3(15700.0f, 23 * 50), porcupine4(353 * 50, 28 * 50), hyena1(0,0), hyena2(0,0),
    isGameOver(false),
    currentCharacter(&timon),
    lifeDisplay(&timon, &pumba),
    menu(1920, 1080), 
    gameStarted(false),
    currentLevel(1){
    loadResources();
    showMainMenu();
    window.setFramerateLimit(60);

    std::ifstream mapFile("map1.txt");
    if (!mapFile.is_open()) {
        throw std::runtime_error("Failed to open map file");
    }

    std::string line;
    std::vector<std::string> mapLines;
    while (std::getline(mapFile, line)) {
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        mapLines.push_back(line);
    }
    gameMap.initialize(mapLines);

    if (!backgroundTexture.loadFromFile("background1.jpg")) {
        throw std::runtime_error("Failed to load background image");
    }

    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
        static_cast<float>(windowSize.y) / backgroundTexture.getSize().y/2 + 0.5
    );
    backgroundSprite.setPosition(2000.f, 100.f);


    if (!font.loadFromFile("alphabet.otf")) {
        throw std::runtime_error("Failed to load font");
    }
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getLocalBounds().width / 2, window.getSize().y / 2 - 25);

    timon.shape.setPosition(1150, 650 - timon.shape.getSize().y + 500);
    pumba.shape.setPosition(1000, 650 - pumba.shape.getSize().y + 500);
    camera = window.getDefaultView();
    cameraPosition = camera.getCenter();
    timon.update(gameMap, 0.f);  
    pumba.update(gameMap, 0.f);
}

void Game::loadResources() {
    if (!menuMusic.openFromFile("song_for_menu.ogg"))
        throw std::runtime_error("Failed to load menu music");
    if (!level1Music.openFromFile("song_for1level.ogg"))
        throw std::runtime_error("Failed to load level 1 music");
    if (!level2Music.openFromFile("song_for2level.ogg"))
        throw std::runtime_error("Failed to load level 2 music");

    if (!pointSoundBuffer.loadFromFile("song_for_score.ogg"))
        throw std::runtime_error("Failed to load point sound");
    if (!damageSoundBuffer.loadFromFile("song_hit.ogg"))
        throw std::runtime_error("Failed to load damage sound");
    if (!deathSoundBuffer.loadFromFile("song_die.ogg"))
        throw std::runtime_error("Failed to load death sound");
    if (!victorySoundBuffer.loadFromFile("song_win.ogg"))
        throw std::runtime_error("Failed to load victory sound");

    pointSound.setBuffer(pointSoundBuffer);
    damageSound.setBuffer(damageSoundBuffer);
    deathSound.setBuffer(deathSoundBuffer);
    victorySound.setBuffer(victorySoundBuffer);
    menuMusic.setVolume(100.0f); 
    level1Music.setVolume(100.0f);
    level2Music.setVolume(100.0f);
    pointSound.setVolume(50.0f);
}

void Game::stopAllMusic() {
    menuMusic.stop();
    level1Music.stop();
    level2Music.stop();
}

void Game::loadLevel(int level) {

    std::string mapFileName = "map" + std::to_string(level) + ".txt"; 
    std::ifstream mapFile(mapFileName);
    if (!mapFile.is_open()) {
        throw std::runtime_error("Failed to open map file");
    }

    std::string line;
    std::vector<std::string> mapLines;
    while (std::getline(mapFile, line)) {
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        mapLines.push_back(line);
    }
    gameMap.initialize(mapLines); 

    std::string backgroundFileName = "background" + std::to_string(level) + ".jpg";
    if (!backgroundTexture.loadFromFile(backgroundFileName)) {
        throw std::runtime_error("Failed to load background image");
    }
    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
        static_cast<float>(windowSize.y) / backgroundTexture.getSize().y / 2 + 0.5
    );
    currentLevel = level;
    spawnEnemiesForLevel(level);
}

void Game::startLevel(int level) {
    stopAllMusic();  
    isMenuActive = false;  
    gameStarted = true;
    loadLevel(level); 

    if (level == 1) {
        level1Music.setLoop(true);
        level1Music.play();  
    }
    else if (level == 2) {
        level2Music.setLoop(true);
        level2Music.play(); 
    }

}


void Game::spawnEnemiesForLevel(int level) {
    
    if (level == 1) {
        porcupine1.setPosition(3800.0f, 1000.0f);
        porcupine2.setPosition(13000.0f, 1200.0f); 
        porcupine3.setPosition(15700.0f, 23*50);
        porcupine4.setPosition(353*50, 28*50);
        spawnBugs1();
    }
    else if (level == 2) {
        timon.reset();
        score = 0;
        pumba.reset();
        hyena1.reset(103 * 50, 19.5 * 50);
        hyena2.reset(199 * 50, 22.5 * 50);
        timon.update(gameMap, 0.f); 
        pumba.update(gameMap, 0.f);
        porcupine1.setPosition(0.0f, 0.0f);
        porcupine2.setPosition(0.0f, 0.0f);
        porcupine3.setPosition(0.0f, 0 * 50);
        porcupine4.setPosition(0 * 50, 0 * 50);

        currentCharacter = &timon;

        camera.setCenter(currentCharacter->shape.getPosition());  
        scoreDisplay.reset();
        spawnBugs2();
    }
}

void Game::spawnBugs1() {
    if (!bugTexture.loadFromFile("bug.png")) {
        throw std::runtime_error("Failed to load bug texture");
    }

    bugs.push_back(Bug(bugTexture, 67*50, 18.5*50, Bug::TYPE5));
    bugs.push_back(Bug(bugTexture, 94*50, 21.5*50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 184*50, 24.5*50, Bug::TYPE4));
    bugs.push_back(Bug(bugTexture, 184*50, 17*50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 262*50, 21*50, Bug::TYPE3)); //fly
    bugs.push_back(Bug(bugTexture, 199*50, 24.5*50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 344*50, 18*50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 48 * 50, 19.5 * 50, Bug::TYPE4));
    bugs.push_back(Bug(bugTexture, 116 * 50, 20.5 * 50, Bug::TYPE4));
    bugs.push_back(Bug(bugTexture, 174 * 50, 24.5 * 50, Bug::TYPE5));
    bugs.push_back(Bug(bugTexture, 224 * 50, 20 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 286 * 50, 20 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 370 * 50, 21.5 * 50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 41 * 50, 17 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 263 * 50, 24.5 * 50, Bug::TYPE4));
    bugs.push_back(Bug(bugTexture, 143 * 50, 18.5 * 50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 293 * 50, 21.5 * 50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 250 * 50, 18 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 351 * 50, 27 * 50, Bug::TYPE3)); //fly
    bugs.push_back(Bug(bugTexture, 362 * 50, 23.5 * 50, Bug::TYPE5));

}

void Game::spawnBugs2() {
    if (!bugTexture.loadFromFile("bug.png")) {
        throw std::runtime_error("Failed to load bug texture");
    }

    bugs.push_back(Bug(bugTexture, 60 * 50, 21.5 * 50, Bug::TYPE5));
    bugs.push_back(Bug(bugTexture, 98 * 50, 15.5 * 50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 130 * 50, 15.5 * 50, Bug::TYPE4));
    bugs.push_back(Bug(bugTexture, 175 * 50, 21 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 121 * 50, 19 * 50, Bug::TYPE3)); //fly
    bugs.push_back(Bug(bugTexture, 178 * 50, 25.5 * 50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 211 * 50, 22 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 244 * 50, 23.5 * 50, Bug::TYPE4)); 
    bugs.push_back(Bug(bugTexture, 300 * 50, 21.5 * 50, Bug::TYPE4));
    bugs.push_back(Bug(bugTexture, 304 * 50, 25.5 * 50, Bug::TYPE5));
    bugs.push_back(Bug(bugTexture, 280 * 50, 19 * 50, Bug::TYPE1)); //flys
    bugs.push_back(Bug(bugTexture, 355 * 50, 24 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 113 * 50, 15.5 * 50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 258 * 50, 19 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 346 * 50, 22.5 * 50, Bug::TYPE4));
    bugs.push_back(Bug(bugTexture, 357 * 50, 20.5 * 50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 193 * 50, 24.5 * 50, Bug::TYPE2));
    bugs.push_back(Bug(bugTexture, 314 * 50, 20 * 50, Bug::TYPE1)); //fly
    bugs.push_back(Bug(bugTexture, 40 * 50, 19 * 50, Bug::TYPE3)); //fly
    bugs.push_back(Bug(bugTexture, 322 * 50, 25.5 * 50, Bug::TYPE5));

}

void Game::showRules() {
    window.clear(sf::Color::Black);

    std::ifstream file("rules.txt"); 
    std::stringstream buffer;
    buffer << file.rdbuf();  

    sf::Text rulesText;
    rulesText.setFont(font);
    rulesText.setString(buffer.str());
    rulesText.setCharacterSize(30);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition(100, 100); 

    window.draw(rulesText); 
    window.display(); 
}





void Game::run() {

    while (window.isOpen()) {
        sf::Event event;

        if (window.pollEvent(event)) {
            std::cout << "Event type: " << event.type << std::endl; 
        }

        if (isMenuActive) {
            menu.processEvents(event, gameStarted, isGameOver, isShowingRules); 

            if (gameStarted) {
                isMenuActive = false;
                startLevel(1);
            }
            else if (isGameOver) {
                window.close();
            }
            else if (isShowingRules) {
                showRules(); 
            }
        }
        else {
            if (isShowingRules) {
                showRules(); 
            }
            else {
                processEvents(event); 
                update(clock.restart().asSeconds());
                checkCollisions();
            }
        }

        render(); 
    }
}




void Game::showMainMenu() {
    stopAllMusic();
    menuMusic.setLoop(true);
    menuMusic.play();

    isMenuActive = true;
    gameStarted = false;
    isGameOver = false;
    isShowingRules = false;
    timon.reset();
    score = 0; //
    pumba.reset();
    if (!backgroundTexture.loadFromFile("background1.jpg")) {
        throw std::runtime_error("Failed to load background image");
    }
    hyena1.reset(100.0f, 450.0f);
    hyena2.reset(100.0f, 450.0f);
    timon.update(gameMap, 2.f);  
    pumba.update(gameMap, 2.f);
   
    currentLevel = 1;
    currentCharacter = &timon; 

    camera.setCenter(currentCharacter->shape.getPosition()); 
    scoreDisplay.reset();
}

void Game::processEvents(sf::Event& event) {
    if (isShowingRules) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                isShowingRules = false; 
                showMainMenu(); 
            }
        }
        return; 
    }

    if (gameOverStarted) {
        return; 
    }

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    static bool isKeyPressed = false;
    if (!isGameOver) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !isKeyPressed) {
            currentCharacter = (currentCharacter == &timon) ? static_cast<Character*>(&pumba) : static_cast<Character*>(&timon);
            isKeyPressed = true;
            lifeDisplay.setActiveCharacter(currentCharacter == &timon);
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            isKeyPressed = false;
        }

        float dx = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            dx = -2.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            dx = 2.0f;
        }

        currentCharacter->move(dx, gameMap);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            currentCharacter->jump();
        }
    }
}

void Game::handleGameOver(float deltaTime) {
    if (timon.lives == 0) {
        stopAllMusic();
        deathSound.play();
        timon.updateAnimation_die(deltaTime);  
        window.draw(timon.sprite);
    }
    else {
        stopAllMusic();
        deathSound.play();
     
        pumba.updateAnimation_die(deltaTime); 

        window.draw(pumba.sprite);
    }

    window.display();
    if (timon.flag || pumba.flag) {
        window.setView(window.getDefaultView());
        window.clear(sf::Color::Black);
        showMainMenu();
    }
}







void Game::update(float deltaTime) {
    timon.checkGameOver(isGameOver);  
    pumba.checkGameOver(isGameOver);
   
    if (isGameOver) {
       
        handleGameOver(deltaTime);
        return;
    }
    if (checkFinish(timon.shape, pumba.shape, gameMap, score, maxScore)) {
        stopAllMusic();
        victorySound.play();
       
        timon.updateAnimation_victory(deltaTime);
        window.draw(timon.sprite);
        pumba.updateAnimation_victory(deltaTime);
        window.draw(pumba.sprite);
       
        window.display();

        
        if (timon.flag || pumba.flag) {
            window.setView(window.getDefaultView());
            window.clear(sf::Color::Black);
            currentLevel++;
            if(currentLevel == 2) startLevel(2);
            else {

                currentLevel = 1;
                showMainMenu();
            }
        }
        return;
    }
    hyena1.checkCollisionWithTimon(timon);
    hyena1.checkCollisionWithPumba(pumba);
    hyena1.checkCollisionWithPumbaHit(pumba);
    hyena2.checkCollisionWithTimon(timon);
    hyena2.checkCollisionWithPumba(pumba);
    hyena2.checkCollisionWithPumbaHit(pumba);
   

    currentCharacter->update(gameMap, deltaTime);
    porcupine1.update(gameMap, deltaTime); 
    porcupine2.update(gameMap, deltaTime);
    porcupine3.update(gameMap, deltaTime);
    porcupine4.update(gameMap, deltaTime);
    for (auto& bug : bugs) {  
        bug.update(deltaTime);  
    }
    hyena1.update(gameMap, timon, deltaTime);
    hyena2.update(gameMap, timon, deltaTime);
    hyena1.update(gameMap, pumba, deltaTime);
    hyena2.update(gameMap, pumba, deltaTime);
    sf::Vector2f characterPos = currentCharacter->shape.getPosition();
    sf::Vector2f cameraCenter = camera.getCenter();
    float cameraCenterX = camera.getCenter().x;
    camera.setCenter(characterPos);  
    window.setView(camera);  

}

bool Game::checkFinish(sf::RectangleShape& timon, sf::RectangleShape& pumba, Map& map, int currentScore, int maxScore) {

    sf::RectangleShape finish = map.getFinish();

    bool timonOnFinish = timon.getGlobalBounds().intersects(finish.getGlobalBounds());
    bool pumbaOnFinish = pumba.getGlobalBounds().intersects(finish.getGlobalBounds());


    return timonOnFinish && pumbaOnFinish && (currentScore == maxScore);
}


void Game::render() {
    if (isShowingRules) {
        return; 
    }
    window.clear();

    if (!gameStarted) {
        menu.draw(window);  
    }
    else {
        window.draw(backgroundSprite);
        window.setView(camera);
        gameMap.draw(window);

        for (auto& bug : bugs) {
            bug.draw(window);
        }

        porcupine1.draw(window);
        porcupine2.draw(window);
        porcupine3.draw(window);
        porcupine4.draw(window);
        hyena1.draw(window);
        hyena2.draw(window);
        window.draw(timon.sprite);

        window.draw(pumba.sprite);
        lifeDisplay.draw(window, camera);
        scoreDisplay.draw(window, camera);
    }

    window.display();
}

void Game::checkCollisions() {
    for (auto it = bugs.begin(); it != bugs.end(); ) {
        if (it->checkCollision(currentCharacter->shape.getGlobalBounds())) {
            pointSound.play();
          
            score++;  
            it = bugs.erase(it); 
            scoreDisplay.update(score, maxScore);
        }
        else {
            ++it;  
        }
    }
    if (porcupine1.checkCollision(*currentCharacter)) {
        damageSound.play();
        currentCharacter->onHitByEnemy(porcupine1.velocity);
    }
    if (porcupine2.checkCollision(*currentCharacter)) {
        damageSound.play();
        currentCharacter->onHitByEnemy(porcupine2.velocity);
    }
    if (porcupine3.checkCollision(*currentCharacter)) {
        damageSound.play();
        currentCharacter->onHitByEnemy(porcupine3.velocity);
    }
    if (porcupine4.checkCollision(*currentCharacter)) {
        damageSound.play();
        currentCharacter->onHitByEnemy(porcupine4.velocity);
    }
}

