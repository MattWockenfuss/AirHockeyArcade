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
void Fruit::draw(sf::RenderWindow* window, double screenRatio, sf::Font font){
	sf::Sprite sprite(tileSet);
	sprite.setScale(sf::Vector2f(screenRatio,screenRatio));
	sprite.setTextureRect(sf::IntRect( sf::Vector2(w*frame,h*state), sf::Vector2(w,h) )); // change this will change image we draw from tile set
	sprite.setPosition(sf::Vector2f( (27+x-w/2)*screenRatio , y*screenRatio ));
	window->draw(sprite);
	
	if(state!=0) // don't draw label if the fruit was cut
		return;
	
	// draw button label above fruit
	sf::Text label(font,"",6*screenRatio);
	switch(type){ // set the label to the type of fruit
		case 0:
			label.setString("A");
			break;
		case 1:
			label.setString("B");
			break;
		case 2:
			label.setString("X");
			break;
		case 3:
			label.setString("Y");
			break;
	}
	sf::FloatRect rect = label.getLocalBounds();
	label.setOrigin(rect.getCenter());
	label.setPosition(sf::Vector2f( (27+x)*screenRatio , (y-rect.getCenter().y/4)*screenRatio ));
	window->draw(label);
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
	sf::Color color(255,255*(points[0]=='-'?0:1),255*(points[0]=='-'?0:1),opacity);
	text.setFillColor(color);
	window->draw(text);
}

void FruitNinjaInstance::drawName(sf::RenderWindow* window, double screenRatio, sf::Font font, bool opaque){
	sf::Text text(font,name,12*screenRatio);
	text.setPosition(sf::Vector2f(8*screenRatio,0));
	if(opaque)
		text.setFillColor(sf::Color(255,255,255,128));
	else
		text.setFillColor(sf::Color(255,255,255,255));
	window->draw(text);
}
void FruitNinjaInstance::drawTotalScore(sf::RenderWindow* window, double screenRatio, sf::Font font, bool opaque){
	std::string points = std::to_string(totalPoints)+"0";
	sf::Text text(font,points,12*screenRatio);
	text.setPosition(sf::Vector2f(8*screenRatio,12*screenRatio));
	if(opaque)
		text.setFillColor(sf::Color(255,255,255,128));
	else
		text.setFillColor(sf::Color(255,255,255,255));
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
	
	// text
	font = ctx->assets->getFont("ST-SimpleSquare");
	p1instance.name = ctx->p1name;
	//p2instance.emplace(ctx->p1name,font,screenRatio);
}

void FruitNinjaGameState::tick(){	
	time = clock.restart();
	dt = time.asSeconds();
	
	// p1instance controls
	if(ctx->input->P1_Left && p1instance.swing==-1){ // move left
		x = p1instance.x;
		y = -1;
		for(int i = 0; i<p1instance.fruits.size(); i++){
			if(p1instance.fruits[i]->y > y && p1instance.fruits[i]->y < 133 &&p1instance.fruits[i]->state==0){
				x = p1instance.fruits[i]->x/38;
				y = p1instance.fruits[i]->y;
			}
		}
		if(p1instance.x>0 && x<p1instance.x)
			p1instance.x = x;
	}
	if(ctx->input->P1_Right && p1instance.swing==-1){ // move right
		x = p1instance.x;
		y = -1;
		for(int i = 0; i<p1instance.fruits.size(); i++){
			if(p1instance.fruits[i]->y > y && p1instance.fruits[i]->y < 133 && p1instance.fruits[i]->state==0){
				x = p1instance.fruits[i]->x/38;
				y = p1instance.fruits[i]->y;
			}
		}
		if(p1instance.x<7 && x>p1instance.x)
			p1instance.x = x;
	}
	if (ctx->input->P1A && p1instance.swing == -1){ // swing sword 'A'
		p1instance.swing = 0;
		p1instance.timer = 0;
		p1instance.frame = 1;
	}
	if (ctx->input->P1B && p1instance.swing == -1){ // swing sword 'B'
		p1instance.swing = 1;
		p1instance.timer = 0;
		p1instance.frame = 1;
	}
	if (ctx->input->P1X && p1instance.swing == -1){ // swing sword 'X'
		p1instance.swing = 2;
		p1instance.timer = 0;
		p1instance.frame = 1;
	}
	if (ctx->input->P1Y && p1instance.swing == -1){ // swing sword 'Y'
		p1instance.swing = 3;
		p1instance.timer = 0;
		p1instance.frame = 1;
	}
	
	// animate p1instance guy
	if(p1instance.swing!=-1){
		p1instance.timer += dt;
		int n = round(p1instance.timer/0.075f); // change frames every 0.075s (also account for lag that may cause a frame to be longer than 0.075s
		p1instance.timer = std::remainder(p1instance.timer,0.075f); // reset timer while retaining any overflow
		p1instance.frame += n;
		if(p1instance.frame==2 && p1instance.cut==0){ // check if we cut fruit
			int i; // we need to reference i after the loop, so we initialize it before
			for(i = 0; i<p1instance.fruits.size(); i++){
				if(p1instance.fruits[i]->x/38==p1instance.x && p1instance.fruits[i]->state==0 && p1instance.fruits[i]->type==p1instance.swing){ // if there is a whole fruit in the same column as the player, and they swung at the correct fruit
					p1instance.fruitHeight = p1instance.fruits[i]->y;
					if(p1instance.fruitHeight <= 133 && p1instance.fruitHeight + p1instance.fruits[i]->h >= 123){ // if the fruit is within the maximum cutting range
						p1instance.score = 1;
						p1instance.fruitHeight += p1instance.fruits[i]->h/2;
						if(p1instance.fruitHeight >= 123 && p1instance.fruitHeight <= 133){ // if the middle of the fruit is within the cutting range
							p1instance.score = 2;
							if(p1instance.fruitHeight >= 125 && p1instance.fruitHeight <= 130){ // if the middle of the fruit is within the cutting spark
								p1instance.score = 3;
							}
						}
						p1instance.score *= 5;
						p1instance.scorePoints.push_back( new ScorePoint(p1instance.score,p1instance.x*38+26,120) );
						p1instance.totalPoints += p1instance.score;
						
						p1instance.cut = 1;
						break;
					}
				}
				else // we completely missed the fruit
					p1instance.cut = -1;
			}
			if(p1instance.cut==1){ // add cut fruit
				type = p1instance.fruits[i]->type;
				x = p1instance.fruits[i]->x;
				y = p1instance.fruits[i]->y;
				w = p1instance.fruits[i]->w;
				h = p1instance.fruits[i]->h;
				if(type==0){ // watermellon width changes when cut, other fruits do not
					p1instance.fruits.push_back( new Fruit(fruitTexs[type],type,1,x-w/2,y,16,h,-0.5,-1) );
					p1instance.fruits.push_back( new Fruit(fruitTexs[type],type,2,x+w/2,y,16,h,0.5,-1) );
				} else {
					p1instance.fruits.push_back( new Fruit(fruitTexs[type],type,1,x-w/2,y,w,h,-0.5,-1) );
					p1instance.fruits.push_back( new Fruit(fruitTexs[type],type,2,x+w/2,y,w,h,0.5,-1) );
				}
				p1instance.fruits[i]->y = 300; // move original fruit off-screen to be deleted
			}
			if(p1instance.cut==-1){ // take away points for a missed fruit
				p1instance.score = -2;
				p1instance.scorePoints.push_back( new ScorePoint(p1instance.score,p1instance.x*38+27,120) );
				p1instance.totalPoints += p1instance.score;
			}
		}
		if(p1instance.frame >= 4 + p1instance.cut==1?1:0){ // stop animation
			p1instance.swing = -1;
			p1instance.facing = (p1instance.facing+1)%2; // will alternate between o and 1
			p1instance.frame = 0;
			p1instance.cut = 0;
		}
		if(p1instance.frame==3 && p1instance.cut==1){ // move to the successful cut frame if we cut the fruit
			p1instance.frame = 4;
		}
	}
	
	// spawn fruit
	if(p1instance.fruitTimer <= 0){
		// set random values for the fruit
		type = rand()%4;
		x = rand()%8;
		// spawn fruit
		if(type==0)
			p1instance.fruits.push_back( new Fruit(fruitTexs[type],type,0,x*38,-16,20,16,0,0) ); // drop mellon
		else
			p1instance.fruits.push_back( new Fruit(fruitTexs[type],type,0,x*38,-8,8,8,0,0) ); // drop other fruit (they are all the same size)
		// set time delay before next fruit
		p1instance.fruitTimer = ((rand()%3)+1)*0.4;
	}
	p1instance.fruitTimer -= dt;
	
	// move fruit
	fallenFruit = -1;
	for(int i = 0; i<p1instance.fruits.size(); i++){
		p1instance.fruits[i]->move(dt);
		if(p1instance.fruits[i]->y >= 180){
			fallenFruit = i;
		}
	}
	// remove fallen fruit
	if(fallenFruit!=-1){
		if(p1instance.fruits[fallenFruit]->y<300 && p1instance.fruits[fallenFruit]->state==0){ // this fruit was uncut, deduct points
			p1instance.score = -2;
			p1instance.scorePoints.push_back( new ScorePoint(p1instance.score,p1instance.fruits[fallenFruit]->x + 27,170) );
			p1instance.totalPoints += p1instance.score;
		}
		p1instance.fruits.erase(p1instance.fruits.begin()+fallenFruit);
	}
	
	// move score points
	erasePoint = -1;
	for(int i = 0; i<p1instance.scorePoints.size(); i++){
		p1instance.scorePoints[i]->y--;
		p1instance.scorePoints[i]->opacity -= 4;
		if(p1instance.scorePoints[i]->opacity<=0)
			erasePoint = i;
	}
	// erase score points
	if(erasePoint!=-1)
		p1instance.scorePoints.erase(p1instance.scorePoints.begin()+erasePoint);
}

void FruitNinjaGameState::p1render(sf::RenderWindow& p1window){
	p1window.clear();
	// background
	p1window.draw(*back);
	
	// draw name and total points behind fruit
	p1instance.drawName(&p1window,screenRatio,font);
	p1instance.drawTotalScore(&p1window,screenRatio,font);
	
	// fruit
	for(int i = 0; i<p1instance.fruits.size(); i++){
		p1instance.fruits[i]->draw(&p1window, screenRatio, font);
	}
	
	// guy
	guy->setTextureRect(sf::IntRect( sf::Vector2(48*p1instance.frame,48*p1instance.facing), sf::Vector2(48,48) )); // change this will change image we draw from tile set
	guy->setPosition(sf::Vector2f( (3+(38*p1instance.x) )*screenRatio , height-(58*screenRatio) ));
	p1window.draw(*guy);
	
	// points
	for(int i = 0; i<p1instance.scorePoints.size(); i++)
		p1instance.scorePoints[i]->draw(&p1window,screenRatio,font);
	
	// draw name and total points in front of fruit
	p1instance.drawName(&p1window,screenRatio,font,true);
	p1instance.drawTotalScore(&p1window,screenRatio,font,true);
}

void FruitNinjaGameState::p2render(sf::RenderWindow& p2window){
	//p2window.clear();
	//// background
	//p2window.draw(*back);
	//// fruit
	//for(int i = 0; i<fruits.size(); i++){
	//	fruits[i]->draw(&p2window, screenRatio, font);
	//}
	//// guy
	//guy->setTextureRect(sf::IntRect( sf::Vector2(48*guyFrame,48*guyFacing), sf::Vector2(48,48) )); // change this will change image we draw from tile set
	//guy->setPosition(sf::Vector2f( (3+(38*guyX) )*screenRatio , height-(58*screenRatio) ));
	//p2window.draw(*guy);
	//// points
	//for(int i = 0; i<scorePoints.size(); i++)
	//	scorePoints[i]->draw(&p2window,screenRatio,font);
	//p2window.draw(*totalPointsText);
}