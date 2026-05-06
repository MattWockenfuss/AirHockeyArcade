#include "IdleState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>


#include "../Context.hpp"
#include "../AssetManager.hpp"
#include "../KeyManager.hpp"
#include "../IO/InputManager.hpp"
#include "GameStateManager.hpp"
#include "../AudioManager.hpp"

void IdleState::init(Context* ctx){
    State::init(ctx);

    title_text.emplace(ctx -> assets -> getFont("SquareSansSerif"), "B J A M M  DUOCADE", 50);
    play_text.emplace(ctx -> assets -> getFont("SquareSansSerif"), "Press A To Start!", 28);


    std::cout << "IdleState Created!" << std::endl;

	const sf::FloatRect titleRect = title_text -> getLocalBounds();
	title_text -> setOrigin(titleRect.getCenter());
	

	const sf::FloatRect textRect = play_text -> getLocalBounds();
	play_text -> setOrigin(textRect.getCenter());
	
}

void IdleState::tick() {
    if(ctx -> window -> getView().getSize().x != viewWidth){
        //if the new x is not viewWidth, update the width and height and everything downstream
        viewWidth = ctx -> window -> getView().getSize().x;
        viewHeight = ctx -> window -> getView().getSize().y;

        title_text -> setPosition({viewWidth / 2.0f, viewHeight / 3.0f});
        play_text -> setPosition({viewWidth / 2.0f, viewHeight / 1.3f});
    }

    
    

    if(ctx -> input -> P1A || ctx -> input -> P2A){
        ctx -> audio -> playSound(ctx -> assets -> getSound("Boot"));
		ctx -> gsm -> requestStateChange(States::NameEntry, 1.5f, 1.5f);
	}


	
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration elapsed = end - start;
    sec = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
	if(sec>=255){ // prevent difference in time from getting too big
		start = std::chrono::steady_clock::now();
	}
}

void IdleState::p1render(sf::RenderWindow& p1window) {
    //render 1 for idlestate
    p1window.draw(*title_text);
    
    if ((sec = static_cast<unsigned int>(sec)) % 2 == 0) {
        p1window.draw(*play_text);
    }
}

void IdleState::p2render(sf::RenderWindow& p2window) {
    //render 2 for idlestate
    p2window.draw(*title_text);
    
    if ((sec = static_cast<unsigned int>(sec)) % 2 == 0) {
        p2window.draw(*play_text);
    }
}