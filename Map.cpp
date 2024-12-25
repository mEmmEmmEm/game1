#include "Map.h"
#include <stdexcept>
#include <iostream>
#include <fstream> 



Map::Map() {
    
    layout = {}; 
    


    if (!blockTexture_green.loadFromFile("green_block.png")) {
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_sand.loadFromFile("sand_block.png")) {
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_stone.loadFromFile("block_stone.png")) { 
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_sponge.loadFromFile("block_sponge.jpg")) { 
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_finish.loadFromFile("cavef.png")) {
        throw std::runtime_error("Failed to load finish texture");
    }
}

void Map::initialize(const std::vector<std::string>& _layout) {
    layout = _layout;

    
    if (!blockTexture_green.loadFromFile("green_block.png")) {
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_stoneformap.loadFromFile("stoneformap_block.png")) {
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_sand.loadFromFile("sand_block.png")) {
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_sandback.loadFromFile("sand_blockback.png")) {
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_stone.loadFromFile("block_stone.png")) { 
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_sponge.loadFromFile("block_sponge.jpg")) { 
        throw std::runtime_error("Failed to load block texture");
    }
    if (!blockTexture_finish.loadFromFile("cavef.png")) {
        throw std::runtime_error("Failed to load finish texture");
    }
    if (!blockTexture_back1.loadFromFile("background1.jpg")) {
        throw std::runtime_error("Failed to load finish texture");
    }
    if (!blockTexture_back2.loadFromFile("background2.jpg")) {
        throw std::runtime_error("Failed to load finish texture");
    }
    if (!blockTexture_back3.loadFromFile("background3.jpg")) {
        throw std::runtime_error("Failed to load finish texture");
    }
    if (!blockTexture_ice.loadFromFile("ice.png")) {
        throw std::runtime_error("Failed to load finish texture");
    }
    if (!blockTexture_grav.loadFromFile("ice_gr.png")) {
        throw std::runtime_error("Failed to load finish texture");
    }

    
    blocks.clear();
    for (size_t row = 0; row < layout.size(); ++row) {
        for (size_t col = 0; col < layout[row].size(); ++col) {
            char cell = layout[row][col];
            sf::RectangleShape block(sf::Vector2f(50.0f, 50.0f));
            block.setPosition(col * 50.0f, row * 50.0f);

            switch (cell) {
            case '#': block.setTexture(&blockTexture_green); break;
            case '0': block.setTexture(&blockTexture_sand); break;
            case 'i': block.setTexture(&blockTexture_ice); break;
            case 'l': block.setTexture(&blockTexture_grav); break;
            case '1': block.setTexture(&blockTexture_sandback); break;
            case 'k': block.setTexture(&blockTexture_stone); break;
            case 'g': block.setTexture(&blockTexture_sponge); break;
            case 'r': block.setTexture(&blockTexture_stoneformap); break;
            case 'f': 
                block.setSize(sf::Vector2f(300.0f, 300.0f));
                block.setTexture(&blockTexture_finish);
                block.setPosition(col * 50.0f, row * 40.0f);
                break;
            case 'b':
                block.setSize(sf::Vector2f(3000.0f, 1200.0f));
                block.setTexture(&blockTexture_back1);
                block.setPosition(col * 50.0f, row * 50.0f);
                break; 
            case 'a':
                block.setSize(sf::Vector2f(3000.0f, 1200.0f));
                block.setTexture(&blockTexture_back3);
                block.setPosition(col * 50.0f, row * 50.0f);
                break;
            case 'v':
                block.setSize(sf::Vector2f(3000.0f, 1200.0f));
                block.setTexture(&blockTexture_back2);
                block.setPosition(col * 50.0f, row * 50.0f);
                break;
            case '2':
                block.setSize(sf::Vector2f(300.0f, 300.0f));
                block.setTexture(&blockTexture_sandback);
                block.setPosition(col * 50.0f, row * 50.0f);
                break;
            default: continue;
            }
            blocks.push_back(block);

        }
    }
}


std::vector<std::string>& Map::getLayout()  {
    return layout;
}


sf::RectangleShape Map::getFinish() const {
    
    for (const auto& block : blocks) {
        if (block.getTexture() == &blockTexture_finish) {
            return block;  
        }
    }
    
    return sf::RectangleShape();
}


const std::vector<sf::RectangleShape>& Map::getBlocks() const {
    return blocks;
}


void Map::draw(sf::RenderWindow& window) {
    for (const auto& block : blocks) {
        window.draw(block); 
    }
}

bool Map::interactWithCharacter(sf::RectangleShape& character, float dx, bool isPumba, bool isCrawling) {
    if (isPumba && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        return false; 
    }
    if (!isPumba && !isCrawling){
        return false; 
    }

    
    for (size_t row = 0; row < layout.size(); ++row) {
        for (size_t col = 0; col < layout[row].size(); ++col) {
            char blockSymbol = layout[row][col];  

            
            if (blockSymbol != '#' && blockSymbol != 'k' && blockSymbol != 'g' && blockSymbol != 'r' && blockSymbol != '0' && blockSymbol != 'i' && blockSymbol != 'l') {
                continue;
            }

            
            float blockX = col * 50.0f;
            float blockY = row * 50.0f;
            sf::RectangleShape* block = nullptr;

            
            for (auto& b : blocks) {
                if (b.getPosition().x == blockX && b.getPosition().y == blockY) {
                    block = &b;
                    break;
                }
            }

            if (!block) continue; 

            
            if (character.getGlobalBounds().intersects(block->getGlobalBounds())) {
                
                if (blockSymbol == '#' || blockSymbol == 'k' || blockSymbol == 'g' ||  blockSymbol == 'r' || blockSymbol == '0' || blockSymbol == 'i' || blockSymbol == 'l') {
                    
                    if (blockSymbol == 'k' && isPumba) {
                        
                        float pushDirection = (dx > 0) ? 50.5f : -50.5f;

                        
                        int adjacentCol = col + ((dx > 0) ? 1 : -1);
                        if (adjacentCol >= 0 && adjacentCol < layout[row].size() &&
                            (layout[row][adjacentCol] != ' ' && layout[row][adjacentCol] != 'k' )) {
                            return false; 
                        }

                        
                        block->move(pushDirection, 0.0f);

                        
                        int newBlockX = static_cast<int>((block->getPosition().x + pushDirection) / 50);
                        if (newBlockX < 0 || newBlockX >= layout[row].size()) {
                            block->move(-pushDirection, 0.0f); 
                            return false;
                        }

                        
                        layout[row][col] = ' ';

                        
                        layout[row][newBlockX] = 'k';

                        
                        block->setPosition(newBlockX * 50.0f, row * 50.0f);

                        
                        while (row + 1 < layout.size() && layout[row + 1][newBlockX] == ' ') {
                            
                            layout[row][newBlockX] = ' '; 
                            row++; 
                            layout[row][newBlockX] = 'k'; 
                            block->setPosition(newBlockX * 50.0f, row * 50.0f); 
                            
                        }

                        return true;
                    }


                    if (blockSymbol == 'g' && !isPumba) {
                        if (character.getPosition().y + character.getGlobalBounds().height / 2.0f >= blockY) {
                                        std::cout << "Timon с головой в блоке 'g' и ползет.\n";
                                        isBlockRemoving = true;
                                        
                                        layout[row][col] = ' ';  

                                        
                                        auto it = std::remove_if(blocks.begin(), blocks.end(), [&](const sf::RectangleShape& b) {
                                            return b.getPosition().x == blockX && b.getPosition().y == blockY;
                                            });
                                        blocks.erase(it, blocks.end());  
                                        
                                        return true;  
                                        

                                    }

                                }

                                
                                character.move(-dx, 0);
                                return true;
                            }

                        }
                       }
    }
    return false;  
}


std::vector<EnemyPorcupine>& Map::getEnemiesP() {
    return enemies;
}

std::vector<EnemyHyena>& Map::getEnemiesH() {
    return hyenas;
}
