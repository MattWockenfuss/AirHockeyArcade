#include "GameStateManager.hpp"
#include "MenuState.hpp"
#include "AirHockeyGameState.hpp"
#include "Snake/SnakeGameState.hpp"

#include "../Context.hpp"



void GameStateManager::init(Context* ctx){
    this -> ctx = ctx;     //ctx is a memory address so no need to dereference, just keep track of the memory address for everyone
    this -> currentState = nullptr;
}

void GameStateManager::requestStateChange(States s){
    pendingStateChange = true;
    pendingState = s;
}

void GameStateManager::changeState(){
    //this function takes the States enum s, and changes to that state
    if(currentState != nullptr){
        delete currentState;
        currentState = nullptr;
    }
    switch(pendingState){
        case States::Menu:
            currentState = new MenuState();
            currentState -> init(ctx);
            break;
        case States::AirHockey:
            currentState = new AirHockeyGameState();
            currentState -> init(ctx);
            break;
        case States::Snake:
            currentState = new SnakeGameState();
            currentState -> init(ctx);
            break;
    }
}

State* GameStateManager::getCurrentState(){
    return currentState;
}