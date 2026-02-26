#include "MenuState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>


#include "../Context.hpp"
#include "../AssetManager.hpp"
#include "../KeyManager.hpp"
#include "GameStateManager.hpp"


MenuState::MenuState(Context& ctx)
    : 
        State(ctx), 
        title_text(ctx.assets -> getFont("Technoid"), "Air Hockey Arcade", 75),
        play_text(ctx.assets -> getFont("SquareSansSerif"), "Insert Coin to Play!", 35),
        seconds_counter(ctx.assets -> getFont("ST-SimpleSquare"), "", 40)
    {
    std::cout << "MenuState Created!" << std::endl;


	const sf::FloatRect titleRect = title_text.getLocalBounds();
	title_text.setOrigin(titleRect.getCenter());
	title_text.setPosition(sf::Vector2f(ctx.window -> getSize().x / 2.0f, ctx.window -> getSize().y / 5.0f));

	const sf::FloatRect textRect = play_text.getLocalBounds();
	play_text.setOrigin(textRect.getCenter());
	play_text.setPosition(sf::Vector2f(ctx.window -> getSize().x / 2.0f, ctx.window -> getSize().y / 1.2f));
    
    const sf::FloatRect secondsRect = seconds_counter.getLocalBounds();
    seconds_counter.setOrigin(secondsRect.getCenter());
    seconds_counter.setPosition(sf::Vector2f(ctx.window -> getSize().x - 150.0f, 50.0f));
    

}

void MenuState::tick() {
    if(ctx.keys -> F4){
        ctx.gsm -> changeState(States::AirHockey);
    }
}

void MenuState::render(sf::RenderWindow& window) {
    //render for menustate

    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration elapsed = end - start;
    long long sec = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    seconds_counter.setString(std::to_string(sec));


    //play_text.setString(std::to_string(sec));
    window.draw(title_text);
    window.draw(seconds_counter);
    
    if ((sec = static_cast<unsigned int>(sec)) % 2 == 0) {
        window.draw(play_text);
    }
}