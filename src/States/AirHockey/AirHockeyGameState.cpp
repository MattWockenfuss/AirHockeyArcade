#include "AirHockeyGameState.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

#include "../../Context.hpp"
#include "../GameStateManager.hpp"
#include "../../AssetManager.hpp"
#include "../../KeyManager.hpp"
#include "../../IO/InputManager.hpp"
#include "../../AudioManager.hpp"


double getScreenX(double x, double y, int screenWidth){
    double screenFieldWidth = 90;
    double y_ = y/100;
	double screenRatio = (double)screenWidth / 320.0; // these funcions were made for a 320x180 screen, not 1280x720 or larger
    if(y<300)
        screenFieldWidth = screenRatio*( (y_*y_) + (7*y_) + 90 );
    else if(y<500)
        screenFieldWidth = screenRatio*( (2*(y_-3)*(y_-3)) + (13*(y_-3)) + 120 );
    else
        screenFieldWidth = screenRatio*( (7*(y_-5)*(y_-5)) + (18*(y_-5)) + 154 );
    
    double widthFrac = x/600;
    
    // half the screen width - half the fieldWidth should get us to the far left side of the screen field
    double xLoc = (screenWidth/2) - (screenFieldWidth/2) + (widthFrac*screenFieldWidth);
    return xLoc;
}
double getScreenY(double y, int screenHeight){
    double yLoc = 11;
    double y_ = y/100;
	double screenRatio = (double)screenHeight / 180.0; // these funcions were made for a 320x180 screen, not 1280x720 or larger
    if(y<300)
        yLoc = screenRatio*( (y_*y_) + (7*y_) + 11 );
    else if(y<500)
        yLoc = screenRatio*( (2*(y_-3)*(y_-3)) + (13*(y_-3)) + 41 );
    else
        yLoc = screenRatio*( (7*(y_-5)*(y_-5)) + (18*(y_-5)) + 75 );
    
    return yLoc;
}

Puck::Puck(double x, double y, double vx, double vy, double diam) {
    this -> x = x;
    this -> y = y;
    this -> vx = vx;
    this -> vy = vy;
    this -> diam = diam;
    
    bool loadImage;
    loadImage = pucks[0].loadFromFile("assets/images/puck_0.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[1].loadFromFile("assets/images/puck_1a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[2].loadFromFile("assets/images/puck_1b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[3].loadFromFile("assets/images/puck_1c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[4].loadFromFile("assets/images/puck_2a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[5].loadFromFile("assets/images/puck_2b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[6].loadFromFile("assets/images/puck_2c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[7].loadFromFile("assets/images/puck_3a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[8].loadFromFile("assets/images/puck_3b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[9].loadFromFile("assets/images/puck_3c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[10].loadFromFile("assets/images/puck_4a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[11].loadFromFile("assets/images/puck_4b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[12].loadFromFile("assets/images/puck_4c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[13].loadFromFile("assets/images/puck_5a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[14].loadFromFile("assets/images/puck_5b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[15].loadFromFile("assets/images/puck_5c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[16].loadFromFile("assets/images/puck_6a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[17].loadFromFile("assets/images/puck_6b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[18].loadFromFile("assets/images/puck_6c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[19].loadFromFile("assets/images/puck_7a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[20].loadFromFile("assets/images/puck_7b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[21].loadFromFile("assets/images/puck_7c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[22].loadFromFile("assets/images/puck_8a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[23].loadFromFile("assets/images/puck_8b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[24].loadFromFile("assets/images/puck_8c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[25].loadFromFile("assets/images/puck_9a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[26].loadFromFile("assets/images/puck_9b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = pucks[27].loadFromFile("assets/images/puck_9c.png");
    if(!loadImage) {std::perror("File not found!"); }
}
int Puck::getIndex(double x, double y, double diam) {
    if(y < (diam / 2)) return 0; // goal, puck 0 
    if(y < diam){
        if(x<200) // puck 1a
            return 1;
        if(x>400) // puck 1c
            return 3;
        return 2; // puck 1b
    }
    if(y < diam * 3){
        if(x<200) // puck 2a
            return 4;
        if(x>400) // puck 2c
            return 6;
        return 5; // puck 2b
    }
    if(y < (diam * 5)){
        if(x<200) // puck 3a
            return 7;
        if(x>400) // puck 3c
            return 9;
        return 8; // puck 3b
    }
    if(y < (diam * 7)){
        if(x<200) // puck 4a
            return 10;
        if(x>400) // puck 4c
            return 12;
        return 11; // puck 4b
    }
    if(y < (diam * 9)){
        if(x<200) // puck 5a
            return 13;
        if(x>400) // puck 5c
            return 15;
        return 14; // puck 5b
    }
    if(y < (diam * 11)){
        if(x<200) // puck 6a
            return 16;
        if(x>400) // puck 6c
            return 18;
        return 17; // puck 6b
    }
    if(y < (diam * 13)){
        if(x<200) // puck 7a
            return 19;
        if(x>400) // puck 7c
            return 21;
        return 20; // puck 7b
    }
    if(y < (diam * 15)){
        if(x<200) // puck 8a
            return 22;
        if(x>400) // puck 8c
            return 24;
        return 23; // puck 8b
    }
    if(x < 200) // puck 9a
        return 25;
    if(x > 400) // puck 9c
        return 27;
    return 26; // puck 9b
}
void Puck::draw1(sf::RenderWindow* window1) {
    int index;
    double xOffset;
    double yOffset;
    double screenX;
    double screenY;
    double screenRatio = (double)(window1 -> getSize().x) / 320.0;
    
    index = getIndex(x, y, diam);
    xOffset = (pucks[index].getSize().x / 2) * screenRatio;
    yOffset = (pucks[index].getSize().y / 2) * screenRatio;
    // as pucks get closer, you see their thickness, their offset needs to be adjusted so they are still drawn at the correct location
    if(index > 12)
        yOffset += screenRatio;
    if(index > 21)
        yOffset += screenRatio;
    if(index > 24)
        yOffset += screenRatio;
    
    screenX = getScreenX(x, y, window1 -> getSize().x);
    screenY = getScreenY(y, window1 -> getSize().y);
	
    sf::Sprite sprite(pucks[index]);
    sprite.setPosition({(float)(screenX - xOffset), (float)(screenY - yOffset)});
    sprite.setScale({(float) screenRatio, (float) screenRatio});
    window1 -> draw(sprite);
}
void Puck::draw2(sf::RenderWindow* window2) {
    int index;
	double x_, y_;
    double xOffset;
    double yOffset;
    double screenX;
    double screenY;
    double screenRatio = (double)(window2 -> getSize().x) / 320.0;
    
	x_ = 600-x;
	y_ = 800-y;
    index = getIndex(x_, y_, diam);
    xOffset = (pucks[index].getSize().x / 2) * screenRatio;
    yOffset = (pucks[index].getSize().y / 2) * screenRatio;
    // as pucks get closer, you see their thickness, their offset needs to be adjusted so they are still drawn at the correct location
    if(index > 12)
        yOffset += screenRatio;
    if(index > 21)
        yOffset += screenRatio;
    if(index > 24)
        yOffset += screenRatio;
    
    screenX = getScreenX(x_, y_, window2 -> getSize().x);
    screenY = getScreenY(y_, window2 -> getSize().y);
	
    sf::Sprite sprite(pucks[index]);
    sprite.setPosition({(float)(screenX - xOffset), (float)(screenY - yOffset)});
    sprite.setScale({(float) screenRatio, (float) screenRatio});
    window2 -> draw(sprite);
}
void Puck::setKickoff(int type){
	double pi = 3.1415926;
	double direction = -1;
	if(type==1)
		direction = 1;
	if(type==0){ // this is the kickoff for the very start of the game
		std::srand(std::time(0)); // seed the random numbers based on the current time at kickoff
		direction = -1 + 2*(std::rand()%2); // make direction either -1 or 1;
	}
	double angle = (double)((std::rand()%121)+30)*pi/180.0;
	vx = 930*cos(angle)*direction;
	vy = 930*sin(angle)*direction;
}

Paddle::Paddle(int xPos, int yPos, double vx, double vy, double diam){
    this -> xPos = xPos;
    this -> yPos = yPos;
    this -> x = xPos * 100;
    this -> y = (yPos * diam) - diam/2;
    this -> vx = vx;
    this -> vy = vy;
    this -> diam = diam;


    bool loadImage;
    loadImage = paddles[0].loadFromFile("assets/images/paddle_1a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[1].loadFromFile("assets/images/paddle_1b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[2].loadFromFile("assets/images/paddle_1c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[3].loadFromFile("assets/images/paddle_2a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[4].loadFromFile("assets/images/paddle_2b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[5].loadFromFile("assets/images/paddle_2c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[6].loadFromFile("assets/images/paddle_3a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[7].loadFromFile("assets/images/paddle_3b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[8].loadFromFile("assets/images/paddle_3c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[9].loadFromFile("assets/images/paddle_4a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[10].loadFromFile("assets/images/paddle_4b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[11].loadFromFile("assets/images/paddle_4c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[12].loadFromFile("assets/images/paddle_5a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[13].loadFromFile("assets/images/paddle_5b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[14].loadFromFile("assets/images/paddle_5c.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[15].loadFromFile("assets/images/paddle_6a.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[16].loadFromFile("assets/images/paddle_6b.png");
    if(!loadImage) {std::perror("File not found!"); }
    loadImage = paddles[17].loadFromFile("assets/images/paddle_6c.png");
    if(!loadImage) {std::perror("File not found!"); }
}
int Paddle::getIndex(double x, double y, double diam){
    // used 198 and 402 instead of 200 and 400 to minimize image jittering on edges
    if(y < diam){
        if(x < 195) // paddle 1a
            return 0;
        if(x > 405) // paddle 1c
            return 2;
        return 1; // paddle 1b
    }
    if(y < diam * 2){
        if(x < 195) // paddle 2a
            return 3;
        if(x > 405) // paddle 2c
            return 5;
        return 4; // paddle 2b
    }
    if(y < diam*3){
        if(x < 195) // paddle 3a
            return 6;
        if(x > 405) // paddle 3c
            return 8;
        return 7; // paddle 3b
    }
    if(y < diam*10){
        if(x < 195) // paddle 4a
            return 9;
        if(x > 405) // paddle 4c
            return 11;
        return 10; // paddle 4b
    }
    if(y < diam*11){
        if(x < 195) // paddle 5a
            return 12;
        if(x > 405) // paddle 5c
            return 14;
        return 13; // paddle 5b
    }
    if(x < 195) // paddle 6a
        return 15;
    if(x > 405) // paddle 6c
        return 17;
    return 16; // paddle 6b
}
void Paddle::draw1(sf::RenderWindow* window1){
    int index;
    double xOffset;
    double yOffset;
    double screenX;
    double screenY;
    double screenRatio = (double)(window1 -> getSize().x) / 320.0;
    
	index = getIndex(x, y, diam);
    xOffset = (paddles[index].getSize().x/2)*screenRatio;
    yOffset = (paddles[index].getSize().y/2)*screenRatio;
    // as paddles get closer, you see their thickness, their offset needs to be adjusted so they are still drawn at the correct location
    if(index>12)
        yOffset += screenRatio;
    if(index>21)
        yOffset += screenRatio;
    if(index>24)
        yOffset += screenRatio;
    
    screenX = getScreenX(x, y, window1 -> getSize().x);
    screenY = getScreenY(y, window1 -> getSize().y);
    
    sf::Sprite sprite(paddles[index]);
    sprite.setPosition({(float)(screenX - xOffset), (float)(screenY - yOffset)});
    sprite.setScale({(float) screenRatio, (float) screenRatio});
    window1 -> draw(sprite);
}
void Paddle::draw2(sf::RenderWindow* window2){
	int index;
	double x_, y_;
    double xOffset;
    double yOffset;
    double screenX;
    double screenY;
    double screenRatio = (double)(window2 -> getSize().x) / 320.0;
    
	x_ = 600-x;
	y_ = 800-y;
	index = getIndex(x_, y_, diam);
    xOffset = (paddles[index].getSize().x/2)*screenRatio;
    yOffset = (paddles[index].getSize().y/2)*screenRatio;
    // as paddles get closer, you see their thickness, their offset needs to be adjusted so they are still drawn at the correct location
    if(index>12)
        yOffset += screenRatio;
    if(index>21)
        yOffset += screenRatio;
    if(index>24)
        yOffset += screenRatio;
    
    screenX = getScreenX(x_, y_, window2 -> getSize().x);
    screenY = getScreenY(y_, window2 -> getSize().y);
    
    sf::Sprite sprite(paddles[index]);
    sprite.setPosition({(float)(screenX - xOffset), (float)(screenY - yOffset)});
    sprite.setScale({(float) screenRatio, (float) screenRatio});
    window2 -> draw(sprite);
}

void puckFriction(double *vx, double *vy, float dt){
	// base puck velocity should be 930 (~1s to travel down the field (750) and ~1s to travel across the field (550) )
    double v;
    double angle;
    double fric;
	
    //BRIAR HAD THIS AT 4000
	double fricConst = 4000; // the larger this is, the lower the friction
    double speedConst = 930; // speed so that the puck travels across the diagonal of the field in 1s
	
    // change velocity
    if(*vx == 0){
		fric = ((dt * *vy) / fricConst) * (*vy > 0 ? -1:1) + 1; // scale friction based on dt and velocity
		if(fric < 0) // handle extreme friction values if game lags for a long time
			fric = 0.1; // don't set to zero or else puck will stop mid-game
		*vy *= fric;
		if(pow(*vy, 2) < pow(speedConst, 2) && *vy != 0)
			*vy = speedConst*(*vy >= 0 ? 1:-1); // make sure to account for velocity being pos or neg
    }else if(*vy == 0){
        fric = ((dt * *vx) / fricConst) * (*vx > 0 ?-1:1) + 1; // scale friction based on dt and velocity
		if(fric < 0) // handle extreme friction values if game lags for a long time
			fric = 0.1; // don't set to zero or else puck will stop mid-game
		*vx *= fric;
		if(pow(*vx, 2) < pow(speedConst, 2) && *vx != 0)
			*vx = speedConst * (*vx >= 0 ? 1:-1); // make sure to account for velocity being pos or neg
    }else{
		// make all values positive to simplify math
		bool xNeg = *vx < 0;
		bool yNeg = *vy < 0;

		if(xNeg) *vx *= -1;
		if(yNeg) *vy *= -1;

		// math
        v = hypot(*vx, *vy);
        angle = atan(*vy / *vx);
		fric = ((dt * v) / (-1 * fricConst)) + 1; // scale friction based on dt and velocity
		if(fric < 0) fric = 0.1;
		v *= fric;
		if(v < speedConst && v != 0) v = speedConst;
		// restore values
        *vx = v*cos(angle)*(xNeg?-1:1);
        *vy = v*sin(angle)*(yNeg?-1:1);
    }
	return;
}
void paddleFriction(Paddle* paddle, float dt){
	int xPos = paddle -> xPos;
    int yPos = paddle -> yPos;
    double x = paddle -> x;
    double y = paddle -> y;
    double vx = paddle -> vx;
    double vy = paddle -> vy;
    double diam = paddle -> diam;
	double v;
	double angle;
    double acc;
	double fric;
    double accConst = 1500; // scaled for average friction value of 0.99 // average dt seems to be around 0.0025
    double fricConst = 70; // the larger this is, the lower the friction
    double jitterConst = 100*dt; // the smaller this is, the more jittering // feels good at 0.01 for 2000fps, and 1 for 60fps // roughly equates to 60*dt
	
	// change velocity
	// xACC
	acc = ((xPos * 100) - x) * accConst * dt;
	vx += acc;
	// yACC
	acc = ((yPos * diam) - (diam / 2) - y) * accConst * dt;
	vy += acc;
	
	// friction
	if(vx == 0){
		fric = ((dt * vy) / fricConst) * (vy > 0 ? -1:1) + 1; // scale friction based on dt and velocity
		if(fric < 0) fric = 0;
		vy *= fric;
		// stabilize jittering
		if(pow(((yPos * diam) - (diam / 2) - y), 2) < jitterConst && pow(vy * dt, 2) < jitterConst){
			y = (yPos * diam) - (diam / 2);
			vy = 0;
		}
		// return
		if(y >= (yPos * diam) - (diam / 2) - 1 && yPos == 3) yPos = 2;
		if(y <= (yPos * diam) - (diam / 2) + 1 && yPos == 10) yPos = 11;
    }else if(vy == 0){
        fric = ((dt * vx) / fricConst) * (vx > 0 ? -1:1) + 1; // scale friction based on dt and velocity
		if(fric < 0) fric = 0;
		vx *= fric;
		// stabilize jittering
		if(pow(((xPos * 100) - x), 2) < jitterConst && pow(vx * dt, 2) < jitterConst){
			x = xPos * 100;
			vx = 0;
		}
    }else{
		// make all values positive to simplify math
		bool xNeg = vx < 0;
		bool yNeg = vy < 0;

		if(xNeg) vx *= -1;
		if(yNeg) vy *= -1;
		
		//caculate the velocity magnitude and the angle
        v = hypot(vx, vy);
        angle = atan(vy / vx);

		//scale friction based on dt and velocity
		fric = ((dt * v) / fricConst) * (v > 0 ? -1:1) + 1;
		if(fric < 0) fric = 0;
		v *= fric;

		// restore values
        vx = v * cos(angle) * (xNeg ? -1:1);
        vy = v * sin(angle) * (yNeg ? -1:1);

		// stabilize jittering
		if(pow(((xPos * 100) - x), 2) < jitterConst && pow(vx * dt, 2) < jitterConst){
			x = xPos * 100;
			vx = 0;
		}
		if(pow(((yPos * diam) - (diam / 2) - y), 2) < jitterConst && pow(vy * dt, 2) < jitterConst){
			y = (yPos * diam) - (diam / 2);
			vy = 0;
		}
		// return
		if(y >= (yPos * diam) - (diam / 2) - 1 && yPos == 3) yPos = 2;
		if(y <= (yPos * diam) - (diam / 2) + 1 && yPos == 10) yPos = 11;
    }

	// restore values to paddle
	paddle -> x = x;
	paddle -> y = y;
	paddle -> vx = vx;
	paddle -> vy = vy;
	paddle -> yPos = yPos;
	return;
}

Player::Player(std::string name){
	this->name = name;
	this->score = 0;
}
void Player::draw1(sf::RenderWindow* window1, sf::Font font){
	double screenRatio = (double)(window1 -> getSize().x) / 320.0;
	sf::Color blue(111,99,255);
	//sf::Color red(223,0,0);
	
	sf::Text text(font, name, 16*screenRatio);
	text.setFillColor(blue);
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(sf::Vector2f( rect.getCenter().x, 0)); // centered horizontally, top-bounded
	text.setPosition(sf::Vector2f(48.0*screenRatio , 16.0*screenRatio));
	window1 -> draw(text);
	
	text.setString(std::to_string(score));
	text.setOrigin(sf::Vector2f(0,0));
	text.setPosition(sf::Vector2f(48*screenRatio - rect.getCenter().x, 40*screenRatio));
	window1 -> draw(text);
}
void Player::draw2(sf::RenderWindow* window2, sf::Font font){
	double screenRatio = (double)(window2 -> getSize().x) / 320.0;
	//sf::Color blue(111,99,255);
	sf::Color red(223,0,0);
	
	sf::Text text(font, name, 16*screenRatio);
	text.setFillColor(red);
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(sf::Vector2f( rect.getCenter().x, 0)); // centered horizontally, top-bounded
	text.setPosition(sf::Vector2f(272.0*screenRatio , 16.0*screenRatio));
	window2 -> draw(text);
	
	text.setString(std::to_string(score));
	text.setOrigin(sf::Vector2f(0,0));
	text.setPosition(sf::Vector2f(272*screenRatio - rect.getCenter().x, 40*screenRatio));
	window2 -> draw(text);
}

void AirHockeyGameState::moveObjects(Puck* puck, Paddle* paddle1, Paddle* paddle2, float dt, int iter){
    // field is 600x800
	// puck
    double pk_x = puck -> x;
    double pk_y = puck -> y;
    double pk_x_ = pk_x;
    double pk_y_ = pk_y;
    double pk_vx = puck -> vx;
    double pk_vy = puck -> vy;
    double pk_diam = puck -> diam;
	// paddle 1
	double pd1_x = paddle1 -> x;
    double pd1_y = paddle1 -> y;
    double pd1_x_ = pd1_x;
    double pd1_y_ = pd1_y;
    double pd1_vx = paddle1 -> vx;
    double pd1_vy = paddle1 -> vy;
    double pd1_diam = paddle1 -> diam;
	// paddle 2
	double pd2_x = paddle2 -> x;
    double pd2_y = paddle2 -> y;
    double pd2_x_ = pd2_x;
    double pd2_y_ = pd2_y;
    double pd2_vx = paddle2 -> vx;
    double pd2_vy = paddle2 -> vy;
    double pd2_diam = paddle2 -> diam;
	// physics vars
	bool collision;
	double m, pA, pB, pC, wA, wB, wC, interX, interY, ddt, minDDT, maxDDT, dist, minDist, maxDist, difX, difY, difVX, difVY;
    
    // change velocity
    puckFriction(&pk_vx,&pk_vy,dt);
	paddleFriction(paddle1,dt);
	paddleFriction(paddle2,dt);
	// change position
    pk_x_ += pk_vx*dt;
    pk_y_ += pk_vy*dt;
	// puck friction doesn't change these values
	pd1_x = paddle1 -> x;
	pd1_y = paddle1 -> y;
	pd1_vx = paddle1 -> vx;
	pd1_vy = paddle1 -> vy;
	pd1_x_ = pd1_x + pd1_vx*dt;
	pd1_y_ = pd1_y + pd1_vy*dt;
	pd2_x = paddle2 -> x;
	pd2_y = paddle2 -> y;
	pd2_vx = paddle2 -> vx;
	pd2_vy = paddle2 -> vy;
	pd2_x_ = pd2_x + pd2_vx*dt;
	pd2_y_ = pd2_y + pd2_vy*dt;
	
	// paddle collisions
	/* PADDLE COLLISION HANDLING IDEA
	 * 1. if the paddle acts as a second simultaneous object that can bounce off walls, then there are edge cases for missed collisions if its physics are handled before or after the puck
	 * 2. to get around this, we are going to be lazy and just cut off the paddle's path and reverse its velocity when it hits a wall
	 * 3. this way, we won't have edge cases to worry about, and *hopefully* the player will not notice too much (the frame rate should be high enough to minimize errors)
	 
	 * 4. or... what if we just don't give the paddle collision? It shouldn't be noticeable, and it makes things so much simpler
	 */
    
	// puck collisions
	/* PUCK COLLISION HANDLING IDEA
     * 1. create line segment from current position to new position at end of frame
     * repeat until no collisions:
     * 2. detect/handle wall collisions
     *   1. move pointA of line segment to point of collision
     *   2. mirror pointB of line segment across pointA, parallel to the given wall
     *   3. return to start of loop
	 // DONE UP TO HERE
     * 3. detect/handle paddle collisions
     *   1. find the distance between the path of the puck and the path of the paddle at time ddt
	 *   2. if this distance indicates there is a collision, find the point of time of the collision using a binary search
	 *   3. calculate the point of collision at the time of collision and move pointA of the puck's path to the POC
     *   4. assume Tangent Of Collision is a wall, find pointB using components parallel and perpendicular to the TOC
     *   5. negate pointB's perpendicular components to reflect pointB across the TOC
     *   6. return to start of loop
	 * 4. move paddles, only need to worry about wall collisions
    */
    collision = true;
	bool Pad1Col = false;
	bool Pad2Col = false;
	bool WalCol = false;
	
    while(collision && !((Pad1Col||Pad2Col) && WalCol) ){
		if(pk_y_<0-pk_diam/2 || pk_y_>800+pk_diam/2){ // the puck is in the goal
			if(pk_y_<0){ // give plr 1 the goal
				player1.score++;
				kickoff = 1;
				ctx->audio->playSound(ctx->assets->getSound("goal1"));
			}
			else{ // give plr 2 the goal
				player2.score++;
				kickoff = 2;
				ctx->audio->playSound(ctx->assets->getSound("goal2"));
			}
			pk_x_ = 300;
			pk_y_ = 400;
			pk_vx = 0;
			pk_vy = 0;
			// end game
			if(player1.score>=11 || player2.score>=11){
				ctx->audio->stopSound();
				ctx->audio->playSound(ctx->assets->getSound("inter"));
				ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
			}
			break;
		}
		
		collision = false;
		// do paddle collisions first, because in most cases, the puck will hit the paddle before the wall, and checking for the paddle first in the other cases will not mess anything up
		
		// paddle 1 collision
		// calculate distance
		minDist = hypot(pk_x-pd1_x,pk_y-pd1_y);
		maxDist = hypot(pk_x_-pd1_x_,pk_y_-pd1_y_);
		dist = hypot( ((pk_x+pk_x_)/2) - ((pd1_x+pd1_x_)/2) , ((pk_y+pk_y_)/2) - ((pd1_y+pd1_y_)/2) );
		// if distance at beginning/end shows a collision, or if the middle distance is less than either end (showing the distance fcn passed through zero), we have a collision
		if( minDist < pk_diam/2 + pd1_diam/2 || maxDist < pk_diam/2 + pd1_diam/2 ){
			/* The square of the distance is given by y = ((A+X(B)) - (C+X(D)))^2 + ((E+X(F)) - (G+X(H)))^2
			 * Where X is a fraction of the current frame, ranging from 0.0-1.0,
			 * A is the initial puck X position,
			 * B is the difference between the final puck X position and the initial puck X position,
			 * C is the initial paddle X position,
			 * D is the difference between the final paddle X position and the initial paddle X position,
			 * and E-H represent the same values but for the Y axis
			 
			 * The derivative of this, is given by y' = 2(B-D)^2X + 2(A-C)(B-D) + 2(F-H)^2X + 2(E-G)(F-H)
			 * Solving for when the derivative equals zero, we can find the point within the frame that the puck and paddle are closest to each other
			 * Plugging this point into the original equation, we can find whether the puck and paddle collided
			 
			 * alternatively, we can just search for whether there is a collision at the beginning or end of the frame
			 * this will only fail if the puck moves fast enough to travel through the entire paddle in one frame
			 */
			collision = true;
			Pad1Col = true;
			ctx->audio->playSound(ctx->assets->getSound("Hit"));
			if(minDist<pk_diam/2 + pd1_diam/2){ // we are colliding at the first point (highly unlikely)
				ddt = 0; // after this step, ddt will be the best guess time of collision
			}
			else{ // we need to do a binary search to find point of collision
				// we want to find the first point of collision, so our goal is to never set minDDT to a point of collision, and set maxDDt to the smallest point of collision
				minDDT = 0;
				maxDDT = 1;
				ddt = 0.5;
				// dist is still at the midpoint from the last step, so we do not need to recalculate it
				if(dist<pk_diam/2+pd1_diam/2){ // this is a point of collision
					maxDDT = ddt;
					maxDist = dist;
				}
				else{ // this is not a point of collision
					// we can still figure out on which side of the collision we are
					if(minDist<maxDist && dist<maxDist){ // if maxDist is the largest, it is the furthest away from the collision
						maxDDT = ddt;
						maxDist = dist;
					}
					else{ // otherwise, minDist is the furthest away from the collision (minDist and maxDist will never be equidistant if the middle point is not a collision)
						minDDT = ddt;
						minDist = dist;
					}
				}
				for(int i = iter; i>0; i--){
					// find distance of the original points plus some fraction of the difference between them and the final points
					// if ddt is 0.5, we look at the points halfway inbetween on both lines
					dist = hypot( (pk_x + ddt*(pk_x_-pk_x)) - (pd1_x + ddt*(pd1_x_-pd1_x)) , (pk_y + ddt*(pk_y_-pk_y)) - (pd1_y + ddt*(pd1_y_-pd1_y)) );
					if(dist<pk_diam/2+pd1_diam/2){ // this is a point of collision
						maxDDT = ddt;
						maxDist = dist;
					}
					else{ // this is not a point of collision
						// we can still figure out on which side of the collision we are
						if(minDist<maxDist && dist<maxDist){ // if maxDist is the largest, it is the furthest away from the collision
							maxDDT = ddt;
							maxDist = dist;
						}
						else{ // otherwise, minDist is the furthest away from the collision (minDist and maxDist will never be equidistant if the middle point is not a collision)
							minDDT = ddt;
							minDist = dist;
						}
					}
					
					// prepare for next iteration
					i--;
					ddt = (minDDT+maxDDT)/2;
				}
				// we now, hopefully, have the closest non-collision value stored in minDDT
				ddt = minDDT;//(minDDT+maxDDT)/2;
			}
			// here, ddt will hold our best guess of when the point of collision was
			// set puck position to the point of collision
			// this is the point in time where the two center-line paths of the puck and the paddle become close enough for a collision, no offset is necessary
			pk_x = pk_x + ddt*(pk_x_ - pk_x);
			pk_y = pk_y + ddt*(pk_y_ - pk_y);
			pd1_x = pd1_x + ddt*(pd1_x_ - pd1_x);
			pd1_y = pd1_y + ddt*(pd1_y_ - pd1_y);
			// now, to make the math easier, we need to create a reference frame where the paddle is not moving
			difX = pd1_x_ - pd1_x;
			difY = pd1_y_ - pd1_y;
			pd1_x_ -= difX;
			pd1_y_ -= difY;
			pk_x_ -= difX;
			pk_y_ -= difY;
			difVX = pd1_vx;
			difVY = pd1_vy;
			// we don't bother changing paddle velocity because it won't be used in the calculations
			if(pk_vx==difVX && pk_vy==difVY){ // if *SOMEHOW* the paddle and puck collide with the exact same velocities, the collision will not be resolved and the puck will be left colliding with the paddle in an infinite loop
				pk_vx -= 0.99*difVX;
				pk_vy -= 0.99*difVY;
			}
			else{
				pk_vx -= difVX;
				pk_vy -= difVY;
			}
			// we now have a reference frame where the paddle is not moving and the puck is moving into the paddle
			// we need the puck to bounce off of the tanget of the paddle as if it is a wall
			//find the angle with respect to the x axis of the line passing through both objects' centers
			m = pd1_x - pk_x;
			if(m==0){ // both objects are vertically aligned, this just got a lot easier
				pk_y_ -= 2*(pk_y_ - pk_y); // reflect y velocity upward
				pk_vy *= -1; // reflect velocity upward
			}
			else{
				m = (pd1_y - pk_y) / m; // if you think about it, slope just defines two sides of a right triangle, dy/dx = opposite/adjacent
				double angle = atan(m);
				double par = 0;
				double perp = 0;
				// we only need to reflect components of velocities that are going into the paddle
				if( (pk_x<pd1_x && pk_vx>0) || (pk_x>pd1_x && pk_vx<0) ){ // we are moving horizontally in the direction of the paddle
					par += pk_vx*sin(angle);
					perp += -1*pk_vx*cos(angle); // reflect perpendicular components to bounce
					pk_vx = 0;
				}
				if( (pk_y<pd1_y && pk_vy>0) || (pk_y>pd1_y && pk_vy<0) ){ // we are moving vertically in the direction of the paddle
					par += pk_vy*cos(angle);
					perp += -1*pk_vy*sin(angle); // reflect perpendicular components to bounce
					pk_vy = 0;
				}
				// reconstruct the velocities from the parallel / perpedicular component vectors
				pk_vx += par*sin(angle);
				pk_vy += par*cos(angle);
				pk_vx += perp*cos(angle);
				pk_vy += perp*sin(angle);
				// reconstruct where the final position of the puck should be according to these adjusted velocities
				pk_x_ = pk_x + pk_vx*dt*(1-ddt);
				pk_y_ = pk_y + pk_vy*dt*(1-ddt);
				// leave reference frame where paddle is not moving
				pd1_x_ += difX;
				pd1_y_ += difY;
				pk_x_ += difX;
				pk_y_ += difY;
				pk_vx += difVX;
				pk_vy += difVY;
			}
			// house keeping, I don't think we need anything here
			continue;
		}
		
		// paddle 2 collision
		// calculate distance
		minDist = hypot(pk_x-pd2_x,pk_y-pd2_y);
		maxDist = hypot(pk_x_-pd2_x_,pk_y_-pd2_y_);
		dist = hypot( ((pk_x+pk_x_)/2) - ((pd2_x+pd2_x_)/2) , ((pk_y+pk_y_)/2) - ((pd2_y+pd2_y_)/2) );
		// if distance at beginning/end shows a collision, or if the middle distance is less than either end (showing the distance fcn passed through zero), we have a collision
		if( minDist < pk_diam/2 + pd2_diam/2 || maxDist < pk_diam/2 + pd2_diam/2 ){
			/* The square of the distance is given by y = ((A+X(B)) - (C+X(D)))^2 + ((E+X(F)) - (G+X(H)))^2
			 * Where X is a fraction of the current frame, ranging from 0.0-1.0,
			 * A is the initial puck X position,
			 * B is the difference between the final puck X position and the initial puck X position,
			 * C is the initial paddle X position,
			 * D is the difference between the final paddle X position and the initial paddle X position,
			 * and E-H represent the same values but for the Y axis
			 
			 * The derivative of this, is given by y' = 2(B-D)^2X + 2(A-C)(B-D) + 2(F-H)^2X + 2(E-G)(F-H)
			 * Solving for when the derivative equals zero, we can find the point within the frame that the puck and paddle are closest to each other
			 * Plugging this point into the original equation, we can find whether the puck and paddle collided
			 
			 * alternatively, we can just search for whether there is a collision at the beginning or end of the frame
			 * this will only fail if the puck moves fast enough to travel through the entire paddle in one frame
			 */
			collision = true;
			Pad2Col = true;
			ctx->audio->playSound(ctx->assets->getSound("Hit"));
			if(minDist<pk_diam/2 + pd2_diam/2){ // we are colliding at the first point (highly unlikely)
				ddt = 0; // after this step, ddt will be the best guess time of collision
			}
			else{ // we need to do a binary search to find point of collision
				// we want to find the first point of collision, so our goal is to never set minDDT to a point of collision, and set maxDDt to the smallest point of collision
				minDDT = 0;
				maxDDT = 1;
				ddt = 0.5;
				// dist is still at the midpoint from the last step, so we do not need to recalculate it
				if(dist<pk_diam/2+pd2_diam/2){ // this is a point of collision
					maxDDT = ddt;
					maxDist = dist;
				}
				else{ // this is not a point of collision
					// we can still figure out on which side of the collision we are
					if(minDist<maxDist && dist<maxDist){ // if maxDist is the largest, it is the furthest away from the collision
						maxDDT = ddt;
						maxDist = dist;
					}
					else{ // otherwise, minDist is the furthest away from the collision (minDist and maxDist will never be equidistant if the middle point is not a collision)
						minDDT = ddt;
						minDist = dist;
					}
				}
				for(int i = iter; i>0; i--){
					// find distance of the original points plus some fraction of the difference between them and the final points
					// if ddt is 0.5, we look at the points halfway inbetween on both lines
					dist = hypot( (pk_x + ddt*(pk_x_-pk_x)) - (pd2_x + ddt*(pd2_x_-pd2_x)) , (pk_y + ddt*(pk_y_-pk_y)) - (pd2_y + ddt*(pd2_y_-pd2_y)) );
					if(dist<pk_diam/2+pd2_diam/2){ // this is a point of collision
						maxDDT = ddt;
						maxDist = dist;
					}
					else{ // this is not a point of collision
						// we can still figure out on which side of the collision we are
						if(minDist<maxDist && dist<maxDist){ // if maxDist is the largest, it is the furthest away from the collision
							maxDDT = ddt;
							maxDist = dist;
						}
						else{ // otherwise, minDist is the furthest away from the collision (minDist and maxDist will never be equidistant if the middle point is not a collision)
							minDDT = ddt;
							minDist = dist;
						}
					}
					
					// prepare for next iteration
					i--;
					ddt = (minDDT+maxDDT)/2;
				}
				// we now, hopefully, have the closest non-collision value stored in minDDT
				ddt = minDDT;//(minDDT+maxDDT)/2;
			}
			// here, ddt will hold our best guess of when the point of collision was
			// set puck position to the point of collision
			// this is the point in time where the two center-line paths of the puck and the paddle become close enough for a collision, no offset is necessary
			pk_x = pk_x + ddt*(pk_x_ - pk_x);
			pk_y = pk_y + ddt*(pk_y_ - pk_y);
			pd2_x = pd2_x + ddt*(pd2_x_ - pd2_x);
			pd2_y = pd2_y + ddt*(pd2_y_ - pd2_y);
			// now, to make the math easier, we need to create a reference frame where the paddle is not moving
			difX = pd2_x_ - pd2_x;
			difY = pd2_y_ - pd2_y;
			pd2_x_ -= difX;
			pd2_y_ -= difY;
			pk_x_ -= difX;
			pk_y_ -= difY;
			difVX = pd2_vx;
			difVY = pd2_vy;
			// we don't bother changing paddle velocity because it won't be used in the calculations
			pk_vx -= difVX;
			pk_vy -= difVY;
			// we now have a reference frame where the paddle is not moving and the puck is moving into the paddle
			// we need the puck to bounce off of the tanget of the paddle as if it is a wall
			//find the angle with respect to the x axis of the line passing through both objects' centers
			m = pd2_x - pk_x;
			if(m==0){ // both objects are vertically aligned, this just got a lot easier
				pk_y_ -= 2*(pk_y_ - pk_y); // reflect y velocity upward
				pk_vy *= -1; // reflect velocity upward
			}
			else{
				m = (pd2_y - pk_y) / m; // if you think about it, slope just defines two sides of a right triangle, dy/dx = opposite/adjacent
				double angle = atan(m);
				double par = 0;
				double perp = 0;
				// we only need to reflect components of velocities that are going into the paddle
				if( (pk_x<pd2_x && pk_vx>0) || (pk_x>pd2_x && pk_vx<0) ){ // we are moving horizontally in the direction of the paddle
					par += pk_vx*sin(angle);
					perp += -1*pk_vx*cos(angle); // reflect perpendicular components to bounce
					pk_vx = 0;
				}
				if( (pk_y<pd2_y && pk_vy>0) || (pk_y>pd2_y && pk_vy<0) ){ // we are moving vertically in the direction of the paddle
					par += pk_vy*cos(angle);
					perp += -1*pk_vy*sin(angle); // reflect perpendicular components to bounce
					pk_vy = 0;
				}
				// reconstruct the velocities from the parallel / perpedicular component vectors
				pk_vx += par*sin(angle);
				pk_vy += par*cos(angle);
				pk_vx += perp*cos(angle);
				pk_vy += perp*sin(angle);
				// reconstruct where the final position of the puck should be according to these adjusted velocities
				pk_x_ = pk_x + pk_vx*dt*(1-ddt);
				pk_y_ = pk_y + pk_vy*dt*(1-ddt);
				// leave reference frame where paddle is not moving
				pd2_x_ += difX;
				pd2_y_ += difY;
				pk_x_ += difX;
				pk_y_ += difY;
				pk_vx += difVX;
				pk_vy += difVY;
			}
			// house keeping, I don't think we need anything here
			continue;
		}
		
		// wall collisions
		if(pk_x_ < pk_diam/2){ // left wall
			collision = true;
			WalCol = true;
			ctx->audio->playSound(ctx->assets->getSound("Bounce"));
			// find standard equation of line that the puck travels
				// note that we will shift the entire line to the left by 25 units so that we are following the left side of the puck and set the exact intersection point
			// calculate puck line like this to avoid errors with moving vertically
			m = pk_x_ - pk_x;
			if(m==0){
				pA = 1;
				pB = 0;
				pC = -( pk_x-(pk_diam/2) );
			}
			else{
				m = (pk_y_ - pk_y) / m; // the slope is not affected by shifting the line, so we don't bother here
				pA = m;
				pB = 1;
				pC = m*( pk_x-(pk_diam/2) ) - pk_y;
			}
			// wall equation (vertical line on origin)
			wA = 1;
			wB = 0;
			wC = 0;
			// intersection point
			interX = (pB*wC - wB*pC) / (pA*wB - wA*pB);
			interY = (wA*pC - pA*wC) / (pA*wB - wA*pB);
			// move puck
			pk_x = interX + (pk_diam/2); // move the puck so that only its edge is touching the wall
			pk_y = interY;
			// reflect x_ across x coord of puck for correct bounce
			pk_x_ += 2*(pk_x - pk_x_);
			pk_vx *= -1;
			// do not fuck with walls slowing down the puck
			continue;
        }
		if(pk_x_ > 600 - pk_diam/2){ // right wall
			collision = true;
			WalCol = true;
			ctx->audio->playSound(ctx->assets->getSound("Bounce"));
			// find standard equation of line that the puck travels
				// note that we will shift the entire line to the right by 25 units so that we are following the right side of the puck and set the exact intersection point
			// calculate puck line like this to avoid errors with moving vertically
			m = pk_x_ - pk_x;
			if(m==0){
				pA = 1;
				pB = 0;
				pC = -( pk_x+(pk_diam/2) );
			}
			else{
				m = (pk_y_ - pk_y) / m; // the slope is not affected by shifting the line, so we don't bother here
				pA = m;
				pB = 1;
				pC = m*( pk_x+(pk_diam/2) ) - pk_y;
			}
			// wall equation (vertical line on origin)
			wA = 1;
			wB = 0;
			wC = -600;
			// intersection point
			interX = (pB*wC - wB*pC) / (pA*wB - wA*pB);
			interY = (wA*pC - pA*wC) / (pA*wB - wA*pB);
			// move puck
			pk_x = interX - (pk_diam/2); // move the puck so that only its edge is touching the wall
			pk_y = interY;
			// reflect x_ across x coord of puck for correct bounce
			pk_x_ += 2*(pk_x - pk_x_);
			pk_vx *= -1;
			// do not fuck with walls slowing down the puck
			continue;
        }
		if(pk_y_ < pk_diam/2 && (pk_x_<200+pk_diam/2 || pk_x_>400-pk_diam/2) ){ // top wall (exclude goal from collisions)
			collision = true;
			WalCol = true;
			ctx->audio->playSound(ctx->assets->getSound("Bounce"));
			// find standard equation of line that the puck travels
				// note that we will shift the entire line up by 25 units so that we are following the top side of the puck and set the exact intersection point
			// calculate puck line like this to avoid errors with moving vertically
			m = pk_x_ - pk_x;
			if(m==0){
				interX = pk_x;
				interY = 0;
			}
			else{
				m = (pk_y_ - pk_y) / m; // the slope is not affected by shifting the line, so we don't bother here
				pA = m;
				pB = 1;
				pC = m*pk_x - ( pk_y-(pk_diam/2) );
				// wall equation (horizontal line on origin)
				wA = 0;
				wB = 1;
				wC = 0;
				// intersection point
				interX = (pB*wC - wB*pC) / (pA*wB - wA*pB);
				interY = (wA*pC - pA*wC) / (pA*wB - wA*pB);
			}
			// move puck
			pk_x = interX;
			pk_y = interY + (pk_diam/2); // move the puck so that only its edge is touching the wall
			// reflect y_ across y coord of puck for correct bounce
			pk_y_ += 2*(pk_y - pk_y_);
			pk_vy *= -1;
			// do not fuck with walls slowing down the puck
			continue;
        }
		if(pk_y_ > 800 - pk_diam/2 && (pk_x_<200+pk_diam/2 || pk_x_>400-pk_diam/2) ){ // bottom wall (exclude goal from collisions)
			collision = true;
			WalCol = true;
			ctx->audio->playSound(ctx->assets->getSound("Bounce"));
			// find standard equation of line that the puck travels
				// note that we will shift the entire line down by 25 units so that we are following the bottom side of the puck and set the exact intersection point
			// calculate puck line like this to avoid errors with moving vertically
			m = pk_x_ - pk_x;
			if(m==0){
				interX = pk_x;
				interY = 800;
			}
			else{
				m = (pk_y_ - pk_y) / m; // the slope is not affected by shifting the line, so we don't bother here
				pA = m;
				pB = 1;
				pC = m*pk_x - ( pk_y-(pk_diam/2) );
				// wall equation (horizontal line on origin)
				wA = 0;
				wB = 1;
				wC = -800;
				// intersection point
				interX = (pB*wC - wB*pC) / (pA*wB - wA*pB);
				interY = (wA*pC - pA*wC) / (pA*wB - wA*pB);
			}
			// move puck
			pk_x = interX;
			pk_y = interY - (pk_diam/2); // move the puck so that only its edge is touching the wall
			// reflect x_ across y coord of puck for correct bounce
			pk_y_ += 2*(pk_y - pk_y_);
			pk_vy *= -1;
			// do not fuck with walls slowing down the puck
			continue;
        }
    }
    
	// unstick the puck if it is pinned between the paddle and the wall
	if(Pad1Col && WalCol){
		pk_x_ = pd1_x_;
		pk_y_ = pd1_y_ - pk_diam/2 - pd1_diam/2;
		if( pow(pk_vy,2) < pow(pd1_diam*2,2) ){
			pk_vy = pd1_diam*2 * (pk_vy<0?-1:1);
		}
	}
	if(Pad2Col && WalCol){
		pk_x_ = pd2_x_;
		pk_y_ = pd2_y_ - pk_diam/2 - pd2_diam/2;
		if( pow(pk_vy,2) < pow(pd2_diam*2,2) ){
			pk_vy = pd2_diam*2 * (pk_vy<0?-1:1);
		}
	}
	
	puck -> x = pk_x_;
    puck -> y = pk_y_;
    puck -> vx = pk_vx;
    puck -> vy = pk_vy;
	
	paddle1 -> x = pd1_x_;
	paddle1 -> y = pd1_y_;
	paddle1 -> vx = pd1_vx;
	paddle1 -> vy = pd1_vy;
	
	paddle2 -> x = pd2_x_;
	paddle2 -> y = pd2_y_;
	paddle2 -> vx = pd2_vx;
	paddle2 -> vy = pd2_vy;
	
	return;
}

void AirHockeyGameState::init(Context* ctx){
    State::init(ctx);
	
	// debugging
    std::cout << "\nAirHockeyGameState Created!" << std::endl;
    std::cout << "Consolas" << &ctx -> assets -> getFont("Consolas") << std::endl;
    std::cout << "ST-SimpleSquare" << &ctx -> assets -> getFont("ST-SimpleSquare") << std::endl;
    
    float width = ctx -> p1window -> getSize().x;
    float height = ctx -> p1window -> getSize().y;
	double screenRatio = width / 320.0;
    //screenRatio = 6;
    std::cout << "Screen Ratio: " << screenRatio << std::endl;
	
	field.emplace(ctx -> assets -> getAsset("Field"));
	field -> setScale({(float) screenRatio, (float) screenRatio});
    fieldBack.emplace(ctx -> assets -> getAsset("FieldBack"));
	fieldBack -> setScale({(float) screenRatio, (float) screenRatio});
	fieldBack -> setPosition({0.f, (float)(height - (8 * screenRatio))});
	
	// text
	if(ctx -> p1name != "") // set player name while making sure the placeholder is not removed if necessary
		player1.name = ctx->p1name;
	if(ctx -> p2name != "")
		player2.name = ctx->p2name;
}

void AirHockeyGameState::tick() {
    time = clock.restart();
    dt = time.asSeconds();
	
	// puck dev controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)){
        puck.x = 100;
        puck.y = 400;
        puck.vx = 0;
        puck.vy = -750;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)){
        puck.x = 300;
        puck.y = 400;
        puck.vx = 0;
        puck.vy = -750;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)){
        puck.x = 500;
        puck.y = 400;
        puck.vx = 0;
        puck.vy = -750;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)){
        puck.x = 300;
        puck.y = 775;
        puck.vx = -550;
        puck.vy = -750;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)){
        puck.x = 300;
        puck.y = 775;
        puck.vx = -775;
        puck.vy = -750;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)){
        puck.x = 300;
        puck.y = 200;
        puck.vx = -550;
        puck.vy = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)){
        puck.x = 300;
        puck.y = 400;
        puck.vx = -550;
        puck.vy = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)){
        puck.x = 300;
        puck.y = 600;
        puck.vx = -550;
        puck.vy = 0;
    }
    // speed up / slow down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)){
        puck.vx *= 2;
        puck.vy *= 2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)){
        puck.vx  = puck.vx/1.25;
        puck.vy = puck.vy/1.25;
    }
	//paddle movement (need to virtually unpress keys so only one press is registered at a time
    // player 1
	if (ctx -> input -> P1_Up && !Up){
        if(p1paddle.yPos>=11)
            p1paddle.yPos -= 1;
        Up = true;
    }
    if (!ctx -> input -> P1_Up && Up){
        Up = false;
    }
    if (ctx -> input -> P1_Down && !Down){
        if(p1paddle.yPos<=11)
            p1paddle.yPos += 1;
        Down = true;
    }
    if (!ctx -> input -> P1_Down && Down){
        Down = false;
    }
    if (ctx -> input -> P1_Left && !Left){
        if(p1paddle.xPos>=2)
            p1paddle.xPos -= 1;
        Left = true;
    }
    if (!ctx -> input -> P1_Left && Left){
        Left = false;
    }
    if (ctx -> input -> P1_Right && !Right){
        if(p1paddle.xPos <= 4)
            p1paddle.xPos += 1;
        Right = true;
    }
    if (!ctx -> input -> P1_Right && Right){
        Right = false;
    }
    // player 2
	if (ctx -> input -> P2_Up && !W){
		if(p2paddle.yPos<=2)
            p2paddle.yPos += 1;
        W = true;
    }
    if (!ctx -> input -> P2_Up && W){
        W = false;
    }
    if (ctx -> input -> P2_Down && !S){
        if(p2paddle.yPos>=2)
            p2paddle.yPos -= 1;
        S = true;
    }
    if (!ctx -> input -> P2_Down && S){
        S = false;
    }
    if (ctx -> input -> P2_Left && !A){
		if(p2paddle.xPos<=4)
            p2paddle.xPos += 1;
        A = true;
    }
    if (!ctx -> input -> P2_Left && A){
        A = false;
    }
    if (ctx -> input -> P2_Right && !D){
        if(p2paddle.xPos>=2)
            p2paddle.xPos -= 1;
        D = true;
    }
    if (!ctx -> input -> P2_Right && D){
        D = false;
    }
    // emergency game exit
	if(ctx->input->P1B && ctx->input->P1Y && ctx->input->P2B && ctx->input->P2X){ // player 1 pressed B and Y, and player 2 pressed B and X at the same time to quit
		ctx -> gsm -> requestStateChange(States::GameSelect, 3.0f, 1.5f);
	}
	
	// kickoff
    if(kickoff>=0){
		puck.x = 300;
		puck.y = 400;
		puck.vx = 0;
		puck.vy = 0;
		timer += dt;
		if(timer >= 3){
			if(kickoff == 0) {
				ctx->audio->playSound(ctx->assets->getSound("cheer"));
			}
			puck.setKickoff(kickoff);
			timer = 0;
			kickoff = -1;
		}
	}
	
	moveObjects(&puck, &p1paddle, &p2paddle, dt, 10);
}

void AirHockeyGameState::p1render(sf::RenderWindow& p1window) {
    p1window.clear();
	
	player1.draw1(&p1window, ctx->assets->getFont("ST-SimpleSquare"));
	player2.draw2(&p1window, ctx->assets->getFont("ST-SimpleSquare"));
	
    p1window.draw(*field);
	
	if(kickoff>=0){
		int num;
		p2paddle.draw1(&p1window);
		if(timer<1){
			num = round(timer*2);
			if(num%2==0)
				puck.draw1(&p1window);
		}
		if(timer<2){
			num = round(timer*8);
			if(num%2==0)
				puck.draw1(&p1window);
		}
		else{
			num = round(timer*16);
			if(num%2==0)
				puck.draw1(&p1window);
		}
		p1paddle.draw1(&p1window);
	} else {
		// decide drawing order
		if(puck.y < p2paddle.y){
			puck.draw1(&p1window);
			p2paddle.draw1(&p1window);
			p1paddle.draw1(&p1window);
		}else if(puck.y > p1paddle.y){
			p2paddle.draw1(&p1window);
			p1paddle.draw1(&p1window);
			puck.draw1(&p1window);
		}else{
			p2paddle.draw1(&p1window);
			puck.draw1(&p1window);
			p1paddle.draw1(&p1window);
		}
	}
    p1window.draw(*fieldBack); //this covers the wall closest to the player, so the pucks and paddles are correctly obscured from view
}

void AirHockeyGameState::p2render(sf::RenderWindow& p2window) {
    p2window.clear();
	
	player1.draw2(&p2window, ctx->assets->getFont("ST-SimpleSquare"));
	player2.draw1(&p2window, ctx->assets->getFont("ST-SimpleSquare"));
	
    p2window.draw(*field);
	
	if(kickoff>=0){
		int num;
		p2paddle.draw2(&p2window);
		if(timer<1){
			num = round(timer*2);
			if(num%2==0)
				puck.draw2(&p2window);
		}
		if(timer<2){
			num = round(timer*8);
			if(num%2==0)
				puck.draw2(&p2window);
		}
		else{
			num = round(timer*16);
			if(num%2==0)
				puck.draw2(&p2window);
		}
		p1paddle.draw2(&p2window);
	} else {
		// decide drawing order
		if(puck.y < p2paddle.y){
			puck.draw2(&p2window);
			p2paddle.draw2(&p2window);
			p1paddle.draw2(&p2window);
		}else if(puck.y > p1paddle.y){
			p2paddle.draw2(&p2window);
			p1paddle.draw2(&p2window);
			puck.draw2(&p2window);
		}else{
			p2paddle.draw2(&p2window);
			puck.draw2(&p2window);
			p1paddle.draw2(&p2window);
		}
	}
    p2window.draw(*fieldBack); //this covers the wall closest to the player, so the pucks and paddles are correctly obscured from view
}