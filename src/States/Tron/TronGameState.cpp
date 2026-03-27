#include "TronGameState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

#include "../../Context.hpp"
#include "../../AssetManager.hpp"
#include "../../KeyManager.hpp"
#include "../GameStateManager.hpp"


void TronGameState::init(Context* ctx){
    State::init(ctx);

    title_text.emplace(ctx -> assets -> getFont("Consolas"), "Tron", 75);
    play_text.emplace(ctx -> assets -> getFont("SquareSansSerif"), "Insert Coin to Play!", 35);
    seconds_counter.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "", 40);


    std::cout << "Tron Created!" << std::endl;


	const sf::FloatRect titleRect = title_text -> getLocalBounds();
	title_text -> setOrigin(titleRect.getCenter());
	title_text -> setPosition(sf::Vector2f(ctx -> window -> getSize().x / 2.0f, ctx -> window -> getSize().y / 5.0f));

	const sf::FloatRect textRect = play_text -> getLocalBounds();
	play_text -> setOrigin(textRect.getCenter());
	play_text -> setPosition(sf::Vector2f(ctx -> window -> getSize().x / 2.0f, ctx -> window -> getSize().y / 1.2f));
    
    const sf::FloatRect secondsRect = seconds_counter -> getLocalBounds();
    seconds_counter -> setOrigin(secondsRect.getCenter());
    seconds_counter -> setPosition(sf::Vector2f(ctx -> window -> getSize().x - 150.0f, 50.0f));

    p1 = Tron(ctx, Direction::North, 5, 10, 10, 10, 15, sf::Color::Green);
    p2 = Tron(ctx, Direction::East, 15, 20, 10, 20, 15, sf::Color::Red, 400.0f);


}

void TronGameState::tick() {
    //change state
	if(ctx->keys->F4){
		ctx->gsm->requestStateChange(States::AirHockey);
	}if(ctx->keys->F5){
		ctx->gsm->requestStateChange(States::FruitNinja);
	}
	
	//assume about 60 ticks per second, it can be slightly less, but good enough
    tickCount++;
    if(tickCount >= 15){
        tickCount = 0;
        p1.tick();
        p2.tick();
    }


}

void TronGameState::render(sf::RenderWindow& window) {
    //render for snake game state
    p1.render(window);
    p2.render(window);
    


    window.draw(*title_text);

}


TronGameState::TronGameState(){}