
#include "KeyboardInput.hpp"
#include "InputManager.hpp"
#include "../Context.hpp"
#include "../KeyManager.hpp"
#include "../AssetManager.hpp"

void KeyboardInput::init(Context* ctx, InputManager* input){
    this -> ctx = ctx;
    this -> input = input;
    textbox.emplace(ctx -> assets -> getFont("Consolas"), "", 12);
}

void KeyboardInput::tick(){
    //okay so on tick
    //if(ctx -> keys -> F1)
    //Player 1
    //Movement
    input -> P1_Up = ctx -> keys -> W;
    input -> P1_Left = ctx -> keys -> A;
    input -> P1_Down = ctx -> keys -> S;
    input -> P1_Right = ctx -> keys -> D;
    
    //Buttons
    input -> P1A = ctx -> keys -> R;
    input -> P1B = ctx -> keys -> T;
    input -> P1X = ctx -> keys -> F;
    input -> P1Y = ctx -> keys -> G;

    //Player 2
    //Movement
    input -> P2_Up = ctx -> keys -> Up;
    input -> P2_Left = ctx -> keys -> Left;
    input -> P2_Down = ctx -> keys -> Down;
    input -> P2_Right = ctx -> keys -> Right;

    //Buttons
    input -> P2A = ctx -> keys -> J;
    input -> P2B = ctx -> keys -> K;
    input -> P2X = ctx -> keys -> N;
    input -> P2Y = ctx -> keys -> M;

    if(ctx -> keys -> F1){
        if(triggeredFlag == false){
            //ok so we are holding F1 and we havent trigger it yet, 
            input -> overlay = !input -> overlay;
            triggeredFlag = true;
        }
    }else{// we stopped holding F1
        triggeredFlag = false;
    } 

}

void KeyboardInput::render(sf::RenderWindow& window){
    //okay on render, if overlay lets display something
    if(!input -> overlay) return;

    //okay so draw if overlay is true, then we want to draw to the window
    //The overlay renders the INPUT MANAGER VALUES for robustness and consistency

    //okay so I want to draw a little controller, actually 2 of them, one for each player
    //each one is going to have the 8 squares and render the values accordingly?

    
    rect.setSize({900.0f, 1150.0f});
    rect.setPosition({20.0f, 20.0f});
    rect.setFillColor(sf::Color(155, 155, 155, 255));
    window.draw(rect);


    textbox -> setFont(ctx -> assets -> getFont("Consolas"));
    textbox -> setCharacterSize(24);
    textbox -> setFillColor(sf::Color::Black);




    textbox -> setPosition({50.0f, 50.0f});
    textbox -> setString("KEYBOARD OVERLAY");
    window.draw(*textbox);

    float startX = 200.0f, startY = 200.0f;

    drawSquare(220.0f + startX, 30.0f + startY, this -> input -> P1_Up, window);
    drawSquare(185.0f + startX, 65.0f + startY, this -> input -> P1_Left, window);
    drawSquare(220.0f + startX, 100.0f + startY, this -> input -> P1_Down, window);
    drawSquare(255.0f + startX, 65.0f + startY, this -> input -> P1_Right, window);

    drawSquare(420.0f + startX, 40.0f + startY, this -> input -> P1A, window);
    drawSquare(470.0f + startX, 45.0f + startY, this -> input -> P1B, window);
    drawSquare(405.0f + startX, 85.0f + startY, this -> input -> P1X, window);
    drawSquare(455.0f + startX, 90.0f + startY, this -> input -> P1Y, window);


    textbox -> setPosition({270.0f, 30.0f});
    textbox -> setString("Up");
    window.draw(*textbox);

    textbox -> setPosition({270.0f, 65.0f});
    textbox -> setString("Left");
    window.draw(*textbox);

    textbox -> setPosition({270.0f, 100.0f});
    textbox -> setString("Down");
    window.draw(*textbox);

    textbox -> setPosition({270.0f, 135.0f});
    textbox -> setString("Right");
    window.draw(*textbox);




}

void KeyboardInput::drawSquare(float x, float y, bool isGreen, sf::RenderWindow& window){
    //this function will draw a square with a border centered and x ,y of width 30 and a border width of 1 px
    int borderWidth = 1;
    float width = 30.0f;
    rect.setPosition({x, y});
    rect.setSize({width + (borderWidth * 2), width + (borderWidth * 2)});
    rect.setOrigin({(width + (borderWidth * 2)) / 2, (width + (borderWidth * 2)) / 2});
    
    rect.setFillColor(sf::Color::Black);
    window.draw(rect);

    rect.setSize({width, width});
    rect.setOrigin({width / 2, width / 2});

    rect.setFillColor(sf::Color::Red);
    if(isGreen) rect.setFillColor(sf::Color::Green);
    window.draw(rect);


};