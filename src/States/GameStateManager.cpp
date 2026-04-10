#include "GameStateManager.hpp"

#include "IdleState.hpp"
#include "AirHockey/AirHockeyGameState.hpp"
#include "Tron/TronGameState.hpp"
#include "FruitNinja/FruitNinjaGameState.hpp"
#include "NameEntryState.hpp"
#include "GameSelect/GameSelectState.hpp"

#include "../Context.hpp"
#include "../AssetManager.hpp"
#include <cmath>
#include <iostream>



void GameStateManager::init(Context* ctx){
    this -> ctx = ctx;     //ctx is a memory address so no need to dereference, just keep track of the memory address for everyone
    this -> currentState = nullptr;

    debugLabel.emplace(ctx -> assets -> getFont("ArcadeNormal"), "", 52);
}

void GameStateManager::requestStateChange(States s, double fadeOut, double fadeIn){
    if(pendingStateChange){
        std::cout << "[GAME STATE MANAGER] Already Processing a State Change! Ignoring Switch to '" << stateToString(s) << "' State" << std::endl;
        return;
    }
    if(s == currentStateType){
        std::cout << "[GAME STATE MANAGER] The Game is already in the '" << stateToString(s) << "' State" << std::endl;
        return;
    }
    std::cout << "CHANGING GAME STATE " << std::endl;
    pendingStateChange = true;
    pendingState = s;
    this -> fadeOut = fadeOut;
    this -> fadeIn = 0;
    this -> fadeOutMax = fadeOut;
    this -> fadeInMax = fadeIn;
}

void GameStateManager::changeState(){
    //this function takes the States enum s, and changes to that state
    std::cout << "Changing State" << std::endl;
    if(currentState != nullptr){
        delete currentState;
        currentState = nullptr;
    }
    switch(pendingState){
        case States::Idle:
            currentState = new IdleState();
            currentState -> init(ctx);
            break;
        case States::AirHockey:
            currentState = new AirHockeyGameState();
            currentState -> init(ctx);
            break;
        case States::Tron:
            currentState = new TronGameState();
            currentState -> init(ctx);
            break;
		case States::FruitNinja:
			currentState = new FruitNinjaGameState();
			currentState -> init(ctx);
			break;
        case States::NameEntry:
			currentState = new NameEntryState();
			currentState -> init(ctx);
			break;
		case States::GameSelect:
			currentState = new GameSelectState();
			currentState -> init(ctx);
			break;
    }
    currentStateType = pendingState;
}

void GameStateManager::tick(){
    /*
        okay, what do we do? if we are pending a state change
        then try to decrease fadeout first, then state change, then fade in, then set pendingStateChange to false, dont allow new state changes

    
    
    
    
    */

    if(pendingStateChange){
        //std::cout << "PENDINGSTATE CHANGE IN TICK" << std::endl;
        //std::cout << std::to_string(alphaLeft) + ", fadeIn: " + std::to_string(fadeIn) + " / " + std::to_string(fadeInMax) + "\nfadeOut: " + std::to_string(fadeOut) + " / " + std::to_string(fadeOutMax) << std::endl;
        //okay so we are either fading out or fading in, or done
        //if we just started fading in, then change the state
        //std::cout << "fadeSpeed: " << std::to_string(fadeSpeed) << std::endl; 
        if(fadeOut > 0){
            fadeOut -= fadeSpeed;
            if(fadeOut < 0) fadeOut = 0;
            //std::cout << "FADING OUT " << std::to_string(fadeOut) << std::endl;
            alphaLeft = std::floor((double)((fadeOutMax - fadeOut) / fadeOutMax) * 255.0f);
        }else if(fadeIn < fadeInMax){
            if(fadeIn == 0) changeState();
            //std::cout << "FADING IN " << std::to_string(fadeIn) << std::endl;
            fadeIn += fadeSpeed;
            if(fadeIn > fadeInMax) fadeIn = fadeInMax;
            //std::cout << "FADING IN " << std::to_string(fadeIn) << std::endl;
            alphaLeft = std::floor((double)((fadeInMax - fadeIn) / fadeInMax) * 255.0f);
        }else{
            //then we are done
            //std::cout << "WE ARE DONE" << std::endl;
            pendingStateChange = false;
            fadeOut = 0.0f;
            fadeIn = fadeInMax;
            alphaLeft = 0;
        }
    }
}   
void GameStateManager::p1render(sf::RenderWindow& p1window){
    //std::cout << "RENDERING" << std::to_string(fadeIn) << std::endl;
    //okay so we render a transition
    p1window.getSize().x;

    square.setFillColor(sf::Color(0, 0, 0, alphaLeft));
    square.setSize({(float) p1window.getSize().x, (float) p1window.getSize().y});
    square.setPosition({0.0f, 0.0f});
    p1window.draw(square);

    //debugLabel -> setOrigin({0.0f, 0.0f});
    //debugLabel -> setString(std::to_string(alphaLeft) + ", fadeIn: " + std::to_string(fadeIn) + " / " + std::to_string(fadeInMax) + "\nfadeOut: " + std::to_string(fadeOut) + " / " + std::to_string(fadeOutMax));
    //debugLabel -> setFillColor(sf::Color::Yellow);
    //debugLabel -> setPosition({10.0f, 10.0f});
    //p1window.draw(*debugLabel);
}
void GameStateManager::p2render(sf::RenderWindow& p2window){
    //okay so we render a transition

    square.setFillColor(sf::Color(0, 0, 0, alphaLeft));
    square.setSize({(float) p2window.getSize().x, (float) p2window.getSize().y});
    square.setPosition({0.0f, 0.0f});
    p2window.draw(square);
}
State* GameStateManager::getCurrentState(){
    return currentState;
}