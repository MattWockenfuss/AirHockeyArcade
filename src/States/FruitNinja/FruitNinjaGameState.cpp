#include "FruitNinjaGameState.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../../Context.hpp"
#include "../GameStateManager.hpp"
#include "../../AssetManager.hpp"
#include "../../KeyManager.hpp"
#include "../../IO/InputManager.hpp"

Fruit::Fruit(sf::Texture tileSet, int type, int state, int x, int y, int w, int h, double vx, double vy){
	this->tileSet = tileSet;
	this->type = type;
	this->state = state;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->vx = vx;
	this->vy = vy;
	this->frame = 0;
	this->time = 0.0;
}
void Fruit::move(float dt){
	time += dt;
	int n = round(time/0.075f); // account for lag where dt may be larger than the 0.075 time step we want
	for(int i = 0; i<n; i++){
		vy += 0.1;
		y += vy;
		x += vx;
	}
	frame = (frame + n)%6;
	time = std::remainder(time,0.075); // bring time back down while including any carry over between time steps
}
void Fruit::draw(sf::RenderWindow* window, double screenRatio){
	sf::Sprite sprite(tileSet);
	sprite.setScale(sf::Vector2f(screenRatio,screenRatio));
	sprite.setTextureRect(sf::IntRect( sf::Vector2(w*frame,h*state), sf::Vector2(w,h) )); // change this will change image we draw from tile set
	sprite.setPosition(sf::Vector2f( ((27-(w/2))+x)*screenRatio , y*screenRatio ));
	window->draw(sprite);
}

ScorePoint::ScorePoint(int score, int x, int y){
	this->score = score;
	this->x = x;
	this->y = y;
	this->opacity = 255;
}
void ScorePoint::draw(sf::RenderWindow* window, double screenRatio, sf::Font font){
	std::string points = std::to_string(score)+"0";
	sf::Text text(font,points,6*screenRatio);
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.getCenter());
	text.setPosition(sf::Vector2f(x*screenRatio,y*screenRatio));
	sf::Color color(255,255,255,opacity);
	text.setFillColor(color);
	window->draw(text);
}

void FruitNinjaGameState::init(Context* ctx){
	State::init(ctx);
	
	// debugging
    std::cout << "\nFruitNinjaGameState Created!" << std::endl;
    std::cout << "Consolas" << &ctx -> assets -> getFont("Consolas") << std::endl;
    std::cout << "ST-SimpleSquare" << &ctx -> assets -> getFont("ST-SimpleSquare") << std::endl;
	
	width = ctx -> p1window -> getSize().x;
    height = ctx -> p1window -> getSize().y;
	screenRatio = width / 320.0;
    //screenRatio = 6;
    std::cout << "Screen Ratio: " << screenRatio << std::endl;
	
	backTex.emplace(ctx->assets->getAsset("background"));
	redTex.emplace(ctx->assets->getAsset("redGuy_tileSet"));
	mellonTex.emplace(ctx->assets->getAsset("mellon_tileSet"));
	appleTex.emplace(ctx->assets->getAsset("apple_tileSet"));
	orangeTex.emplace(ctx->assets->getAsset("orange_tileSet"));
	lemonTex.emplace(ctx->assets->getAsset("lemon_tileSet"));
	fruitTexs.push_back(*mellonTex);
	fruitTexs.push_back(*appleTex);
	fruitTexs.push_back(*orangeTex);
	fruitTexs.push_back(*lemonTex);
	back.emplace(*backTex);
	back->setScale(sf::Vector2f(screenRatio,screenRatio));
	guy.emplace(*redTex);
	guy->setScale(sf::Vector2f(screenRatio,screenRatio));
	
	std::cout << "\nInit() Succeeded!" << std::endl;
	
	// text
	totalPointText.emplace(ctx->assets->getFont("ST-SimpleSquare"),"0",10*screenRatio);
	totalPointText->setPosition(sf::Vector2f(8*screenRatio,0));
}

void FruitNinjaGameState::tick(){	
	time = clock.restart();
	dt = time.asSeconds();
	
	if (ctx->input->P1_Left && !guySwing && guyMove==0){ // move left
		int x = guyX;
		int y = -1;
		for(int i = 0; i<fruits.size(); i++){
			if(fruits[i]->y > y && fruits[i]->y < 133 && fruits[i]->state==0){
				x = fruits[i]->x/38;
				y = fruits[i]->y;
			}
		}
		if(guyX>0 && x<guyX){
			guyX = x;
		}
	}
	if (ctx->input->P1_Right && !guySwing && guyMove==0){ // move right
		int x = guyX;
		int y = -1;
		for(int i = 0; i<fruits.size(); i++){
			if(fruits[i]->y > y && fruits[i]->y < 133 && fruits[i]->state==0){
				x = fruits[i]->x/38;
				y = fruits[i]->y;
			}
		}
		if(guyX<7 && x>guyX){
			guyX = x;
		}
	}
	if (ctx->input->P1A && !guySwing && guyMove==0){ // swing sword
		guySwing = true;
		guyTime = 0;
		guyFrame = 1;
	}

	// animate guy
	if(guySwing){
		guyTime += dt;
		int n = round(guyTime/0.075f); // change frames every 0.075s (also account for lag that may cause a frame to be longer than 0.075s
		guyTime = std::remainder(guyTime,0.075f); // reset guyTime while retaining any overflow
		guyFrame += n;
		if(guyFrame==2 && !cutFruit){ // check if we cut fruit
			int i;
			for(i = 0; i<fruits.size(); i++){
				if(fruits[i]->x/38==guyX && fruits[i]->state==0){ // if there is a whole fruit in the same column as the player
					fruitHeight = fruits[i]->y;
					if(fruitHeight <= 133 && fruitHeight + fruits[i]->h >= 123){ // if the fruit is within the maximum cutting range
						score = 1;
						fruitHeight += fruits[i]->h/2;
						if(fruitHeight >= 123 && fruitHeight <= 133){ // if the middle of the fruit is within the cutting range
							score = 2;
							if(fruitHeight >= 125 && fruitHeight <= 130){ // if the middle of the fruit is within the cutting spark
								score = 3;
							}
						}
						score *= 5;
						scorePoints.push_back( new ScorePoint(score,guyX*38+26,120) );
						
						totalPoints += score;
						std::string str = std::to_string(totalPoints)+"0";
						totalPointText->setString(str);
						
						cutFruit = true;
						break;
					}
				}
			}
			if(cutFruit){
				int type = fruits[i]->type;
				int x = fruits[i]->x;
				int y = fruits[i]->y;
				int w = fruits[i]->w;
				int h = fruits[i]->h;
				if(type==0){ // watermellon width changes when cut, other fruits do not
					fruits.push_back( new Fruit(fruitTexs[type],type,1,x-w/2,y,16,h,-0.5,-1) );
					fruits.push_back( new Fruit(fruitTexs[type],type,2,x+w/2,y,16,h,0.5,-1) );
				} else {
					fruits.push_back( new Fruit(fruitTexs[type],type,1,x-w/2,y,w,h,-0.5,-1) );
					fruits.push_back( new Fruit(fruitTexs[type],type,2,x+w/2,y,w,h,0.5,-1) );
				}
				fruits[i]->y = 200*screenRatio;
			}
		}
		if(guyFrame >= 4 + cutFruit?1:0){ // stop animation
			guySwing = false;
			guyFacing = (guyFacing+1)%2; // will alternate between o and 1
			guyFrame = 0;
			cutFruit = false;
		}
		if(guyFrame==3 && cutFruit){ // move to the successful cut frame if we cut the fruit
			guyFrame = 4;
		}
	}
	if(guyMove!=0){
		guyStep += dt;
		if(guyStep>=0.2){
			guyX += guyMove;
			guyMove = 0;
			guyFrame = 0;
		}
	}
	
	// update objects
	// guy
	guy->setTextureRect(sf::IntRect( sf::Vector2(48*guyFrame,48*guyFacing), sf::Vector2(48,48) )); // change this will change image we draw from tile set
	guy->setPosition(sf::Vector2f( (3+(38*(guyX + (guyMove*guyStep/0.2) )))*screenRatio , height-(58*screenRatio) ));
	// spawn fruit
	if(fruitTime <= 0){
		// set random values for the fruit
		fruitType = rand()%4;
		fruitX = rand()%8;
		// spawn fruit
		if(fruitType==0)
			fruits.push_back( new Fruit(fruitTexs[fruitType],fruitType,0,fruitX*38,-16,20,16,0,0) ); // drop mellon
		else
			fruits.push_back( new Fruit(fruitTexs[fruitType],fruitType,0,fruitX*38,-8,8,8,0,0) ); // drop other fruit (they are all the same size)
		// set time delay before next fruit
		fruitTime = ((rand()%3)+1)*0.4;
	}
	fruitTime -= dt;
	// move fruit
	fallenFruit = -1;
	for(int i = 0; i<fruits.size(); i++){
		fruits[i]->move(dt);
		if(fruits[i]->y >= 180*screenRatio){
			fallenFruit = i;
		}
	}
	// remove fallen fruit
	if(fallenFruit!=-1)
		fruits.erase(fruits.begin()+fallenFruit);
	
	// move score points
	int erasePoint = -1;
	for(int i = 0; i<scorePoints.size(); i++){
		scorePoints[i]->y--;
		scorePoints[i]->opacity -= 4;
		if(scorePoints[i]->opacity<=0)
			erasePoint = i;
	}
	// erase score points
	if(erasePoint!=-1)
		scorePoints.erase(scorePoints.begin()+erasePoint);
}

void FruitNinjaGameState::p1render(sf::RenderWindow& p1window){
	p1window.clear();
	// background
	p1window.draw(*back);
	// fruit
	for(int i = 0; i<fruits.size(); i++){
		fruits[i]->draw(&p1window, screenRatio);
	}
	// guy
	p1window.draw(*guy);
	// points
	for(int i = 0; i<scorePoints.size(); i++)
		scorePoints[i]->draw(&p1window,screenRatio,ctx->assets->getFont("ST-SimpleSquare"));
	p1window.draw(*totalPointText);
}

void FruitNinjaGameState::p2render(sf::RenderWindow& p2window){
	p2window.clear();
	// background
	p2window.draw(*back);
	// fruit
	for(int i = 0; i<fruits.size(); i++){
		fruits[i]->draw(&p2window, screenRatio);
	}
	// guy
	p2window.draw(*guy);
	// points
	for(int i = 0; i<scorePoints.size(); i++)
		scorePoints[i]->draw(&p2window,screenRatio,ctx->assets->getFont("ST-SimpleSquare"));
	p2window.draw(*totalPointText);
}