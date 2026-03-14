#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "InputInterface.hpp"
#include "../Context.hpp"
#include "../AssetManager.hpp"
#include "../KeyManager.hpp"

#include <iostream>

InputManager::InputManager() = default;



void InputManager::init(Context* ctx){
    this -> ctx = ctx;

    /*
        So depending on whether or not we have it, we want to tick GPIO, always tick keyboard?
        I like that too
    
    */


    backendInput = new KeyboardInput();
    backendInput -> init(ctx, this);

    std::cout << "[InputManager] Using KEYBOARD INPUT" << std::endl;

    //for the graphics because most SFML objects dont have default constructors D:
    textbox.emplace(ctx -> assets -> getFont("Consolas"), "", 12);
}

void InputManager::tick(){
    if(ctx -> keys -> F1){
        if(triggeredFlag == false){
            //ok so we are holding F1 and we havent trigger it yet, 
            overlay = !overlay;
            triggeredFlag = true;
        }
    }else{// we stopped holding F1
        triggeredFlag = false;
    } 
    backendInput -> tick();
}
void InputManager::render(sf::RenderWindow& window){
    backendInput -> render(window);
    renderDebugControllers(window);
}

void InputManager::renderDebugControllers(sf::RenderWindow& window){
    //okay on render, if overlay lets display something
    if(!overlay) return;

    //okay so draw if overlay is true, then we want to draw to the window
    //The overlay renders the INPUT MANAGER VALUES for robustness and consistency

    //okay so I want to draw a little controller, actually 2 of them, one for each player
    //each one is going to have the 8 squares and render the values accordingly?

    float startX = 200.0f, startY = 200.0f;


    //Player 1 Controller
    float bw = 2.0f;
    //base rect
    //border
    rect.setSize({420.0f + (bw * 2), 200.0f + (bw * 2)});
    rect.setPosition({startX - bw, startY - bw});
    rect.setFillColor(sf::Color(74, 74, 74, 255));
    window.draw(rect);
    //actual
    rect.setSize({420.0f, 200.0f});
    rect.setPosition({startX, startY});
    rect.setFillColor(sf::Color(173, 216, 230, 255));
    window.draw(rect);

    //player title text rect
    //border
    rect.setSize({210.0f + (bw * 2), 40.0f + (bw * 2)});
    rect.setPosition({startX + 105.0f - bw, startY - 40.0f - bw});
    rect.setFillColor(sf::Color(74, 74, 74, 255));
    window.draw(rect);
    //actual
    rect.setSize({210.0f, 40.0f});
    rect.setPosition({startX + 105.0f, startY - 40.0f});
    rect.setFillColor(sf::Color(173, 216, 230, 255));
    window.draw(rect);


    textbox -> setFont(ctx -> assets -> getFont("Consolas"));
    textbox -> setCharacterSize(36);
    textbox -> setFillColor(sf::Color::Black);
    //textbox -> setOrigin({});



    textbox -> setPosition({startX + 110.0f, startY - 58.0f});
    textbox -> setString("Player 1");
    //text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textbox -> setStyle(sf::Text::Bold);
    window.draw(*textbox);
    textbox -> setStyle(sf::Text::Regular);
    textbox -> setCharacterSize(24);
    

    //up down left right
    drawSquare(120.0f + startX, 40.0f + startY, P1_Up, window);
    drawSquare(85.0f + startX, 75.0f + startY, P1_Left, window);
    drawSquare(120.0f + startX, 110.0f + startY, P1_Down, window);
    drawSquare(155.0f + startX, 75.0f + startY, P1_Right, window);

    //A B X Y
    drawSquare(320.0f + startX, 50.0f + startY, P1A, window);
    drawSquare(370.0f + startX, 55.0f + startY, P1B, window);
    drawSquare(305.0f + startX, 95.0f + startY, P1X, window);
    drawSquare(355.0f + startX, 100.0f + startY, P1Y, window);

    //up down left right
    textbox -> setPosition({120.0f + startX - 15.0f, 40.0f + startY - 50.0f});
    textbox -> setString("Up");
    window.draw(*textbox);

    textbox -> setPosition({85.0f + startX - 80.0f, 75.0f + startY - 15.0f});
    textbox -> setString("Left");
    window.draw(*textbox);

    textbox -> setPosition({120.0f + startX - 25.0f, 110.0f + startY + 20.0f});
    textbox -> setString("Down");
    window.draw(*textbox);

    textbox -> setPosition({155.0f + startX + 25.0f, 75.0f + startY - 15.0f});
    textbox -> setString("Right");
    window.draw(*textbox);

    //A B X Y
    textbox -> setPosition({320.0f + startX - 6.5f, 50.0f + startY - 44.0f});
    textbox -> setString("A");
    window.draw(*textbox);

    textbox -> setPosition({370.0f + startX - 6.5f, 55.0f + startY - 44.0f});
    textbox -> setString("B");
    window.draw(*textbox);

    textbox -> setPosition({305.0f + startX - 6.5f, 95.0f + startY + 11.0f});
    textbox -> setString("X");
    window.draw(*textbox);

    textbox -> setPosition({355.0f + startX - 6.5f, 100.0f + startY + 11.0f});
    textbox -> setString("Y");
    window.draw(*textbox);





    startX = 200.0f, startY = 500.0f;
    //Player 2 Controller
    bw = 2.0f;
    //base rect
    //border
    rect.setSize({420.0f + (bw * 2), 200.0f + (bw * 2)});
    rect.setPosition({startX - bw, startY - bw});
    rect.setFillColor(sf::Color(74, 74, 74, 255));
    window.draw(rect);
    //actual
    rect.setSize({420.0f, 200.0f});
    rect.setPosition({startX, startY});
    rect.setFillColor(sf::Color(255, 255, 224, 255));
    window.draw(rect);

    //player title text rect
    //border
    rect.setSize({210.0f + (bw * 2), 40.0f + (bw * 2)});
    rect.setPosition({startX + 105.0f - bw, startY - 40.0f - bw});
    rect.setFillColor(sf::Color(74, 74, 74, 255));
    window.draw(rect);
    //actual
    rect.setSize({210.0f, 40.0f});
    rect.setPosition({startX + 105.0f, startY - 40.0f});
    rect.setFillColor(sf::Color(255, 255, 224, 255));
    window.draw(rect);


    textbox -> setFont(ctx -> assets -> getFont("Consolas"));
    textbox -> setCharacterSize(36);
    textbox -> setFillColor(sf::Color::Black);
    //textbox -> setOrigin({});



    textbox -> setPosition({startX + 110.0f, startY - 58.0f});
    textbox -> setString("Player 2");
    //text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textbox -> setStyle(sf::Text::Bold);
    window.draw(*textbox);
    textbox -> setStyle(sf::Text::Regular);
    textbox -> setCharacterSize(24);
    

    //up down left right
    drawSquare(120.0f + startX, 40.0f + startY, P2_Up, window);
    drawSquare(85.0f + startX, 75.0f + startY, P2_Left, window);
    drawSquare(120.0f + startX, 110.0f + startY, P2_Down, window);
    drawSquare(155.0f + startX, 75.0f + startY, P2_Right, window);

    //A B X Y
    drawSquare(320.0f + startX, 50.0f + startY, P2A, window);
    drawSquare(370.0f + startX, 55.0f + startY, P2B, window);
    drawSquare(305.0f + startX, 95.0f + startY, P2X, window);
    drawSquare(355.0f + startX, 100.0f + startY, P2Y, window);

    //up down left right
    textbox -> setPosition({120.0f + startX - 15.0f, 40.0f + startY - 50.0f});
    textbox -> setString("Up");
    window.draw(*textbox);

    textbox -> setPosition({85.0f + startX - 80.0f, 75.0f + startY - 15.0f});
    textbox -> setString("Left");
    window.draw(*textbox);

    textbox -> setPosition({120.0f + startX - 25.0f, 110.0f + startY + 20.0f});
    textbox -> setString("Down");
    window.draw(*textbox);

    textbox -> setPosition({155.0f + startX + 25.0f, 75.0f + startY - 15.0f});
    textbox -> setString("Right");
    window.draw(*textbox);

    //A B X Y
    textbox -> setPosition({320.0f + startX - 6.5f, 50.0f + startY - 44.0f});
    textbox -> setString("A");
    window.draw(*textbox);

    textbox -> setPosition({370.0f + startX - 6.5f, 55.0f + startY - 44.0f});
    textbox -> setString("B");
    window.draw(*textbox);

    textbox -> setPosition({305.0f + startX - 6.5f, 95.0f + startY + 11.0f});
    textbox -> setString("X");
    window.draw(*textbox);

    textbox -> setPosition({355.0f + startX - 6.5f, 100.0f + startY + 11.0f});
    textbox -> setString("Y");
    window.draw(*textbox);
}
void InputManager::drawSquare(float x, float y, bool isGreen, sf::RenderWindow& window){
    //this function will draw a square with a border centered and x ,y of width 30 and a border width of 1 px
    int borderWidth = 2;
    float width = 30.0f;
    rect.setPosition({x, y});
    rect.setSize({width + (borderWidth * 2), width + (borderWidth * 2)});
    rect.setOrigin({(width + (borderWidth * 2)) / 2, (width + (borderWidth * 2)) / 2});
    
    rect.setFillColor(sf::Color::Black);
    window.draw(rect);

    rect.setSize({width, width});
    rect.setOrigin({width / 2, width / 2});

    rect.setFillColor(sf::Color::Red);
    if(isGreen) rect.setFillColor(sf::Color(0, 158, 96));
    window.draw(rect);


};




InputManager::~InputManager(){
    delete backendInput;
}