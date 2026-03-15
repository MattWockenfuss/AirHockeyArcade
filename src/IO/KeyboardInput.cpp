
#include "KeyboardInput.hpp"
#include "InputManager.hpp"
#include "../Context.hpp"
#include "../KeyManager.hpp"
#include "../AssetManager.hpp"

void KeyboardInput::init(Context* ctx, InputManager* input){
    this -> ctx = ctx;
    this -> input = input;
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
}

void KeyboardInput::render(sf::RenderWindow& window){
    //render something to display key presses at some point?
}

