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
	this->speed = 1;
	this->visOffset = 0;
	this->virOffset = 0;
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
	bRect.setPosition(sf::Vector2f( (2*x+16)*screenRatio + (dir==3?-4*screenRatio:0) + (dir%2==1?visOffset:0) , (2*y+20)*screenRatio + (dir==0?-4*screenRatio:0) + (dir%2==0?visOffset:0) ));
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
	bRect.setPosition(sf::Vector2f( (2*x+16)*screenRatio + (dir==3?-4*screenRatio:0) + (dir%2==1?visOffset:0) , (2*y+20)*screenRatio + (dir==0?-4*screenRatio:0) + (dir%2==0?visOffset:0) ));
	window2->draw(bRect);
}

void bTronGameState::moveObjects(Bike* player1, Bike* player2, std::vector<std::vector<int>> grid, int gridSX, int gridSY, float dt){
	// player 1 vars
	int p1score = player1->score;
	int p1x = player1->x;
	int p1y = player1->y;
	float p1visOffset = player1->visOffset;
	float p1virOffset = player1->virOffset;
	int p1dir = player1->dir;
	float p1speed = player1->speed;
	// player 2 vars
	int p2score = player2->score;
	int p2x = player2->x;
	int p2y = player2->y;
	float p2visOffset = player2->visOffset;
	float p2virOffset = player2->virOffset;
	int p2dir = player2->dir;
	float p2speed = player2->speed;
	// general vars
	float minSpeed = gridSX/6; // div 6 meaning 6 seconds to cross the field going base speed
	float maxSpeed = minSpeed*3;
	float accConst = minSpeed; // this should amount to doubling your base speed after 1 second of acceleration
	bool moved = false;
	bool turned = false;
	bool drafting = false;
	
	// code
	// check if drafting
	if(p1dir%2==0){ // dir is 0 or 2, up or down
		if(p1x>0 && p1x<gridSX-1 && p1y>0 && p1y<gridSY-1){ // player is within bounds to check for immediate walls (WORKS)
			if( grid[p1x-1][p1y-1]!=0 && grid[p1x-1][p1y]!=0 && grid[p1x-1][p1y+1]!=0 ){ // wall immediately to the left
				drafting = true;
			}
			if( grid[p1x+1][p1y-1]!=0 && grid[p1x+1][p1y]!=0 && grid[p1x+1][p1y+1]!=0 ){ // wall immediately to the right
				drafting = true;
			}
		}
		if(p1x>1 && p1x<gridSX-2 && p1y>1 && p1y<gridSY-2){ // player is within bounds to check for 1-gap walls
			if( grid[p1x-2][p1y-1]!=0 && grid[p1x-2][p1y]!=0 && grid[p1x-2][p1y+1]!=0 ){ // wall 1-gap to the left
				drafting = true;
			}
			if( grid[p1x+2][p1y-1]!=0 && grid[p1x+2][p1y]!=0 && grid[p1x+2][p1y+1]!=0 ){ // wall 1-gap to the right
				drafting = true;
			}
		}
		if(p1x>2 && p1x<gridSX-3 && p1y>2 && p1y<gridSY-3){ // player is within bounds to check for 2-gap walls
			if( grid[p1x-3][p1y-1]!=0 && grid[p1x-3][p1y]!=0 && grid[p1x-2][p1y+1]!=0 ){ // wall 2-gap to the left
				drafting = true;
			}
			if( grid[p1x+3][p1y-1]!=0 && grid[p1x+3][p1y]!=0 && grid[p1x+2][p1y+1]!=0 ){ // wall 2-gap to the right
				drafting = true;
			}
		}
	}
	else{ // dir is 1 or 3, left or right
		if(p1x>0 && p1x<gridSX-1 && p1y>0 && p1y<gridSY-1){ // player is within bounds to check for immediate walls (WORKS)
			if( grid[p1x-1][p1y-1]!=0 && grid[p1x][p1y-1]!=0 && grid[p1x+1][p1y-1]!=0 ){ // wall immediately to the top
				drafting = true;
			}
			if( grid[p1x-1][p1y+1]!=0 && grid[p1x][p1y+1]!=0 && grid[p1x+1][p1y+1]!=0 ){ // wall immediately to the bottom
				drafting = true;
			}
		}
		if(p1x>1 && p1x<gridSX-2 && p1y>1 && p1y<gridSY-2){ // player is within bounds to check for 1-gap walls
			if( grid[p1x-1][p1y-2]!=0 && grid[p1x][p1y-2]!=0 && grid[p1x+1][p1y-2]!=0 ){ // wall 1-gap to the top
				drafting = true;
			}
			if( grid[p1x-1][p1y+2]!=0 && grid[p1x][p1y+2]!=0 && grid[p1x+1][p1y+2]!=0 ){ // wall 1-gap to the bottom
				drafting = true;
			}
		}
		if(p1x>2 && p1x<gridSX-3 && p1y>2 && p1y<gridSY-3){ // player is within bounds to check for 2-gap walls
			if( grid[p1x-1][p1y-3]!=0 && grid[p1x][p1y-3]!=0 && grid[p1x+1][p1y-3]!=0 ){ // wall 2-gap to the top
				drafting = true;
			}
			if( grid[p1x-1][p1y+3]!=0 && grid[p1x][p1y+3]!=0 && grid[p1x+1][p1y+3]!=0 ){ // wall 2-gap to the bottom
				drafting = true;
			}
		}
	}
	
	// accelerate
	if(drafting)
		p1speed += accConst*dt;
	else
		p1speed -= (accConst/2)*dt; // slow down half as fast to let the speed last longer
	// keep speed between min and max
	if(p1speed<minSpeed)
		p1speed = minSpeed;
	if(p1speed>maxSpeed)
		p1speed = maxSpeed;
	
	// move bike
	switch(p1dir){
		case 0:{ // up
			p1visOffset -= p1speed*dt;
			while(p1visOffset<=-1){ // we initially move within the visual offset, we transfer it to the actual position in discrete increments
				p1y--;
				p1visOffset += 1;
				moved = true;
			}
			break;
		}
		case 1:{ // right
			p1visOffset += p1speed*dt;
			while(p1visOffset>=1){ // we initially move within the visual offset, we transfer it to the actual position in discrete increments
				p1x++;
				p1visOffset -= 1;
				moved = true;
			}
			break;
		}
		case 2:{ // down
			p1visOffset += p1speed*dt;
			while(p1visOffset>=1){ // we initially move within the visual offset, we transfer it to the actual position in discrete increments
				p1y++;
				p1visOffset -= 1;
				moved = true;
			}
			break;
		}
		case 3:{ // left
			p1visOffset -= p1speed*dt;
			while(p1visOffset<=-1){ // we initially move within the visual offset, we transfer it to the actual position in discrete increments
				p1x--;
				p1visOffset += 1;
				moved = true;
			}
			break;
		}
	}
	
	// place trail
	
	// turning
	if(moved){
		while(!turned && player1->queue.size()>0){ // while we haven't turned yet and there are still turn inputs in the queue
			if(player1->queue[0] != p1dir && player1->queue[0] != (p1dir+2)%4){ // if the direction is not the way we are facing / opposite, then we can use it
				p1dir = player1->queue[0];
				p1visOffset = 0;
				turned = true;
			}
			player1->queue.erase(player1->queue.begin()); // remove the turn input from queue regardless
		}
	}
	
	// reinstate vars
	// player 1
	player1->score = p1score;
	player1->x = p1x;
	player1->y = p1y;
	player1->visOffset = p1visOffset;
	player1->virOffset = p1virOffset;
	player1->dir = p1dir;
	player1->speed = p1speed;
	// player 2
	player2->score = p2score;
	player2->x = p2x;
	player2->y = p2y;
	player2->visOffset = p2visOffset;
	player2->virOffset = p2virOffset;
	player2->dir = p2dir;
	player2->speed = p2speed;
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
	if (ctx -> input -> P1_Up && !W){
        player1.queue.push_back(0);
		W = true;
    }
    if (!ctx -> input -> P1_Up && W){
        W = false;
    }
    if (ctx -> input -> P1_Down && !S){
        player1.queue.push_back(2);
		S = true;
    }
    if (!ctx -> input -> P1_Down && S){
        S = false;
    }
    if (ctx -> input -> P1_Left && !A){
        player1.queue.push_back(3);
		A = true;
    }
    if (!ctx -> input -> P1_Left && A){
        A = false;
    }
    if (ctx -> input -> P1_Right && !D){
        player1.queue.push_back(1);
		D = true;
    }
    if (!ctx -> input -> P1_Right && D){
        D = false;
    }
    // player 2
	if (ctx -> input -> P2_Up && !Up){
        player2.queue.push_back(0);
		Up = true;
    }
    if (!ctx -> input -> P2_Up && Up){
        Up = false;
    }
    if (ctx -> input -> P2_Down && !Down){
        player2.queue.push_back(2);
        Down = true;
    }
    if (!ctx -> input -> P2_Down && Down){
        Down = false;
    }
    if (ctx -> input -> P2_Left && !Left){
        player2.queue.push_back(3);
        Left = true;
    }
    if (!ctx -> input -> P2_Left && Left){
        Left = false;
    }
    if (ctx -> input -> P2_Right && !Right){
        player2.queue.push_back(1);
        Right = true;
    }
    if (!ctx -> input -> P2_Right && Right){
        Right = false;
    }
    
	// emergency game exit
	if(ctx->input->P1B && ctx->input->P1Y && ctx->input->P2B && ctx->input->P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
		ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
	}
	
	// kickoff
    if(kickoff!=0){
		
	}
	
	moveObjects(&player1, &player2, gameGrid, gridSX, gridSY, dt);
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