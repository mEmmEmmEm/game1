#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Enemyporcupine.h"
#include "EnemyHyena.h"

class Map {
public:
    Map();  
    
    Map(const std::vector<std::string>& layout);
    void initialize(const std::vector<std::string>& _layout);
    
    
    int isBlockRemoving = 0; 
    bool isBlockE = false;
    
    std::vector<std::string>& getLayout();
    sf::Clock removeBlockClock;
    

    const std::vector<sf::RectangleShape>& getBlocks() const;

    bool interactWithCharacter(sf::RectangleShape& character, float dx, bool isPumba, bool isCrowling); 

    
    void draw(sf::RenderWindow& window);
    sf::RectangleShape getFinish() const;
    std::vector<EnemyPorcupine>& getEnemiesP(); 
    std::vector<std::string> layout;
    std::vector<EnemyHyena>& getEnemiesH();

private:
    std::vector<sf::RectangleShape> blocks; 
    
    sf::Texture blockTexture_green; 
    sf::Texture blockTexture_sand;
    sf::Texture blockTexture_sandback;
    sf::Texture blockTexture_stone; 
    sf::Texture blockTexture_finish;
    sf::Texture blockTexture_back1;
    sf::Texture blockTexture_back2;
    sf::Texture blockTexture_back3;
    sf::Texture blockTexture_ice;
    sf::Texture blockTexture_grav;
    sf::Texture blockTexture_stoneformap;
    sf::RectangleShape finish;
    sf::Texture blockTexture_sponge;
    std::vector<EnemyPorcupine> enemies;
    std::vector<EnemyHyena> hyenas;
};

#endif 

