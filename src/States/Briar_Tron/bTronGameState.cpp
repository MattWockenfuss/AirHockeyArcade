#include "bTronGameState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

#include "../../Context.hpp"
#include "../GameStateManager.hpp"
#include "../../AssetManager.hpp"
#include "../../KeyManager.hpp"
#include "../../IO/InputManager.hpp"
#include "../../AudioManager.hpp"
#include "../../States/Leaderboard/LeaderboardInterface.hpp"

Bike::Bike(std::string name, int x, int y, int dir){
	this->name = name;
	this->score = 0;
	this->x = x;
	this->y = y;
	this->dir = dir;
}
void Bike::draw1(sf::RenderWindow* window1, sf::Font font){
	// vars
	double screenRatio = (double)(window1 -> getSize().x) / 320.0;
	// player name
	sf::Text text(font, name, 16*screenRatio);
	text.setFillColor(sf::Color(0,0,200));
	sf::FloatRect rect = text.getLocalBounds();
	text.setPosition(sf::Vector2f(2.0*screenRatio , 1.0*screenRatio));
	window1 -> draw(text);
	// player score
	text.setString(std::to_string(score));
	text.setPosition(sf::Vector2f(rect.size.x + 8*screenRatio, 2*screenRatio));
	window1 -> draw(text);
	
	// player bike
	sf::RectangleShape bRect(sf::Vector2f(0.0f,0.0f));
	bRect.setFillColor(sf::Color(0,0,255));
	if(dir==0 || dir==2)
		bRect.setSize(sf::Vector2f(2.0*screenRatio,6.0*screenRatio));
	else
		bRect.setSize(sf::Vector2f(6*screenRatio,2*screenRatio));
	bRect.setPosition(sf::Vector2f( (2*x+16)*screenRatio + (dir==3?-4*screenRatio:0) , (2*y+20)*screenRatio + (dir==0?-4*screenRatio:0) ));
	window1->draw(bRect);
}
void Bike::draw2(sf::RenderWindow* window2, sf::Font font){
	// vars
	double screenRatio = (double)(window2 -> getSize().x) / 320.0;
	// player name
	sf::Text text(font, name, 16*screenRatio);
	text.setFillColor(sf::Color(200,0,0));
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(sf::Vector2f(rect.size.x,0));
	text.setPosition(sf::Vector2f(318.0*screenRatio , 1.0*screenRatio));
	window2 -> draw(text);
	// player score
	float txPos = 310.0*screenRatio - rect.size.x;
	text.setString(std::to_string(score));
	rect = text.getLocalBounds();
	text.setOrigin(sf::Vector2f(rect.size.x,0));
	text.setPosition(sf::Vector2f(txPos, 2.0*screenRatio));
	window2 -> draw(text);
	
	// player bike
	sf::RectangleShape bRect(sf::Vector2f(0.0f,0.0f));
	bRect.setFillColor(sf::Color(255,0,0));
	if(dir==0 || dir==2)
		bRect.setSize(sf::Vector2f(2*screenRatio,6*screenRatio));
	else
		bRect.setSize(sf::Vector2f(6*screenRatio,2*screenRatio));
	bRect.setPosition(sf::Vector2f( (2*x+16)*screenRatio + (dir==3?-4*screenRatio:0) , (2*y+20)*screenRatio + (dir==0?-4*screenRatio:0) ));
	window2->draw(bRect);
}

void bTronGameState::moveObjects(Bike* player1, Bike* player2){
	
}

void bTronGameState::init(Context* ctx){
    State::init(ctx);
	
	// debugging
    std::cout << "\bTronGameState Created!" << std::endl;
    std::cout << "Consolas" << &ctx -> assets -> getFont("Consolas") << std::endl;
    std::cout << "ST-SimpleSquare" << &ctx -> assets -> getFont("ST-SimpleSquare") << std::endl;
    
    float width = ctx -> p1window -> getSize().x;
    float height = ctx -> p1window -> getSize().y;
	screenRatio = width / 320.0;
    std::cout << "Screen Ratio: " << screenRatio << std::endl;
	
	// player names
	if(ctx -> p1name != "") // set player name while making sure the placeholder is not removed if necessary
		player1.name = ctx->p1name;
	if(ctx -> p2name != "")
		player2.name = ctx->p2name;
	
	// set walls of game grid
	for(int i = 0; i<gridSX; i++){
		gameGrid.push_back(*(new std::vector<int>));
		for(int j = 0; j<gridSY; j++){
			if(i==0 || j==0 || i==gridSX-1 || j==gridSY-1)
				gameGrid[i].push_back(3);
			else
				gameGrid[i].push_back(0);
		}
	}
	
	
	// on-screen message initializing
	//errMsg_1.emplace(ctx->assets->getFont("ST-SimpleSquare"), "Oops, Something Went Wrong!", 10*screenRatio);
	//sf::FloatRect rect = errMsg_1->getLocalBounds();
	//errMsg_1->setOrigin(sf::Vector2f( rect.getCenter().x, rect.getCenter().y)); // centered horizontally
	//errMsg_1->setPosition(sf::Vector2f(160.0*screenRatio , 70.0*screenRatio));
}

void bTronGameState::tick() {
    time = clock.restart();
    dt = time.asSeconds();
	
	//bike movement (need to virtually unpress keys so only one press is registered at a time
    // player 1
	//if (ctx -> input -> P1_Up && !W){
    //    if(p1paddle.yPos>=11)
    //        p1paddle.yPos -= 1;
    //    Up = true;
    //}
    //if (!ctx -> input -> P1_Up && W){
    //    Up = false;
    //}
    //if (ctx -> input -> P1_Down && !S){
    //    if(p1paddle.yPos<=11)
    //        p1paddle.yPos += 1;
    //    Down = true;
    //}
    //if (!ctx -> input -> P1_Down && S){
    //    Down = false;
    //}
    //if (ctx -> input -> P1_Left && !A){
    //    if(p1paddle.xPos>=2)
    //        p1paddle.xPos -= 1;
    //    Left = true;
    //}
    //if (!ctx -> input -> P1_Left && A){
    //    Left = false;
    //}
    //if (ctx -> input -> P1_Right && !D){
    //    if(p1paddle.xPos <= 4)
    //        p1paddle.xPos += 1;
    //    Right = true;
    //}
    //if (!ctx -> input -> P1_Right && D){
    //    Right = false;
    //}
    //// player 2
	//if (ctx -> input -> P2_Up && !UP){
	//	if(p2paddle.yPos<=2)
    //        p2paddle.yPos += 1;
    //    W = true;
    //}
    //if (!ctx -> input -> P2_Up && UP){
    //    W = false;
    //}
    //if (ctx -> input -> P2_Down && !DOWN){
    //    if(p2paddle.yPos>=2)
    //        p2paddle.yPos -= 1;
    //    S = true;
    //}
    //if (!ctx -> input -> P2_Down && DOWN){
    //    S = false;
    //}
    //if (ctx -> input -> P2_Left && !LEFT){
	//	if(p2paddle.xPos<=4)
    //        p2paddle.xPos += 1;
    //    A = true;
    //}
    //if (!ctx -> input -> P2_Left && LEFT){
    //    A = false;
    //}
    //if (ctx -> input -> P2_Right && !RIGHT){
    //    if(p2paddle.xPos>=2)
    //        p2paddle.xPos -= 1;
    //    D = true;
    //}
    //if (!ctx -> input -> P2_Right && RIGHT){
    //    D = false;
    //}
    //
	// emergency game exit
	if(ctx->input->P1B && ctx->input->P1Y && ctx->input->P2B && ctx->input->P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
		ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
	}
	
	// kickoff
    if(kickoff!=0){
		
	}
	
	moveObjects(&player1, &player2);
}

void bTronGameState::p1render(sf::RenderWindow& p1window) {
    p1window.clear();
	
	// draw background
	sf::RectangleShape rect(sf::Vector2f(2*screenRatio,180*screenRatio));
	rect.setFillColor(sf::Color(45,45,45));
	for(int i = 0; i<320; i++){
		if(i%16==12){
			rect.setPosition(sf::Vector2f((i-1)*screenRatio,0));
			p1window.draw(rect);
		}
	}
	rect.setSize(sf::Vector2f(320*screenRatio,2*screenRatio));
	for(int i = 0; i<180; i++){
		if(i%16==8){
			rect.setPosition(sf::Vector2f(0,(i-1)*screenRatio));
			p1window.draw(rect);
		}
	}
	
	// trails and walls
	rect.setSize(sf::Vector2f(2*screenRatio,2*screenRatio));
	for(int i = 0; i<gridSX; i++){
		for(int j = 0; j<gridSY; j++){
			rect.setPosition(sf::Vector2f( (2*i+16)*screenRatio , (2*j+20)*screenRatio ));
			switch(gameGrid[i][j]){
				case 1:
					rect.setFillColor(sf::Color(0,0,200)); // player 1 trail
					break;
				case 2:
					rect.setFillColor(sf::Color(200,0,0)); // player 2 trail
					break;
				case 3:
					rect.setFillColor(sf::Color(255,255,255)); // wall
					break;
				default:
					continue; // not wall, skip drawing
			}
			p1window.draw(rect);
		}
	}
	
	// bikes / names / scores
	player1.draw1(&p1window, ctx->assets->getFont("ST-SimpleSquare"));
	player2.draw2(&p1window, ctx->assets->getFont("ST-SimpleSquare"));
}

void bTronGameState::p2render(sf::RenderWindow& p2window) {
    p2window.clear();
	
	// copy/paste p1render, then switch all draw1 functions with draw2, and vice-versa
}