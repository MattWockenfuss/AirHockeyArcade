#include "TronGameState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

#include "../../Context.hpp"
#include "../../AssetManager.hpp"
#include "../../KeyManager.hpp"
#include "../../IO/InputManager.hpp"
#include "../GameStateManager.hpp"


void TronGameState::init(Context* ctx){
    State::init(ctx);

    title_text.emplace(ctx -> assets -> getFont("Consolas"), "Tron", 75);
    play_text.emplace(ctx -> assets -> getFont("SquareSansSerif"), "Insert Coin to Play!", 35);
    seconds_counter.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "", 40);

    p1score.emplace(ctx -> assets -> getFont("SquareSansSerif"), "Player 1: 0", 30);
    p2score.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), "Player 2: 0", 30);

    std::cout << "Tron Created!" << std::endl;


	const sf::FloatRect titleRect = title_text -> getLocalBounds();
	title_text -> setOrigin(titleRect.getCenter());
	title_text -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x / 2.0f, ctx -> p1window -> getSize().y / 5.0f));

	const sf::FloatRect textRect = play_text -> getLocalBounds();
	play_text -> setOrigin(textRect.getCenter());
	play_text -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x / 2.0f, ctx -> p1window -> getSize().y / 1.2f));
    
    const sf::FloatRect secondsRect = seconds_counter -> getLocalBounds();
    seconds_counter -> setOrigin(secondsRect.getCenter());
    seconds_counter -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x - 150.0f, 50.0f));

    p1score -> setOrigin(secondsRect.getCenter());
    p1score -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x - 250.0f, 50.0f));

    p2score -> setOrigin(secondsRect.getCenter());
    p2score -> setPosition(sf::Vector2f(ctx -> p1window -> getSize().x - 250.0f, 150.0f));


    p1 = Tron(ctx, Direction::North, 5, 10, 10, 10, 15, sf::Color::Green);
    p2 = Tron(ctx, Direction::East, 15, 20, 10, 20, 15, sf::Color::Red, 400.0f);


}

void TronGameState::tick() {
	//assume about 60 ticks per second, it can be slightly less, but good enough
	
	// emergency game exit
	if(ctx->input->P1B && ctx->input->P1Y && ctx->input->P2B && ctx->input->P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
		ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
	}
	
    tickCount++;
    if(tickCount >= 6){
        tickCount = 0;
        p1.tick();
        p2.tick();
    }

    seconds_counter -> setString(std::to_string(tickCount));


}

void TronGameState::p1render(sf::RenderWindow& p1window) {
    //render for snake game state
    p1.render(p1window);
    p2.render(p1window);

    p1window.draw(*title_text);
    p1window.draw(*play_text);
    p1window.draw(*seconds_counter);

    p1window.draw(*p1score);
    p1window.draw(*p2score);
}

void TronGameState::p2render(sf::RenderWindow& p2window) {
    //render for snake game state
    p1.render(p2window);
    p2.render(p2window);

    p2window.draw(*title_text);
    p2window.draw(*play_text);
    p2window.draw(*seconds_counter);

    p2window.draw(*p1score);
    p2window.draw(*p2score);
}