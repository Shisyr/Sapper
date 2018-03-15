//
//  Sapper.cpp
//  Sapper
//
//  Created by Шисыр Мухаммед Шарипович on 12.12.17.
//  Copyright © 2017 Шисыр Мухаммед Шарипович. All rights reserved.
//

#include "Sapper.hpp"

using namespace std;
using namespace sf;

Sapper::Sapper(int s, float width, float height){
    size = s;
    w = width;
    h = height;
    restart();
}
void Sapper::restart(){
    mines.clear();
    img.loadFromFile("/Users/Mukhamed/Downloads/Photoes/mine.png");
    t.loadFromImage(img);
    sprite.setTexture(t);
    numberOfMines = 0;
    init();
    improveMines();
}

Sprite Sapper::getMark(){
    return sprite;
}

void Sapper::init(){
    if(!font.loadFromFile("/Users/Mukhamed/Downloads/fonts/Black.ttf")){
        //error
    }
    int sizeX = w / (size * 2);
    int sizeY = h / (size * 2);
    int sizeString = (sizeX) - (sizeY / 2);
    float tY =  h / 2 - (h / 4);
    for(int i = 0;i < size;i++){
        vector<Text> row;
        int countMines = 3;
        float tX = w / 2 - (h / 3) + (size * 2);
        for(int j = 0;j < size;j++){
            int r = rand() % 2;
            Text t;
            t.setCharacterSize(sizeString);
            t.setFont(font);
            t.setFillColor(Color::Green);
            t.setString((r == 1 and countMines > 0) ? "*" : "0");
            numberOfMines += (r == 1 and countMines > 0) ? 1 : 0;
            countMines -= (r == 1) ? 1 : 0;
            t.setPosition(tX, tY);
            row.push_back(t);
            tX += sizeX;
        }
        tY += sizeY;
        mines.push_back(row);
    }
}
int Sapper::getNumberOfMines(){
    return numberOfMines;
}
void Sapper::improveMines(){
    for(int i = 0;i < mines.size();i++){
        for(int j = 0;j < mines[i].size();j++){
            if(mines[i][j].getString() == "0"){
                int cnt = 0;
                if(i + 1 < mines.size()){
                    cnt += (mines[i + 1][j].getString() == "*")? 1 : 0;
                }
                if(j + 1 < mines[i].size()){
                    cnt += (mines[i][j + 1].getString() == "*")? 1 : 0;
                }
                if(j - 1 >= 0){
                    cnt += (mines[i][j - 1].getString() == "*")? 1 : 0;
                }
                if(i - 1 >= 0){
                    cnt += (mines[i - 1][j].getString() == "*")? 1 : 0;
                }
                if(i - 1 >= 0 and j - 1 >= 0){
                    cnt += (mines[i - 1][j - 1].getString() == "*")? 1 : 0;
                }
                if(i - 1 >= 0 and j + 1 < mines[i].size()){
                    cnt += (mines[i - 1][j + 1].getString() == "*")? 1 : 0;
                }
                if(i + 1 < mines.size() and j + 1 < mines[i].size()){
                    cnt += (mines[i + 1][j + 1].getString() == "*")? 1 : 0;
                }
                if(i + 1 < mines.size() and j - 1 >= 0){
                    cnt += (mines[i + 1][j - 1].getString() == "*")? 1 : 0;
                }
                mines[i][j].setString(to_string(cnt));
            }
       }
    }
}

vector< vector<Text> > Sapper::getMines(){
    return mines;
}
void Sapper::setMines(vector< vector<Text> > sM){
    mines = sM;
}

void Sapper::draw(RenderWindow& window){
    for(int i = 0;i < mines.size();i++){
        for(int j = 0;j < mines[i].size();j++){
           window.draw(mines[i][j]);
        }
    }
}
