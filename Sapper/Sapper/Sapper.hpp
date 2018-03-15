//
//  Sapper.hpp
//  Sapper
//
//  Created by Шисыр Мухаммед Шарипович on 12.12.17.
//  Copyright © 2017 Шисыр Мухаммед Шарипович. All rights reserved.
//

#ifndef Sapper_hpp
#define Sapper_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>

class Sapper{
public:
    Sapper(int s, float width, float height);
    void restart();
    void setMines(std::vector< std::vector<sf::Text> > sM);
    void init();
    void draw(sf::RenderWindow& window);
    std::vector< std::vector<sf::Text> > getMines();
    void improveMines();
    sf::Sprite getMark();
    int getNumberOfMines();
private:
    int numberOfMines;
    std::vector< std::vector<int> > ms;
    int size;
    sf::Image img;
    sf::Texture t;
    sf::Sprite sprite;
    sf::Font font;
    std::vector< std::vector<sf::Text> > mines;
    float w;
    float h;
    
};
#endif /* Sapper_hpp */
