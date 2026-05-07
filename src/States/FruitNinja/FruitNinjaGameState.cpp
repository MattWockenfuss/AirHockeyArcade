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
#include "../../AudioManager.hpp"
#include "../Leaderboard/LeaderboardInterface.hpp"

Fruit::Fruit(int type, int state, int x, int y, int w, int h, double vx, double vy){
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
void Fruit::init(int type, int state, int x, int y, int w, int h, double vx, double vy){
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
void Fruit::draw(sf::RenderTexture* window, double screenRatio, sf::Text& text, sf::Color& color, sf::Sprite& sprite){
	sprite.setTextureRect(sf::IntRect( sf::Vector2(w*frame,h*state), sf::Vector2(w,h) )); // change this will change image we draw from tile set
	sprite.setPosition(sf::Vector2f( (27+x-w/2)*screenRatio , y*screenRatio ));
	window->draw(sprite);
	
	if(state!=0) // don't draw label if the fruit was cut
		return;
	
	// draw button label above fruit
	switch(type){ // set the label to the type of fruit
		case 0:
			text.setString("A");
			break;
		case 1:
			text.setString("B");
			break;
		case 2:
			text.setString("X");
			break;
		case 3:
			text.setString("Y");
			break;
	}
	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(sf::Vector2f( (27+x)*screenRatio , (y-text.getLocalBounds().getCenter().y/4)*screenRatio ));
	window->draw(text);
}

ScorePoint::ScorePoint(int score, int x, int y){
	this->score = score;
	this->x = x;
	this->y = y;
	this->opacity = 255;
}
void ScorePoint::init(int score, int x, int y){
	this->score = score;
	this->x = x;
	this->y = y;
	this->opacity = 255;
}
void ScorePoint::draw(sf::RenderTexture* window, double screenRatio, sf::Text& text, sf::Color& color){
	text.setString(std::to_string(score)+"0");
	text.setOrigin(text.getLocalBounds().getCenter());
	text.setPosition(sf::Vector2f(x*screenRatio,y*screenRatio));
	color.g = 255*(score<0?0:1);
	color.b = 255*(score<0?0:1);
	color.a = opacity;
	text.setFillColor(color);
	window->draw(text);
}

FruitNinjaInstance::FruitNinjaInstance(std::string name){
	this->name = name;
}
void FruitNinjaInstance::drawNames(sf::RenderTexture* window, double screenRatio, sf::Text& text, sf::Color& color, std::string oppName){
	// player name
	text.setString(name);
	text.setOrigin(sf::Vector2f(0,0));
	text.setPosition(sf::Vector2f(8*screenRatio,0));
	color.g = 255;
	color.b = 255;
	text.setFillColor(color);
	window->draw(text);
	
	// opponent name
	text.setString(oppName);
	text.setOrigin(sf::Vector2f(text.getLocalBounds().size.x, 0));
	text.setPosition(sf::Vector2f(312*screenRatio,0));
	window->draw(text);
}
void FruitNinjaInstance::drawTotalScores(sf::RenderTexture* window, double screenRatio, sf::Text& text, sf::Color& color, int oppScore){
	// player score
	text.setString(std::to_string(totalPoints)+"0");
	text.setOrigin(sf::Vector2f(0,0));
	text.setPosition(sf::Vector2f(8*screenRatio,12*screenRatio));
	color.g = 255*(totalPoints<0?0:1);
	color.b = 255*(totalPoints<0?0:1);
	text.setFillColor(color);
	window->draw(text);
	
	// opponent score
	text.setString(std::to_string(oppScore)+"0");
	text.setOrigin(sf::Vector2f(text.getLocalBounds().size.x,0));
	text.setPosition(sf::Vector2f(312*screenRatio,12*screenRatio));
	color.g = 255*(oppScore<0?0:1);
	color.b = 255*(oppScore<0?0:1);
	text.setFillColor(color);
	window->draw(text);
}

void FruitNinjaGameState::init(Context* ctx){
	State::init(ctx);
	
	// debugging
    std::cout << "\nFruitNinjaGameState Created!" << std::endl;
    //std::cout << "Consolas" << &ctx -> assets -> getFont("Consolas") << std::endl;
    //std::cout << "ST-SimpleSquare" << &ctx -> assets -> getFont("ST-SimpleSquare") << std::endl;
	
	width = ctx -> p1window -> getView().getSize().x;
    height = ctx -> p1window -> getView().getSize().y;
	screenRatio = width / 320.0;
    //screenRatio = 6;
    std::cout << "Screen Ratio: " << screenRatio << std::endl;
	
	// sprites
	background.emplace(ctx->assets->getAsset("background"));
	background->setScale(sf::Vector2f(screenRatio,screenRatio));
	guy.emplace(ctx->assets->getAsset("redGuy_tileSet"));
	guy->setScale(sf::Vector2f(screenRatio,screenRatio));
	fruitSprites[0].emplace(ctx -> assets -> getAsset("mellon_tileSet"));
	fruitSprites[1].emplace(ctx -> assets -> getAsset("apple_tileSet"));
	fruitSprites[2].emplace(ctx -> assets -> getAsset("orange_tileSet"));
	fruitSprites[3].emplace(ctx -> assets -> getAsset("lemon_tileSet"));
	for(int i = 0; i<4; i++){
		fruitSprites[i]->setScale(sf::Vector2f(screenRatio,screenRatio));
	}
	
	// create instances
	instances.push_back( *(new FruitNinjaInstance("PLR")) );
	instances.push_back( *(new FruitNinjaInstance("COM")) );
	if(ctx->p1name!="")
		instances[0].name = ctx->p1name;
	if(ctx->p2name!="")
		instances[1].name = ctx->p2name;
	
	// text
	text.emplace(ctx -> assets -> getFont("ST-SimpleSquare"), instances[0].name, 12);
	color.emplace(255,255,255,255);
	
	// create object pools
	for(int i = 0; i<50; i++){ // create scorePoints
		pointPool.push_back( new ScorePoint(0,0,0) );
	}
	for(int i = 0; i<4; i++){ // create fruit
		fruitPool.push_back( *(new std::vector<Fruit*>) );
		for(int j = 0; j<30; j++){ // create 30 instances of each kind of fruit
			fruitPool[i].push_back( new Fruit(i,0,0,(i==0?-16:-12),(i==0?20:8),(i==0?16:8),0,0) );
		}
	}
	
	// seed random number generator
	std::srand(std::time(0)); // seed the random numbers based on the current time at kickoff
	
	// set fruit timing lists for the songs
	// fruit drop delay from spawn to sweet spot is roughly 3.933 seconds
	
	/* The first note in song 1 is 1.2s after the start
	   The first fruit of the song should wait 2 seconds to fall to give players time to get ready
	   If it takes a fruit 3.933s to fall from spawn to the perfect cut height,
	   Then the first fruit needs to start falling 3.933 - 1.2 seconds before the start of the song
	   The first song should start 2.733 seconds after the first fruit spawns
	 */
	instances[0].fruitSongDelays.push_back(1.733); // only player 1 will play sounds, but I'm giving player 2 the info too just in case
	instances[1].fruitSongDelays.push_back(1.733);
	// song 1, player 1 // 45 notes
	instances[0].fruitSongTimes.push_back( *(new std::vector<float>{ /*time delay relative to start of song : */2.0, 4.0, 2.0, 0.8, 0.8, 0.4, 0.8, 0.4, 1.2, 0.4, 0.8, 0.4, 0.4, 2.4, 0.4, 0.4, 0.4, 0.8, 0.4, 0.8, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8, 0.8, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4}) );
	// song 1, player 2 // 45 notes
	instances[1].fruitSongTimes.push_back( *(new std::vector<float>{ /*time delay relative to start of song : */4.0, 2.4, 1.2, 0.8, 1.2, 0.4, 0.8, 0.4, 0.8, 0.4, 0.8, 0.4, 0.4, 0.8, 0.4, 0.4, 0.4, 2.8, 0.4, 0.8, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 1.2, 0.8, 0.4, 0.4, 0.8, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4}) );
	/* The first note in song 2 is 2s after the start
	   The first fruit of the song should wait 2 seconds to fall to give players time to get ready
	   If it takes a fruit 3.933s to fall from spawn to the perfect cut height,
	   Then the first fruit needs to start falling 3.933 - 2 seconds before the start of the song
	   The second song should start 1.933 seconds after the first fruit spawns
	 */
	instances[0].fruitSongDelays.push_back(0.933);
	instances[1].fruitSongDelays.push_back(0.933);
	// song 2, player 1 // 56 notes
	instances[0].fruitSongTimes.push_back( *(new std::vector<float>{ /*time delay relative to start of song : */6.4, 0.8, 0.8, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8, 0.4, 0.4, 0.8, 0.4, 0.8, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.4, 0.4, 0.4, 0.8, 0.4, 0.8, 0.4, 0.8, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.8, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.4}) );
	// song 2, player 2 // 56 notes
	instances[1].fruitSongTimes.push_back( *(new std::vector<float>{ /*time delay relative to start of song : */6.0, 0.8, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8, 0.8, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8, 0.4, 0.4, 0.8, 0.4, 0.8, 0.4, 0.8, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.8, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.4}) );
	/* The first note in song 3 is 0.8s after the start
	   The first fruit of the song should wait 2 seconds to fall to give players time to get ready
	   If it takes a fruit 3.933s to fall from spawn to the perfect cut height,
	   Then the first fruit needs to start falling 3.933 - 0.8 seconds before the start of the song
	   The third song should start 3.133 seconds after the first fruit spawns
	 */
	instances[0].fruitSongDelays.push_back(2.133);
	instances[1].fruitSongDelays.push_back(2.133);
	// song 3, player 1 // 54 notes
	instances[0].fruitSongTimes.push_back( *(new std::vector<float>{ /*time delay relative to start of song : */6.0, 0.4, 0.4, 1.2, 0.4, 0.4, 1.2, 0.4, 0.4, 0.4, 0.8, 0.4, 0.4, 0.4, 0.8, 1.6, 0.8, 0.4, 0.4, 0.4, 0.8, 0.8, 0.4, 0.4, 0.8, 0.4, 0.8, 0.8, 1.6, 0.4, 0.4, 0.8, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.4, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.4, 0.4, 0.4}) );
	// song 3, player 2 // 54 notes
	instances[1].fruitSongTimes.push_back( *(new std::vector<float>{ /*time delay relative to start of song : */6.0, 0.4, 0.4, 1.2, 0.4, 0.4, 1.2, 0.4, 0.4, 0.4, 0.8, 0.4, 0.4, 0.8, 0.8, 0.4, 0.4, 0.8, 1.2, 0.4, 0.8, 0.8, 0.4, 0.4, 0.8, 0.4, 0.4, 0.8, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 2.0, 0.4, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8, 0.4, 0.8, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.8, 0.4, 0.4, 0.4}) );
	
	// set the initial fruit vars
	instances[0].fruitDelay = instances[0].fruitSongTimes[0][0];
	instances[1].fruitDelay = instances[1].fruitSongTimes[0][0];
	
	std::cout << "END OF FRUIT NINJA INIT" << std::endl;
}

void FruitNinjaGameState::tick(){	
	time = clock.restart();
	dt = time.asSeconds();
	
	// player 1 controls
	if(ctx->input->P1_Left && instances[0].swing==-1){ // move left
		x = instances[0].x;
		y = -1;
		for(int i = 0; i<instances[0].fruits.size(); i++){
			if(instances[0].fruits[i]->y > y && instances[0].fruits[i]->y < 133 &&instances[0].fruits[i]->state==0){
				x = instances[0].fruits[i]->x/38;
				y = instances[0].fruits[i]->y;
			}
		}
		if(instances[0].x>0 && x<instances[0].x)
			instances[0].x = x;
	}
	if(ctx->input->P1_Right && instances[0].swing==-1){ // move right
		x = instances[0].x;
		y = -1;
		for(int i = 0; i<instances[0].fruits.size(); i++){
			if(instances[0].fruits[i]->y > y && instances[0].fruits[i]->y < 133 && instances[0].fruits[i]->state==0){
				x = instances[0].fruits[i]->x/38;
				y = instances[0].fruits[i]->y;
			}
		}
		if(instances[0].x<7 && x>instances[0].x)
			instances[0].x = x;
	}
	if (ctx->input->P1A && instances[0].swing == -1){ // swing sword 'A'
		instances[0].swing = 0;
		instances[0].timer = 0;
		instances[0].frame = 1;
	}
	if (ctx->input->P1B && instances[0].swing == -1){ // swing sword 'B'
		instances[0].swing = 1;
		instances[0].timer = 0;
		instances[0].frame = 1;
	}
	if (ctx->input->P1X && instances[0].swing == -1){ // swing sword 'X'
		instances[0].swing = 2;
		instances[0].timer = 0;
		instances[0].frame = 1;
	}
	if (ctx->input->P1Y && instances[0].swing == -1){ // swing sword 'Y'
		instances[0].swing = 3;
		instances[0].timer = 0;
		instances[0].frame = 1;
	}
	// player 2 controls
	if(ctx->input->P2_Left && instances[1].swing==-1){ // move left
		x = instances[1].x;
		y = -1;
		for(int i = 0; i<instances[1].fruits.size(); i++){
			if(instances[1].fruits[i]->y > y && instances[1].fruits[i]->y < 133 &&instances[1].fruits[i]->state==0){
				x = instances[1].fruits[i]->x/38;
				y = instances[1].fruits[i]->y;
			}
		}
		if(instances[1].x>0 && x<instances[1].x)
			instances[1].x = x;
	}
	if(ctx->input->P2_Right && instances[1].swing==-1){ // move right
		x = instances[1].x;
		y = -1;
		for(int i = 0; i<instances[1].fruits.size(); i++){
			if(instances[1].fruits[i]->y > y && instances[1].fruits[i]->y < 133 && instances[1].fruits[i]->state==0){
				x = instances[1].fruits[i]->x/38;
				y = instances[1].fruits[i]->y;
			}
		}
		if(instances[1].x<7 && x>instances[1].x)
			instances[1].x = x;
	}
	if (ctx->input->P2A && instances[1].swing == -1){ // swing sword 'A'
		instances[1].swing = 0;
		instances[1].timer = 0;
		instances[1].frame = 1;
	}
	if (ctx->input->P2B && instances[1].swing == -1){ // swing sword 'B'
		instances[1].swing = 1;
		instances[1].timer = 0;
		instances[1].frame = 1;
	}
	if (ctx->input->P2X && instances[1].swing == -1){ // swing sword 'X'
		instances[1].swing = 2;
		instances[1].timer = 0;
		instances[1].frame = 1;
	}
	if (ctx->input->P2Y && instances[1].swing == -1){ // swing sword 'Y'
		instances[1].swing = 3;
		instances[1].timer = 0;
		instances[1].frame = 1;
	}
	// emergency game exit
	if(ctx->input->P1B && ctx->input->P1Y && ctx->input->P2B && ctx->input->P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
		// delete all object pools first
		// scorePoints
		while(pointPool.size()>0){
			delete pointPool[0];
			pointPool.erase(pointPool.begin());
		}
		while(instances[0].scorePoints.size()>0){
			delete instances[0].scorePoints[0];
			instances[0].scorePoints.erase(instances[0].scorePoints.begin());
		}
		while(instances[1].scorePoints.size()>0){
			delete instances[1].scorePoints[0];
			instances[1].scorePoints.erase(instances[1].scorePoints.begin());
		}
		// fruit
		for(int j = 0; j<4; j++){
			while(fruitPool[j].size()>0){
				delete fruitPool[j][0];
				fruitPool[j].erase(fruitPool[j].begin());
			}
		}
		while(instances[0].fruits.size()>0){
			delete instances[0].fruits[0];
			instances[0].fruits.erase(instances[0].fruits.begin());
		}
		while(instances[1].fruits.size()>0){
			delete instances[1].fruits[0];
			instances[1].fruits.erase(instances[1].fruits.begin());
		}
		
		ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
	}
	
	// logic for each instance
	for(int i = 0; i<instances.size(); i++){
		// animate guy
		if(instances[i].swing!=-1){
			instances[i].timer += dt;
			n = round(instances[i].timer/0.075f); // change frames every 0.075s (also account for lag that may cause a frame to be longer than 0.075s
			instances[i].timer = std::remainder(instances[i].timer,0.075f); // reset timer while retaining any overflow
			instances[i].frame += n;
			if(instances[i].frame==2 && instances[i].cut==0){ // check if we cut fruit
				int j; // we need to reference j after the loop, so we initialize it before
				for(j = 0; j<instances[i].fruits.size(); j++){
					if(instances[i].fruits[j]->x/38==instances[i].x && instances[i].fruits[j]->state==0 && instances[i].fruits[j]->type==instances[i].swing){ // if there is a whole fruit in the same column as the player, and they swung at the correct fruit
						fruitHeight = instances[i].fruits[j]->y;
						if(fruitHeight <= 133 && fruitHeight + instances[i].fruits[j]->h >= 123){ // if the fruit is within the maximum cutting range
							score = 1;
							fruitHeight += instances[i].fruits[j]->h/2;
							if(fruitHeight >= 123 && fruitHeight <= 133){ // if the middle of the fruit is within the cutting range
								score = 2;
								if(fruitHeight >= 125 && fruitHeight <= 130){ // if the middle of the fruit is within the cutting spark
									score = 3;
								}
							}
							score *= 5;
							instances[i].totalPoints += score;
							// spawn a point from the pool
							if(pointPool.size()>0){
								pointPool[0]->init(score,instances[i].x*38 + 27, 120);
								instances[i].scorePoints.push_back(pointPool[0]);
								pointPool.erase(pointPool.begin());
							}
							else{ // spawn a new score point entirely
								instances[i].scorePoints.push_back( new ScorePoint(score,instances[i].x*38+27,120) );
							}
							instances[i].cut = 1;
							break;
						}
					}
					else // we completely missed the fruit
						instances[i].cut = -1;
				}
				if(instances[i].cut==1){ // add cut fruit
					type = instances[i].fruits[j]->type;
					x = instances[i].fruits[j]->x;
					y = instances[i].fruits[j]->y;
					w = instances[i].fruits[j]->w;
					h = instances[i].fruits[j]->h;
					// spawn fruit
					if(type==0){ // drop mellon
						// left half (reuse existing fruit)
						instances[i].fruits[j]->init(type,1,x-w/2,y,16,h,-0.5,-1);
						// right half
						if(fruitPool[0].size()>0){
							fruitPool[0][0]->init(type,2,x-w/2,y,16,h,0.5,-1);
							instances[i].fruits.push_back(fruitPool[0][0]);
							fruitPool[0].erase(fruitPool[0].begin());
						}
						else{
							instances[i].fruits.push_back( new Fruit(type,2,x+w/2,y,16,h,0.5,-1) );
						}
					}
					else{ // drop other fruit (they are all the same size)
						// left half (reuse existing fruit)
						instances[i].fruits[j]->init(type,1,x-w/2,y,w,h,-0.5,-1);
						// right half
						if(fruitPool[type].size()>0){
							fruitPool[type][0]->init(type,2,x-w/2,y,w,h,0.5,-1);
							instances[i].fruits.push_back(fruitPool[type][0]);
							fruitPool[type].erase(fruitPool[type].begin());
						}
						else{
							instances[i].fruits.push_back( new Fruit(type,2,x+w/2,y,w,h,0.5,-1) );
						}
					}
				}
				if(instances[i].cut==-1){ // take away points for a missed fruit
					score = -2;
					instances[i].totalPoints += score;
					// spawn a point from the pool
					if(pointPool.size()>0){
						pointPool[0]->init(score,instances[i].x*38 + 27, 170);
						instances[i].scorePoints.push_back(pointPool[0]);
						pointPool.erase(pointPool.begin());
					}
					else{ // spawn a new score point entirely
						instances[i].scorePoints.push_back( new ScorePoint(score,instances[i].x*38+27,170) );
					}
				}
			}
			if(instances[i].frame >= 4 + instances[i].cut==1?1:0){ // stop animation
				instances[i].swing = -1;
				instances[i].facing = (instances[i].facing+1)%2; // will alternate between o and 1
				instances[i].frame = 0;
				instances[i].cut = 0;
			}
			if(instances[i].frame==3 && instances[i].cut==1){ // move to the successful cut frame if we cut the fruit
				instances[i].frame = 4;
			}
		}
		
		// start songs
		instances[i].songDelay -= dt;
		if(i==0 && instances[i].songDelay <= 0 && instances[i].songDelay+dt>0){
			switch(instances[i].fruitSong){
				case 0:
					ctx->audio->playSound(ctx->assets->getSound("song1"));
					break;
				case 1:
					ctx->audio->playSound(ctx->assets->getSound("song2"));
					break;
				case 2:
					ctx->audio->playSound(ctx->assets->getSound("song3"));
					break;
			}
		}
		// spawn fruit
		instances[i].fruitDelay -= dt;
		if(instances[i].fruitDelay <= 10 && instances[i].fruitSong==-1 && triggerGameEnd){ // the game was triggered to close
			//add player scores to the leaderboard
			triggerGameEnd = false;
			ctx -> leaderboard -> addScore(instances[0].name, instances[1].name, instances[0].totalPoints*10, instances[1].totalPoints*10, 2); // points are kept small and artificially increased for performance
			// free object pools
			// only one instance will call this, so I need to handle both instances and the global pools here
			// scorePoints
			while(pointPool.size()>0){
				delete pointPool[0];
				pointPool.erase(pointPool.begin());
			}
			while(instances[0].scorePoints.size()>0){
				delete instances[0].scorePoints[0];
				instances[0].scorePoints.erase(instances[0].scorePoints.begin());
			}
			while(instances[1].scorePoints.size()>0){
				delete instances[1].scorePoints[0];
				instances[1].scorePoints.erase(instances[1].scorePoints.begin());
			}
			// fruit
			for(int j = 0; j<4; j++){
				while(fruitPool[j].size()>0){
					delete fruitPool[j][0];
					fruitPool[j].erase(fruitPool[j].begin());
				}
			}
			while(instances[0].fruits.size()>0){
				delete instances[0].fruits[0];
				instances[0].fruits.erase(instances[0].fruits.begin());
			}
			while(instances[1].fruits.size()>0){
				delete instances[1].fruits[0];
				instances[1].fruits.erase(instances[1].fruits.begin());
			}
			
			// quit game
			ctx -> gsm -> requestStateChange(States::GameSelect, 5.0f, 1.5f);
		}
		if(instances[i].fruitDelay <= 0){
			// set random values for the fruit
			type = rand()%4;
			x = rand()%8;
			// spawn fruit
			if(type==0){ // drop mellon
				if(fruitPool[0].size()>0){
					fruitPool[0][0]->init(type,0,x*38,-16,20,16,0,0);
					instances[i].fruits.push_back(fruitPool[0][0]);
					fruitPool[0].erase(fruitPool[0].begin());
				}
				else{
					instances[i].fruits.push_back( new Fruit(type,0,x*38,-16,20,16,0,0) );
				}
			}
			else{ // drop other fruit (they are all the same size)
				if(fruitPool[type].size()>0){
					fruitPool[type][0]->init(type,0,x*38,-12,8,8,0,0);
					instances[i].fruits.push_back(fruitPool[type][0]);
					fruitPool[type].erase(fruitPool[type].begin());
				}
				else{
					instances[i].fruits.push_back( new Fruit(type,0,x*38,-12,8,8,0,0) );
				}
			}
			
			// set time delay before next song
			if(instances[i].fruitNote==1){
				instances[i].songDelay = instances[i].fruitSongDelays[instances[i].fruitSong];
			}
			// set time delay before next fruit
			instances[i].fruitDelay = instances[i].fruitSongTimes[instances[i].fruitSong][instances[i].fruitNote];
			instances[i].fruitNote++;
			if(instances[i].fruitNote >= instances[i].fruitSongTimes[instances[i].fruitSong].size()){
				instances[i].fruitNote = 0;
				instances[i].fruitSong++;
				if(instances[i].fruitSong >= instances[i].fruitSongTimes.size() ){ // we finished the last song, set game to close and return to the game select menu
					instances[i].fruitDelay = 16;
					instances[i].fruitSong = -1;
				}
			}
		}
		
		// move fruit
		fallenFruit = -1;
		for(int j = 0; j<instances[i].fruits.size(); j++){
			instances[i].fruits[j]->move(dt);
			if(instances[i].fruits[j]->y >= 180){
				fallenFruit = j;
			}
		}
		// remove fallen fruit
		if(fallenFruit!=-1){
			if(instances[i].fruits[fallenFruit]->y<300 && instances[i].fruits[fallenFruit]->state==0){ // this fruit was uncut, deduct points
				score = -2;
				instances[i].totalPoints += score;
				// bring a score point from the pool
				if(pointPool.size()>0){
					pointPool[0]->init(score,instances[i].fruits[fallenFruit]->x + 27, 170);
					instances[i].scorePoints.push_back(pointPool[0]);
					pointPool.erase(pointPool.begin());
				}
				else{ // spawn a new score point entirely
					instances[i].scorePoints.push_back( new ScorePoint(score,instances[i].fruits[fallenFruit]->x + 27,170) );
				}
			}
			fruitPool[instances[i].fruits[fallenFruit]->type].push_back(instances[i].fruits[fallenFruit]);
			instances[i].fruits.erase(instances[i].fruits.begin()+fallenFruit);
		}
		
		// move score points
		erasePoint = -1;
		for(int j = 0; j<instances[i].scorePoints.size(); j++){
			instances[i].scorePoints[j]->y--;
			instances[i].scorePoints[j]->opacity -= 4;
			if(instances[i].scorePoints[j]->opacity<=0)
				erasePoint = j;
		}
		// erase score points
		if(erasePoint!=-1){
			pointPool.push_back(instances[i].scorePoints[erasePoint]);
			instances[i].scorePoints.erase(instances[i].scorePoints.begin()+erasePoint);
		}
		//break;
	}
}

void FruitNinjaGameState::p1render(sf::RenderTexture& p1window){
	/* some color and text attributes may be set before any drawing functions are called
	 * these are constant values that will not change between the items being drawn
	 * they are set before the drawing loop to reduce the number of operations
	 *
	 * color.r and occasionally color.a are commented out because their values don't change between rendering different parts of the screen
	 */
	
	p1window.clear();
	// background
	p1window.draw(*background);
	
	// draw name and total points behind fruit
	text->setCharacterSize(12*screenRatio);
	//color->r = 255;
	color->a = 255;
	instances[0].drawNames(&p1window,screenRatio,*text,*color,instances[1].name);
	instances[0].drawTotalScores(&p1window,screenRatio,*text,*color,instances[1].totalPoints);
	
	// fruit
	//color->r = 255;
	color->g = 255;
	color->b = 255;
	//color->a = 255;
	text->setCharacterSize(6*screenRatio);
	text->setFillColor(*color);
	for(int i = 0; i<instances[0].fruits.size(); i++){
		instances[0].fruits[i]->draw(&p1window, screenRatio, *text, *color, *(fruitSprites[instances[0].fruits[i]->type]) );
	}
	
	// guy
	guy->setTextureRect(sf::IntRect( sf::Vector2(48*instances[0].frame,48*instances[0].facing), sf::Vector2(48,48) )); // change this will change image we draw from tile set
	guy->setPosition(sf::Vector2f( (3+(38*instances[0].x) )*screenRatio , height-(58*screenRatio) ));
	p1window.draw(*guy);
	
	// points
	//text->setCharacterSize(6*screenRatio);
	//color->r = 255;
	for(int i = 0; i<instances[0].scorePoints.size(); i++)
		instances[0].scorePoints[i]->draw(&p1window,screenRatio,*text,*color);
	
	// draw name and total points in front of fruit
	text->setCharacterSize(12*screenRatio);
	//color->r = 255;
	color->a = 128;
	instances[0].drawNames(&p1window,screenRatio,*text,*color,instances[1].name);
	instances[0].drawTotalScores(&p1window,screenRatio,*text,*color,instances[1].totalPoints);
}

void FruitNinjaGameState::p2render(sf::RenderTexture& p2window){
	/* some color and text attributes may be set before any drawing functions are called
	 * these are constant values that will not change between the items being drawn
	 * they are set before the drawing loop to reduce the number of operations
	 *
	 * color.r and occasionally color.a are commented out because their values don't change between rendering different parts of the screen
	 */
	
	p2window.clear();
	// background
	p2window.draw(*background);
	
	// draw name and total points behind fruit
	text->setCharacterSize(12*screenRatio);
	//color->r = 255;
	color->a = 255;
	instances[1].drawNames(&p2window,screenRatio,*text,*color,instances[0].name);
	instances[1].drawTotalScores(&p2window,screenRatio,*text,*color,instances[0].totalPoints);
	
	// fruit
	//color->r = 255;
	color->g = 255;
	color->b = 255;
	//color->a = 255;
	text->setCharacterSize(6*screenRatio);
	text->setFillColor(*color);
	for(int i = 0; i<instances[1].fruits.size(); i++){
		instances[1].fruits[i]->draw(&p2window, screenRatio, *text, *color, *(fruitSprites[instances[1].fruits[i]->type]) );
	}
	
	// guy
	guy->setTextureRect(sf::IntRect( sf::Vector2(48*instances[1].frame,48*instances[1].facing), sf::Vector2(48,48) )); // change this will change image we draw from tile set
	guy->setPosition(sf::Vector2f( (3+(38*instances[1].x) )*screenRatio , height-(58*screenRatio) ));
	p2window.draw(*guy);
	
	// points
	//text->setCharacterSize(6*screenRatio);
	//color->r = 255;
	for(int i = 0; i<instances[1].scorePoints.size(); i++)
		instances[1].scorePoints[i]->draw(&p2window,screenRatio,*text,*color);
	
	// draw name and total points in front of fruit
	text->setCharacterSize(12*screenRatio);
	//color->r = 255;
	color->a = 128;
	instances[1].drawNames(&p2window,screenRatio,*text,*color,instances[0].name);
	instances[1].drawTotalScores(&p2window,screenRatio,*text,*color,instances[0].totalPoints);
}