
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

    
    rect.setSize({300.0f, 200.0f});
    rect.setPosition({20.0f, 20.0f});
    rect.setFillColor(sf::Color(155, 155, 155, 255));

    textbox -> setFont(ctx -> assets -> getFont("Consolas"));
    textbox -> setCharacterSize(24);
    textbox -> setFillColor(sf::Color::White);

    textbox -> setPosition({50.0f, 50.0f});
    textbox -> setString("KEYBOARD OVERLAY");
    window.draw(rect);
    window.draw(*textbox);
}