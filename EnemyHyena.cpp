#include "EnemyHyena.h"
#include "Map.h"
#include "Character.h"

EnemyHyena::EnemyHyena(float x, float y)
    : currentFrame(0), animationTimer(0.f), frameTime(0.15f), speed(100.f), speed1(200.f), isDead(false), spriteOffset(60.f, 70.f), state(Idle) {
    shape.setSize({ 150.f, 120.f });
    shape.setPosition(x, y);
    if (!texture.loadFromFile("hyenafinish.png")) {
        throw std::runtime_error("Не удалось загрузить текстуру для Гиены");
    }

    sprite.setTexture(texture);

    for (int i = 0; i < 6; ++i) {
        frames.emplace_back(i * 60 + 2, 0, 60, 60);
    }
    for (int i = 0; i < 5; ++i) {
        chaseFrames.emplace_back(i * 62 + 2, 55, 60, 54);
    }
    for (int i = 0; i < 14; ++i) {
        deathFrames.emplace_back(i * 60 + 2, 150, 60, 80);
    }
    sprite.setOrigin(30.f, 30.f);
    sprite.setTextureRect(frames[currentFrame]);
    sprite.setPosition(shape.getPosition());

    velocity.x = speed;
    velocity.y = 0;
    frameTime = 0.2f;
}

void EnemyHyena::reset(float x, float y) {

    shape.setPosition(x, y);
    sprite.setPosition(shape.getPosition() + spriteOffset);

    velocity.x = speed;
    velocity.y = 0;

    state = Idle;
    isDead = false;

    currentFrame = 0;
    animationTimer = 0.f;
    sprite.setTextureRect(frames[currentFrame]);
    sprite.setScale(3.f, 2.5f); 

    if (!texture.loadFromFile("hyenafinish.png")) {
        throw std::runtime_error("Не удалось загрузить текстуру для Гиены");
    }
}


void EnemyHyena::update(Map& map, Character& character, float deltaTime) {
    if (state == Death) {

        playDeathAnimation(deltaTime);
        
        return;
    }


    sf::Vector2f hyenaPosition = shape.getPosition();
    sf::Vector2f characterPosition = character.shape.getPosition();

    bool isInRadius = isCharacterInRadius(character, 500.f);


    if (isInRadius && state != Death) {
        state = Chase; 
    }
    else if (!isInRadius && state != Death) {
        state = Idle;
    }
    sf::FloatRect hyenaBounds = shape.getGlobalBounds();

   
    bool isBlocked = false;
    for (const auto& block : map.getBlocks()) {
        
        float blockX = block.getPosition().x;
        float blockY = block.getPosition().y;

        int row = static_cast<int>(blockY / 50.0f);  
        int col = static_cast<int>(blockX / 50.0f);  
        char blockSymbol = map.getLayout()[row][col];  

        if (blockSymbol == 'a') continue;

        
        if (hyenaBounds.intersects(block.getGlobalBounds())) {
            isBlocked = true;
            break;
        }
    }

    if (isBlocked) {
        if (!isInRadius) {
          
            velocity.x = -velocity.x; 
            sprite.setScale((velocity.x > 0 ? 3.f : -3.f), 2.5f);
        }
        else {
           
            velocity.x = 0;  
        }
    }
    else {
        if (state == Chase) {
            if (characterPosition.x < hyenaPosition.x) {
                velocity.x = -speed1;
                sprite.setScale(-3.f, 2.5f);
            }
            else if (characterPosition.x > hyenaPosition.x) {
                velocity.x = speed1;
                sprite.setScale(3.f, 2.5f); 
            }
        }
        else if (state == Idle) {
            velocity.x = (velocity.x < 0) ? -speed : speed;
            sprite.setScale((velocity.x > 0 ? 3.f : -3.f), 2.5f);
        }
    }
 
    shape.move(velocity * deltaTime);


    updateAnimation(deltaTime);

    if (checkCollisionWithPumbaHit(character)) {
        state = Death; 
    }
    else if (checkCollisionWithPumba(character)) {

        return;
    }
    sprite.setPosition(shape.getPosition() + spriteOffset);
}



void EnemyHyena::playDeathAnimation(float deltaTime) {

    animationTimer += deltaTime;
    if (animationTimer >= frameTime) {
        animationTimer -= frameTime;
        currentFrame++;
        if (currentFrame < deathFrames.size()) {
            sprite.setTextureRect(deathFrames[currentFrame]);
        }
        else {
            isDead = true;
           
            
            shape.setPosition(-1000.f, -1000.f); 
           
        }
    }

}

void EnemyHyena::checkCollisionWithMap(Map& map, float deltaTime, bool isInRadius) {
    
    sf::FloatRect hyenaBounds = shape.getGlobalBounds();

   
    bool isBlocked = false;
    for (const auto& block : map.getBlocks()) {
      
        float blockX = block.getPosition().x;
        float blockY = block.getPosition().y;

        int row = static_cast<int>(blockY / 50.0f);  
        int col = static_cast<int>(blockX / 50.0f);  
        char blockSymbol = map.getLayout()[row][col];  

        if (blockSymbol == 'a') continue;

      
        if (hyenaBounds.intersects(block.getGlobalBounds())) {
            isBlocked = true;
            break;
        }
    }

    if (isBlocked) {
        if (!isInRadius) {
            
            velocity.x = -velocity.x;  
            sprite.setScale((velocity.x > 0 ? 3.f : -3.f), 2.5f); 
        }
        else {
           
            velocity.x = 0;  
        }
    }
    else {

    }
}




void EnemyHyena::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= frameTime) {
        animationTimer -= frameTime;
        currentFrame = (currentFrame + 1) %
            ((state == Chase) ? chaseFrames.size() : frames.size());
        if (state == Chase) {
            sprite.setTextureRect(chaseFrames[currentFrame]);
        }
        else if (state == Idle) {
            sprite.setTextureRect(frames[currentFrame]);
        }
    }
}


bool EnemyHyena::checkCollisionWithPumbaHit(Character& pumba) {
    if (pumba.isPumba && sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && shape.getGlobalBounds().intersects(pumba.shape.getGlobalBounds())) {
        return true;
    }
    return false;
}

bool EnemyHyena::checkCollisionWithPumba(Character& pumba) {
    if (pumba.isPumba && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        return false;
    }
    if (pumba.isPumba && shape.getGlobalBounds().intersects(pumba.shape.getGlobalBounds()) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        pumba.onHitByEnemy(velocity);
        return true;
    }
    return false;
}


bool EnemyHyena::checkCollisionWithTimon(Character& timon) {
    if (!timon.isPumba && shape.getGlobalBounds().intersects(timon.shape.getGlobalBounds())) {
        timon.onHitByEnemy(velocity);
        return true;
    }
    return false;
}


bool EnemyHyena::isCharacterInRadius(Character& character, float radius) {
    sf::Vector2f hyenaPosition = shape.getPosition();
    sf::Vector2f characterPosition = character.shape.getPosition();

    float distance = std::sqrt(
        std::pow(characterPosition.x - hyenaPosition.x, 2) +
        std::pow(characterPosition.y - hyenaPosition.y, 2)
    );

    return distance <= radius;
}
void EnemyHyena::draw(sf::RenderWindow& window) {
    if (!isDead) {
        window.draw(sprite);
        //window.draw(shape);  
    }
}