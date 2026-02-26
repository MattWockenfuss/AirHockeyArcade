#include "GameStateManager.hpp"
#include "MenuState.hpp"
#include "AirHockeyGameState.hpp"
#include "../Context.hpp"

GameStateManager::GameStateManager(Context& ctx)
:ctx(ctx), currentState(nullptr)
{
    //Created the GameStateManager
}

void GameStateManager::changeState(States s){
    //this function takes the States enum s, and changes to that state
    if(currentState != nullptr){
        delete currentState;
        currentState = nullptr;
    }
    switch(s){
        case States::Menu:
            currentState = new MenuState(ctx);
            break;
        case States::AirHockey:
            currentState = new AirHockeyGameState(ctx);
            break;
    }
}

State* GameStateManager::getCurrentState(){
    return currentState;
}



GameStateManager::~GameStateManager(){
    if (currentState != nullptr){
        delete currentState;
        currentState = nullptr;
    }
}