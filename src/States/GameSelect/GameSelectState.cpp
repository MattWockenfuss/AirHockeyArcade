#include "GameSelectState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>


#include "../../Context.hpp"
#include "../../AssetManager.hpp"
#include "../../KeyManager.hpp"
#include "../../IO/InputManager.hpp"
#include "../../AudioManager.hpp"
#include "../GameStateManager.hpp"

std::vector<int> getIndexes(int center, int size){ // determine which 3 games to display in the rotating menu (size must be at least 1)
	// create indexes
	int trueCenter = center%size; // wrap the center around if it exceeds the size
	std::vector<int> indexes = {trueCenter-1,trueCenter,trueCenter+1};
	// wrap around if needed
	if(indexes[0]<0)
		indexes[0] = size-1;
	if(indexes[2]>=size)
		indexes[2] = 0;
	
	return indexes;
}

std::vector<double> getAnimXYZ(float frame){
	// 0s   -> (-192,56,80)
	// 0.5s -> (-96,48,96)
	// 1.5s -> (96,32,128)
	// 2.5s -> (288,48,96)
	// 3s   -> (384,56,80)
	
	double x = -192.0 + (192*frame);
	if(frame<1.5){ // distance is smaller for element left of center
		x = -144.0 + (160*frame);
	}
	double y = 32 + 16*sqrt( pow(1.5-frame,2.0));
	double z = 128 - 32*sqrt( pow(1.5-frame,2.0));
	std::vector<double> xyz = {x,y,z};
	return xyz;
}

GameOption::GameOption(sf::String name, sf::String desc, sf::Texture img, sf::Color foreColor, sf::Color backColor){
	this->name = name;
	this->desc = desc;
	this->image = img;
	this->foreColor = foreColor;
	this->backColor = backColor;
}

void GameSelectState::init(Context* ctx){
	State::init(ctx);
	
	// debugging
    std::cout << "\nGame Select State Created!" << std::endl;
	
	// screen size
	width = ctx -> p1window -> getSize().x;
    height = ctx -> p1window -> getSize().y;
	screenRatio = width / 320.0;
    //screenRatio = 6;
    std::cout << "Screen Ratio: " << screenRatio << std::endl;
	
	// game options
													// this is the longest description we can have before it goes off the screen
	gameOptions.push_back( GameOption("Air Hockey", "Play first to 11 in this arcade recreation of your favorite game!", ctx->assets->getAsset("AirHockeyIcon"), sf::Color(19,0,223), sf::Color(155,0,0) ));
	gameOptions.push_back( GameOption("Tron", "Gameplay Coming Soon!", ctx->assets->getAsset("CoffeeIcon"), sf::Color(202,208,210), sf::Color(91,63,50) ));
	gameOptions.push_back( GameOption("Fruit Warrior", "Cut Fruit To The Rythm, In This Fast-Paced Competition!", ctx->assets->getAsset("FruitIcon"), sf::Color(255,108,5), sf::Color(255,252,0) ));
	gameOptions.push_back( GameOption("Pong", "Relive old memories and play Pong against your friends!", ctx->assets->getAsset("PongIcon"), sf::Color(222,19,195), sf::Color(12,59,6) ));
	gameOptions.push_back( GameOption("Leaderboard", "View top scores across all games!", ctx->assets->getAsset("LeaderboardIcon"), sf::Color(250,186,24), sf::Color(205,152,20) ));
	gameOptions.push_back( GameOption("Exit", "Stop playing for now", ctx->assets->getAsset("ExitIcon"), sf::Color(214,0,0), sf::Color(155,0,0) ));
	size = gameOptions.size();
	
	// textures
	arrows.push_back(ctx -> assets -> getAsset("gameSelectArrow_L1"));
	arrows.push_back(ctx -> assets -> getAsset("gameSelectArrow_L2"));
	arrows.push_back(ctx -> assets -> getAsset("gameSelectArrow_R1"));
	arrows.push_back(ctx -> assets -> getAsset("gameSelectArrow_R2"));
	
	arrowSprite.emplace(arrows[0]);
	arrowSprite->setScale(sf::Vector2f(screenRatio,screenRatio));
	iconSprite.emplace(arrows[0]); // this sprite will never use this texture, but it does need to be initialized with something
	
	// text
	title.emplace(ctx->assets->getFont("ST-SimpleSquare"), "", 24*screenRatio);
	description.emplace(ctx->assets->getFont("SquareSansSerif"),"",7*screenRatio);
	
	msg.emplace(ctx->assets->getFont("SquareSansSerif"),"To Exit The Game At Any Time, Press",10*screenRatio);
	textRect = msg->getLocalBounds();
	msg->setOrigin(sf::Vector2f(textRect.getCenter().x, textRect.getCenter().y));
	msg->setPosition(sf::Vector2f(160*screenRatio, 75*screenRatio));
	p1msg.emplace(ctx->assets->getFont("SquareSansSerif"),"'B' and 'Y' At the Same Time As Your Opponent",10*screenRatio);
	textRect = p1msg->getLocalBounds();
	p1msg->setOrigin(sf::Vector2f(textRect.getCenter().x, textRect.getCenter().y));
	p1msg->setPosition(sf::Vector2f(160*screenRatio, 90*screenRatio));
	p2msg.emplace(ctx->assets->getFont("SquareSansSerif"),"'B' and 'X' At the Same Time As Your Opponent",10*screenRatio);
	textRect = p2msg->getLocalBounds();
	p2msg->setOrigin(sf::Vector2f(textRect.getCenter().x, textRect.getCenter().y));
	p2msg->setPosition(sf::Vector2f(160*screenRatio, 90*screenRatio));
	
	// initialize the scrolling menu
	indexes = getIndexes(selection,size);
	
	// stop sounds
	ctx -> audio -> stopSound();
}

void GameSelectState::tick(){
	time = clock.restart();
	dt = time.asSeconds();
	lap_Arrow += dt;
	 // inputs
	if(state==0 && ctx -> input -> P1_Left && animate==0){ // animate forward
		animate = 1;
	}
	if(state==0 && ctx -> input -> P1_Right && animate==0){ // animate backward
		animate = -1;
	}
	// enter games
	if(state==0 && ctx->input->P1A && animate==0){ // select game, start loading animation
		switch(selection){ // screens chosen here ignore the exit message, otherwise, they go through the extra animation
			//case 4: // Leaderboard
			//	ctx -> gsm -> requestStateChange(States::Leaderboard, 1.5f, 1.5f);
			//	break;
			case 5: // Exit
				ctx -> gsm -> requestStateChange(States::Idle, 1.5f, 1.5f);
				break;
			default:
				state = 1;
				timer = 0;
				break;
		}
	}
	// state logic
	if(state!=0){
		timer += dt;
		if(timer >= 1.5){
			timer = 0;
			state++;
			if(state==4){ // loading animation finished, switch to the selected game
				switch(selection){
					case 0: // air hockey
						ctx -> gsm -> requestStateChange(States::AirHockey, 1.5f, 1.5f);
						break;
					case 1: // Tron
						ctx -> gsm -> requestStateChange(States::Tron, 1.5f, 1.5f);
						break;
					case 2: // Fruit Ninja
						ctx -> gsm -> requestStateChange(States::FruitNinja, 1.5f, 1.5f);
						break;
					case 3: // Pong
						ctx -> gsm -> requestStateChange(States::Pong, 1.5f, 1.5f);
						break;
					case 4: // Leaderboard
						ctx -> gsm -> requestStateChange(States::Leaderboard, 1.5f, 1.5f);
						break;
					case 5: // Exit
						ctx -> gsm -> requestStateChange(States::Idle, 1.5f, 1.5f);
						break;
				}
			}
		}
		
	}
	
	// animation logic
	if(animate==1){
		for(int i = 0; i<3; i++){
			frames[i] += dt;
			// wrap around
			if(frames[i]>3)
				frames[i] -= 3;
			// change selection
			if(frames[0]>=1 && frames[0]-dt<1){
				frames[0] = 0.0;
				frames[1] = 1.0;
				frames[2] = 2.0;
				selection -= 1;
				if(selection<0)
					selection = size-1;
				indexes = getIndexes(selection,size);
			}
			// pause animation
			if(frames[1]>=1.5 && frames[1]-dt<1.5){
				frames[0] = 0.5;
				frames[1] = 1.5;
				frames[2] = 2.5;
				animate = 0;
			}
		}
	}
	if(animate==-1){
		for(int i = 0; i<3; i++){
			frames[i] -= dt;
			// wrap around
			if(frames[i]<0)
				frames[i] += 3;
			// change selection
			if(frames[2]<=2 && frames[2]+dt>2){
				frames[0] = 1.0;
				frames[1] = 2.0;
				frames[2] = 3.0;
				selection += 1;
				if(selection>=size)
					selection = 0;
				indexes = getIndexes(selection,size);
			}
			// pause animation
			if(frames[1]>=1.5 && frames[1]-dt<1.5){
				frames[0] = 0.5;
				frames[1] = 1.5;
				frames[2] = 2.5;
				animate = 0;
			}
		}
	}
	
	// set title text
	title->setString(gameOptions[selection].name);
	textRect = title->getLocalBounds();
	title->setOrigin(textRect.getCenter());
	
	description->setString(gameOptions[selection].desc);
	textRect = description->getLocalBounds();
	description->setOrigin(textRect.getCenter());
	
	// keep lap time from going too high
	if(lap_Arrow>=1.5)
		lap_Arrow = 0;
}

void GameSelectState::p1render(sf::RenderWindow& p1window) {
	// title
	title->setPosition(sf::Vector2f( (width/2)+screenRatio , (14+1)*screenRatio ));
	title->setFillColor(gameOptions[selection].backColor);
	p1window.draw(*title);
	
	title->setPosition(sf::Vector2f( width/2 , 14*screenRatio ));
	title->setFillColor(gameOptions[selection].foreColor);
	p1window.draw(*title);
	
	// description
	description->setPosition(sf::Vector2f( width/2 , height - (14*screenRatio) ));
	description->setFillColor(gameOptions[selection].foreColor);
	p1window.draw(*description);
	
	// arrows
	if(lap_Arrow<=0.75){ // alternate frames every 0.75s
		// left arrow
		arrowSprite->setTexture(arrows[0], false); // false keeps the same sprite rectangle with the new texture // each texture is the same size, so this works
		arrowSprite->setPosition(sf::Vector2f(48.0*screenRatio,64.0*screenRatio));
		p1window.draw(*arrowSprite);
		// right arrow
		arrowSprite->setTexture(arrows[2], false); // false keeps the same sprite rectangle with the new texture // each texture is the same size, so this works
		arrowSprite->setPosition(sf::Vector2f(240.0*screenRatio,64.0*screenRatio));
		p1window.draw(*arrowSprite);
	}
	else{
		// left arrow
		arrowSprite->setTexture(arrows[1], false); // false keeps the same sprite rectangle with the new texture // each texture is the same size, so this works
		arrowSprite->setPosition(sf::Vector2f(48.0*screenRatio,64.0*screenRatio));
		p1window.draw(*arrowSprite);
		// right arrow
		arrowSprite->setTexture(arrows[3], false); // false keeps the same sprite rectangle with the new texture // each texture is the same size, so this works
		arrowSprite->setPosition(sf::Vector2f(240.0*screenRatio,64.0*screenRatio));
		p1window.draw(*arrowSprite);
	}
	
	// icons
	for(int i = 0; i<3; i++){
		xyz = getAnimXYZ(frames[i]);
		iconSprite->setTexture(gameOptions[indexes[i]].image,true);
		iconSprite->setPosition(sf::Vector2f( xyz[0]*screenRatio , xyz[1]*screenRatio ));
		iconSprite->setScale(sf::Vector2f( xyz[2]/128*screenRatio , xyz[2]/128*screenRatio ));
		p1window.draw(*iconSprite);
	}
	
	// loading animation
	if(state==1){
		sf::RectangleShape r(sf::Vector2f(320*screenRatio, 180*screenRatio));
		r.setFillColor(sf::Color(0,0,0,255*timer/1.5));
		p1window.draw(r);
	}
	if(state==2){
		sf::RectangleShape r(sf::Vector2f(320*screenRatio, 180*screenRatio));
		r.setFillColor(sf::Color(0,0,0,255));
		p1window.draw(r);
		
		msg->setFillColor(sf::Color(255,255,255,255*timer/1.5));
		p1msg->setFillColor(sf::Color(255,255,255,255*timer/1.5));
		p1window.draw(*msg);
		p1window.draw(*p1msg);
	}
	if(state>2){
		sf::RectangleShape r(sf::Vector2f(320*screenRatio, 180*screenRatio));
		r.setFillColor(sf::Color(0,0,0,255));
		p1window.draw(r);
		
		msg->setFillColor(sf::Color(255,255,255,255));
		p1msg->setFillColor(sf::Color(255,255,255,255));
		p1window.draw(*msg);
		p1window.draw(*p1msg);
	}
}
void GameSelectState::p2render(sf::RenderWindow& p2window){ // call the first render function using the p2 window because the screens should be the same
	// title
	title->setPosition(sf::Vector2f( (width/2)+screenRatio , (14+1)*screenRatio ));
	title->setFillColor(gameOptions[selection].backColor);
	p2window.draw(*title);
	
	title->setPosition(sf::Vector2f( width/2 , 14*screenRatio ));
	title->setFillColor(gameOptions[selection].foreColor);
	p2window.draw(*title);
	
	// description
	description->setPosition(sf::Vector2f( width/2 , height - (14*screenRatio) ));
	description->setFillColor(gameOptions[selection].foreColor);
	p2window.draw(*description);
	
	// arrows
	if(lap_Arrow<=0.75){ // alternate frames every 0.75s
		// left arrow
		arrowSprite->setTexture(arrows[0], false); // false keeps the same sprite rectangle with the new texture // each texture is the same size, so this works
		arrowSprite->setPosition(sf::Vector2f(48.0*screenRatio,64.0*screenRatio));
		p2window.draw(*arrowSprite);
		// right arrow
		arrowSprite->setTexture(arrows[2], false); // false keeps the same sprite rectangle with the new texture // each texture is the same size, so this works
		arrowSprite->setPosition(sf::Vector2f(240.0*screenRatio,64.0*screenRatio));
		p2window.draw(*arrowSprite);
	}
	else{
		// left arrow
		arrowSprite->setTexture(arrows[1], false); // false keeps the same sprite rectangle with the new texture // each texture is the same size, so this works
		arrowSprite->setPosition(sf::Vector2f(48.0*screenRatio,64.0*screenRatio));
		p2window.draw(*arrowSprite);
		// right arrow
		arrowSprite->setTexture(arrows[3], false); // false keeps the same sprite rectangle with the new texture // each texture is the same size, so this works
		arrowSprite->setPosition(sf::Vector2f(240.0*screenRatio,64.0*screenRatio));
		p2window.draw(*arrowSprite);
	}
	
	// icons
	for(int i = 0; i<3; i++){
		xyz = getAnimXYZ(frames[i]);
		iconSprite->setTexture(gameOptions[indexes[i]].image,true);
		iconSprite->setPosition(sf::Vector2f( xyz[0]*screenRatio , xyz[1]*screenRatio ));
		iconSprite->setScale(sf::Vector2f( xyz[2]/128*screenRatio , xyz[2]/128*screenRatio ));
		p2window.draw(*iconSprite);
	}
	
	// loading animation
	if(state==1){
		sf::RectangleShape r(sf::Vector2f(320*screenRatio, 180*screenRatio));
		r.setFillColor(sf::Color(0,0,0,255*timer/1.5));
		p2window.draw(r);
	}
	if(state==2){
		sf::RectangleShape r(sf::Vector2f(320*screenRatio, 180*screenRatio));
		r.setFillColor(sf::Color(0,0,0,255));
		p2window.draw(r);
		
		msg->setFillColor(sf::Color(255,255,255,255*timer/1.5));
		p2msg->setFillColor(sf::Color(255,255,255,255*timer/1.5));
		p2window.draw(*msg);
		p2window.draw(*p2msg);
	}
	if(state>2){
		sf::RectangleShape r(sf::Vector2f(320*screenRatio, 180*screenRatio));
		r.setFillColor(sf::Color(0,0,0,255));
		p2window.draw(r);
		
		msg->setFillColor(sf::Color(255,255,255,255));
		p2msg->setFillColor(sf::Color(255,255,255,255));
		p2window.draw(*msg);
		p2window.draw(*p2msg);
	}
}