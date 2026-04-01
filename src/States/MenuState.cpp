#include "MenuState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>


#include "../Context.hpp"
#include "../AssetManager.hpp"
#include "../KeyManager.hpp"
#include "GameStateManager.hpp"

MenuState::MenuState(){}

void MenuState::init(Context* ctx){
    State::init(ctx);

    title_text.emplace(ctx -> assets -> getFont("Technoid"), "Air Hockey Arcade", 75);
    play_text.emplace(ctx -> assets -> getFont("SquareSansSerif"), "Insert Coin to Play!", 35);
    seconds_counter.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "", 40);


    std::cout << "MenuState Created!" << std::endl;


	const sf::FloatRect titleRect = title_text -> getLocalBounds();
	title_text -> setOrigin(titleRect.getCenter());
	title_text -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x / 2.0f, ctx -> p1window -> getSize().y / 5.0f));

	const sf::FloatRect textRect = play_text -> getLocalBounds();
	play_text -> setOrigin(textRect.getCenter());
	play_text -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x / 2.0f, ctx -> p1window -> getSize().y / 1.2f));
    
    const sf::FloatRect secondsRect = seconds_counter -> getLocalBounds();
    seconds_counter -> setOrigin(secondsRect.getCenter());
    seconds_counter -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x - 150.0f, 50.0f));
    

}

void MenuState::tick() {
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration elapsed = end - start;
    sec = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    seconds_counter -> setString(std::to_string(sec));

    // if ((sec = static_cast<unsigned int>(sec)) % 2 == 0) {
    //     ctx -> gpio -> P1A_LED.lastKnownValue = true;
    // }else{
    //     ctx -> gpio -> P1A_LED.lastKnownValue = false;
    // }
}

void MenuState::p1render(sf::RenderWindow& p1window) {
    //render for menustate
    p1window.draw(*title_text);
    p1window.draw(*seconds_counter);
    
    
    if ((sec = static_cast<unsigned int>(sec)) % 2 == 0) {
        p1window.draw(*play_text);
    }
}

void MenuState::p2render(sf::RenderWindow& p2window) {
    //render for menustate
    p2window.draw(*title_text);
    p2window.draw(*seconds_counter);
    
    
    if ((sec = static_cast<unsigned int>(sec)) % 2 == 0) {
        p2window.draw(*play_text);
    }
}