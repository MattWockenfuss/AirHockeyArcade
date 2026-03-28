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
	greenTex.emplace(ctx->assets->getAsset("greenGuy_tileSet"));
	mellonTex.emplace(ctx->assets->getAsset("mellon_tileSet"));
	appleTex.emplace(ctx->assets->getAsset("apple_tileSet"));
	orangeTex.emplace(ctx->assets->getAsset("orange_tileSet"));
	lemonTex.emplace(ctx->assets->getAsset("lemon_tileSet"));
	fruitTexs.push_back(*mellonTex);
	fruitTexs.push_back(*appleTex);
	fruitTexs.push_back(*orangeTex);
	fruitTexs.push_back(*lemonTex);
	std::cout<<"Fruits Pushed!"<<std::endl;
	back.emplace(*backTex);
	back->setScale(sf::Vector2f(screenRatio,screenRatio));
	guy.emplace(*redTex);
	guy->setScale(sf::Vector2f(screenRatio,screenRatio));
	std::cout<<"Sprites Created!"<<std::endl;
}

void FruitNinjaGameState::tick(){
	//change state
	if(ctx->keys->F3){
		ctx->gsm->requestStateChange(States::Tron);
	}
	if(ctx->keys->F4){
		ctx->gsm->requestStateChange(States::AirHockey);
	}
	
	time = clock.restart();
	dt = time.asSeconds();
	
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab) && guyCanChange==false){ // prepare to change shirt color
				guyCanChange = true;
			}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab) && guyCanChange==true){ // change shirt color
		if(guyRed){
			guy->setTexture(*greenTex,false);
			guyRed = false;
		}
		else{
			guy->setTexture(*redTex,false);
			guyRed = true;
		}
		guyCanChange = false;
	}
	if (ctx->input->P1_Left && !guySwing && guyMove==0){ // move left
		if(guyRed && guyX>0){
			guyMove = -1;
			guyFrame = 6;
			guyStep = 0;
		} else {
			int x = guyX;
			int y = -1;
			for(int i = 0; i<fruits.size(); i++){
				if(fruits[i]->y > y && fruits[i]->y < 133 && fruits[i]->state==0){
					x = fruits[i]->x/38;
					y = fruits[i]->y;
				}
			}
			if(!guyRed && guyX>0 && x<guyX){
				guyX = x;
			}
		}
	}
	if (ctx->input->P1_Right && !guySwing && guyMove==0){ // move right
		if(guyRed && guyX<7){
			guyMove = 1;
			guyFrame = 5;
			guyStep = 0;
		} else {
			int x = guyX;
			int y = -1;
			for(int i = 0; i<fruits.size(); i++){
				if(fruits[i]->y > y && fruits[i]->y < 133 && fruits[i]->state==0){
					x = fruits[i]->x/38;
					y = fruits[i]->y;
				}
			}
			if(!guyRed && guyX<7 && x>guyX){
				guyX = x;
			}
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
		if(guyFrame==2 && !cutFruit){
			int i;
			for(i = 0; i<fruits.size(); i++){
				if(fruits[i]->x/38==guyX && fruits[i]->y+fruits[i]->h > 122 && fruits[i]->y+fruits[i]->h <= 133 && fruits[i]->state==0){
					cutFruit = true;
					break;
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
		if(guyFrame==3 && cutFruit){ // did we cut fruit?
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
		fruitType = rand()%4;
		if(fruitType==0){
			fruits.push_back( new Fruit(fruitTexs[fruitType],fruitType,0,fruitX*38,-16,20,16,0,0) ); // drop mellon
		}
		else{
			fruits.push_back( new Fruit(fruitTexs[fruitType],fruitType,0,fruitX*38,-8,8,8,0,0) ); // drop other fruit (they are all the same size)
		}
		
		fruitTime = rand()%2; // choose whether the new fruit will fall to the left or to the right
		if(fruitTime==0){ // move the fruit a ranom number of columns (stay close enough that it is reachable within time)
			fruitTime = fruitDist[ rand()%( fruitDist.size() ) ];
			//fruitTime = rand()&10; // random column movement between 0 and 9
			fruitX += fruitTime;
			if(fruitX>7)
				fruitX = ((fruitX-7)*-1)+7; // reflect off of walls
				fruitX = 7;
		}
		else{ // move in the other direction
			fruitTime = fruitDist[ rand()%( fruitDist.size() ) ];
			//fruitTime = rand()&10; // random column movement between 0 and 9
			fruitX -= fruitTime;
			if(fruitX<0)
				fruitX = fruitX*-1; // reflect off of walls
		}
		if(guyRed){ // original movement style
			fruitTime = (fruitTime+1)*0.4; // turn random column movement into random time spacing between fruits of 0.4s (same column) to 4.0s (any column plus a pause)
		} else { // alternate movement style
			fruitTime = ((rand()%3)+1)*0.4;
		}
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
	if(fallenFruit!=-1){
		fruits.erase(fruits.begin()+fallenFruit);
	}
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
}