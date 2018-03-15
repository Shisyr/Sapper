
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Sapper.hpp"
#include <iostream>
#include <stack>
// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

using namespace sf;
using namespace std;


Image img[2];
Text text;
Texture texture[2];
Sprite sprite[2];
bool isEndOfGame();
bool isPlay;
int indx;
const int SIZE = 10;
vector<Text> outMines;
RenderWindow window(VideoMode(1500, 1200), "SFML window");
RectangleShape screen[SIZE][SIZE];
Sapper sapper(SIZE, window.getSize().x, window.getSize().y);
vector<Sprite> bombs;

struct Pixel{
    int r;
    int c;
    int nb;
    Pixel(int aR, int aC, int aNB) : r(aR), c(aC), nb(aNB){
    }
};



void init(){
    int squareX = window.getSize().x / (SIZE * 2);
    int squareY = window.getSize().y / (SIZE * 2);
    float sY = window.getSize().y / 2 - (window.getSize().y / 4);
    for(int i = 0;i < SIZE;i++){
        float sX = window.getSize().x / 2 - (window.getSize().y / 3);
        for(int j = 0;j < SIZE;j++){
            RectangleShape s(Vector2f(squareX, squareY));
            s.setPosition(sX, sY);
            s.setFillColor(Color::White);
            s.setOutlineThickness(1);
            s.setOutlineColor(Color::Blue);
            screen[i][j] = s;
            sX += squareX;
        }
        sY += squareY;
    }
}

void drawScreen(){
    for(int i = 0;i < SIZE;i++){
        for(int j = 0;j < SIZE;j++){
            window.draw(screen[i][j]);
        }
    }
}

void expandMines(int x, int y){
    vector<int> dr = {-1, 0, 1, 0};
    vector<int> dc = {0, 1, 0, -1};
    stack<Pixel> stack;
    vector< vector<Text> > mines = sapper.getMines();
    mines[x][y].setString(".");
    stack.push(Pixel(x, y, 0));
    while(not stack.empty()){
        bool found = false;
        for(int i = stack.top().nb;i < 4 and not found;i++){

            int cr = stack.top().r + dr[i];
            int cc = stack.top().c + dc[i];
            if((cr >= 0 and cr < mines.size()) and (cc >= 0 and cc < mines.size())){
                string check = mines[cr][cc].getString();
                
                if(check == "0"){
                    found = true;
                    mines[cr][cc].setString(".");
                    stack.top().nb = i;
                    stack.push(Pixel(cr, cc, 0));
                }
                else if(check != "*" and check != "0" and check != "."){
                    outMines.push_back(mines[cr][cc]);
                }
            }
        }
        if(not found){
            stack.pop();
        }
    }
    for(int i = 0;i < mines.size();i++){
        for(int j = 0;j < mines[i].size();j++){
            if(mines[i][j].getString() == "."){
                mines[i][j].setString("0");
                outMines.push_back(mines[i][j]);
            }
        }
    }
}


void openMines(Event& event){
    if(event.type == Event::MouseButtonPressed){
        int x = event.mouseButton.x;
        int y = event.mouseButton.y;
            for(int i = 0;i < sapper.getMines().size();i++){
                for(int j = 0;j < sapper.getMines()[i].size();j++){
                    int spX = sapper.getMines()[i][j].getPosition().x;
                    int spY = sapper.getMines()[i][j].getPosition().y;
                    int size = sapper.getMines()[i][j].getCharacterSize();
                    if(x >= spX - (SIZE * 2) and x < spX + size and y >= spY and y < spY + size){
                        if(Mouse::isButtonPressed(Mouse::Right)){
                            bool isDelete = true;
                            for(int b = 0;b < bombs.size();b++){
                                if(bombs[b].getPosition().x == spX and bombs[b].getPosition().y == spY){
                                    bombs.erase(bombs.begin() + b, bombs.begin() + b + 1);
                                    isDelete = false;
                                }
                            }
                            if(isDelete){
                                Sprite sp = sapper.getMark();
                                sp.setPosition(spX, spY);
                                bombs.push_back(sp);
                            }
                        }
                        else{
                            if(sapper.getMines()[i][j].getString() == "*"){
                                for(int i1 = 0;i1 < sapper.getMines().size();i1++){
                                    for(int j1 = 0;j1 < sapper.getMines()[i1].size();j1++){
                                        if(sapper.getMines()[i1][j1].getString() == "*"){
                                            outMines.push_back(sapper.getMines()[i1][j1]);
                                        }
                                    }
                                }
                                text.setString("You lose!");
                                isPlay = false;
                            }
                            else{
                                bool isAdd = true;
                                for(int i1 = 0;i1 < outMines.size();i1++){
                                    if(spX == outMines[i1].getPosition().x and spY == outMines[i1].getPosition().y){
                                        isAdd = false;
                                    }
                                }
                                if(isAdd){
                                    if(sapper.getMines()[i][j].getString() != "0"){
                                        outMines.push_back(sapper.getMines()[i][j]);
                                    }
                                    else{
                                        expandMines(i, j);
                                    }
                                }
                            }
                        }
                        
                    }
                }
            }
    }
}

void play(Event& event, Sprite spr){
    int x = event.mouseButton.x;
    int y = event.mouseButton.y;
    int sX = spr.getPosition().x;
    int sY = spr.getPosition().y;
    if(x >= sX and x < sX + 50 and y >= sY and y < sY + 50){
        sapper.restart();
        outMines.clear();
        bombs.clear();
        isPlay = true;
    }
}
void addImage(){
    img[0].loadFromFile("/Users/Mukhamed/Downloads/Photoes/over.gif");
    img[1].loadFromFile("/Users/Mukhamed/Downloads/Photoes/play.png");
    texture[0].loadFromImage(img[0]);
    texture[1].loadFromImage(img[1]);
    sprite[0].setTexture(texture[0]);
    sprite[1].setTexture(texture[1]);
    sprite[0].setPosition(window.getSize().x / 2, 20);
    sprite[1].setPosition(window.getSize().x / 2, 20);
}

int main(int, char const**)
{
    Font font;
    if(!font.loadFromFile("/Users/Mukhamed/Downloads/fonts/Black.ttf")){
        //error
    }
    text.setFont(font);
    text.setString("You win!");
    text.setFillColor(Color::Yellow);
    text.setPosition(window.getSize().x / 2 - 100, 100);
    addImage();
    init();
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                window.close();
            }
            play(event, sprite[indx]);
            if(isPlay){
             openMines(event);
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
            indx = (isPlay) ? 1 : 0;
        }
        window.clear();
        if(isEndOfGame()){
            window.draw(text);
            isPlay = false;
        }
        if(not isPlay){
            window.draw(text);
        }
        drawScreen();
        window.draw(sprite[indx]);
        for(int i = 0;i < outMines.size();i++){
            window.draw(outMines[i]);
        }
        for(int i = 0;i < bombs.size();i++){
            window.draw(bombs[i]);
        }
        //sapper.draw(window);
        window.display();
    }

    return EXIT_SUCCESS;
}





bool isEndOfGame(){
    int count = sapper.getNumberOfMines();
    return (outMines.size() + count == 100) ? true : false;
}
