#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>

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

struct Puck{
    double x;
    double y;
    double vx;
    double vy;
    double diam;
	//sf::Sprite sprite();
    sf::Texture pucks[28];
    Puck(double x, double y, double vx, double vy, double diam){
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
        this->diam = diam;
        pucks[0].loadFromFile("assets/images/puck_0.png");
		pucks[1].loadFromFile("assets/images/puck_1a.png");
		pucks[2].loadFromFile("assets/images/puck_1b.png");
		pucks[3].loadFromFile("assets/images/puck_1c.png");
		pucks[4].loadFromFile("assets/images/puck_2a.png");
		pucks[5].loadFromFile("assets/images/puck_2b.png");
		pucks[6].loadFromFile("assets/images/puck_2c.png");
		pucks[7].loadFromFile("assets/images/puck_3a.png");
		pucks[8].loadFromFile("assets/images/puck_3b.png");
		pucks[9].loadFromFile("assets/images/puck_3c.png");
		pucks[10].loadFromFile("assets/images/puck_4a.png");
		pucks[11].loadFromFile("assets/images/puck_4b.png");
		pucks[12].loadFromFile("assets/images/puck_4c.png");
		pucks[13].loadFromFile("assets/images/puck_5a.png");
		pucks[14].loadFromFile("assets/images/puck_5b.png");
		pucks[15].loadFromFile("assets/images/puck_5c.png");
		pucks[16].loadFromFile("assets/images/puck_6a.png");
		pucks[17].loadFromFile("assets/images/puck_6b.png");
		pucks[18].loadFromFile("assets/images/puck_6c.png");
		pucks[19].loadFromFile("assets/images/puck_7a.png");
		pucks[20].loadFromFile("assets/images/puck_7b.png");
		pucks[21].loadFromFile("assets/images/puck_7c.png");
		pucks[22].loadFromFile("assets/images/puck_8a.png");
		pucks[23].loadFromFile("assets/images/puck_8b.png");
		pucks[24].loadFromFile("assets/images/puck_8c.png");
		pucks[25].loadFromFile("assets/images/puck_9a.png");
		pucks[26].loadFromFile("assets/images/puck_9b.png");
		pucks[27].loadFromFile("assets/images/puck_9c.png");
    }
    int getIndex(double x, double y, double diam){
    if(y<diam/2) // goal, puck 0
        return 0;
    if(y<diam){
        if(x<200) // puck 1a
            return 1;
        if(x>400) // puck 1c
            return 3;
        return 2; // puck 1b
    }
    if(y<diam*3){
        if(x<200) // puck 2a
            return 4;
        if(x>400) // puck 2c
            return 6;
        return 5; // puck 2b
    }
    if(y<diam*5){
        if(x<200) // puck 3a
            return 7;
        if(x>400) // puck 3c
            return 9;
        return 8; // puck 3b
    }
    if(y<diam*7){
        if(x<200) // puck 4a
            return 10;
        if(x>400) // puck 4c
            return 12;
        return 11; // puck 4b
    }
    if(y<diam*9){
        if(x<200) // puck 5a
            return 13;
        if(x>400) // puck 5c
            return 15;
        return 14; // puck 5b
    }
    if(y<diam*11){
        if(x<200) // puck 6a
            return 16;
        if(x>400) // puck 6c
            return 18;
        return 17; // puck 6b
    }
    if(y<diam*13){
        if(x<200) // puck 7a
            return 19;
        if(x>400) // puck 7c
            return 21;
        return 20; // puck 7b
    }
    if(y<diam*15){
        if(x<200) // puck 8a
            return 22;
        if(x>400) // puck 8c
            return 24;
        return 23; // puck 8b
    }
    if(x<200) // puck 9a
        return 25;
    if(x>400) // puck 9c
        return 27;
    return 26; // puck 9b
}
    void draw(sf::RenderWindow* window){
        int index;
        double xOffset;
        double yOffset;
        double screenX;
        double screenY;
		double screenRatio = (double)(window->getSize().x) / 320.0;
        
        index = getIndex(x, y, diam);
        xOffset = (pucks[index].getSize().x/2)*screenRatio;
        yOffset = (pucks[index].getSize().y/2)*screenRatio;
        // as pucks get closer, you see their thickness, their offset needs to be adjusted so they are still drawn at the correct location
        if(index>12)
            yOffset += screenRatio;
        if(index>21)
            yOffset += screenRatio;
        if(index>24)
            yOffset += screenRatio;
        
        screenX = getScreenX(x, y, window->getSize().x);
        screenY = getScreenY(y, window->getSize().y);
        sf::Sprite sprite(pucks[index]);
		//&sprite.setTexture(pucks[index], true); // true to reset the sprite rectangle to the size of the new texture
		sprite.setPosition((sf::Vector2f){screenX-xOffset,screenY-yOffset});
		sprite.setScale((sf::Vector2f){screenRatio,screenRatio});
		window->draw(sprite);
		
        //DrawTexture(pucks[index],screenX-xOffset,screenY-yOffset,WHITE);
        //printf("\nPuck X: %lf\nPuck OffX: %lf\nPuck Y: %lf\nPuck OffY: %lf\n",screenX,xOffset,screenY,yOffset);
    }
};
struct Paddle{
    int xPos;
    int yPos;
    double x;
    double y;
    double vx;
    double vy;
    double diam;
	//sf::Sprite sprite();
    sf::Texture paddles[18];
    Paddle(int xPos, int yPos, double vx, double vy, double diam){
        this->xPos = xPos;
        this->yPos = yPos;
        this->x = xPos*100;
        this->y = (yPos*diam) - diam/2;
        this->vx = vx;
        this->vy = vy;
        this->diam = diam;
        paddles[0].loadFromFile("assets/images/paddle_1a.png");
        paddles[1].loadFromFile("assets/images/paddle_1b.png");
        paddles[2].loadFromFile("assets/images/paddle_1c.png");
        paddles[3].loadFromFile("assets/images/paddle_2a.png");
        paddles[4].loadFromFile("assets/images/paddle_2b.png");
        paddles[5].loadFromFile("assets/images/paddle_2c.png");
        paddles[6].loadFromFile("assets/images/paddle_3a.png");
        paddles[7].loadFromFile("assets/images/paddle_3b.png");
        paddles[8].loadFromFile("assets/images/paddle_3c.png");
        paddles[9].loadFromFile("assets/images/paddle_4a.png");
        paddles[10].loadFromFile("assets/images/paddle_4b.png");
        paddles[11].loadFromFile("assets/images/paddle_4c.png");
        paddles[12].loadFromFile("assets/images/paddle_5a.png");
        paddles[13].loadFromFile("assets/images/paddle_5b.png");
        paddles[14].loadFromFile("assets/images/paddle_5c.png");
        paddles[15].loadFromFile("assets/images/paddle_6a.png");
        paddles[16].loadFromFile("assets/images/paddle_6b.png");
        paddles[17].loadFromFile("assets/images/paddle_6c.png");
    }
    int getIndex(double x, double y, double diam){
    // used 198 and 402 instead of 200 and 400 to minimize image jittering on edges
    if(y<diam){
        if(x<195) // paddle 1a
            return 0;
        if(x>405) // paddle 1c
            return 2;
        return 1; // paddle 1b
    }
    if(y<diam*2){
        if(x<195) // paddle 2a
            return 3;
        if(x>405) // paddle 2c
            return 5;
        return 4; // paddle 2b
    }
    if(y<diam*3){
        if(x<195) // paddle 3a
            return 6;
        if(x>405) // paddle 3c
            return 8;
        return 7; // paddle 3b
    }
    if(y<diam*10){
        if(x<195) // paddle 4a
            return 9;
        if(x>405) // paddle 4c
            return 11;
        return 10; // paddle 4b
    }
    if(y<diam*11){
        if(x<195) // paddle 5a
            return 12;
        if(x>405) // paddle 5c
            return 14;
        return 13; // paddle 5b
    }
    if(x<195) // paddle 6a
        return 15;
    if(x>405) // paddle 6c
        return 17;
    return 16; // paddle 6b
}
    void draw(sf::RenderWindow* window){
        int index;
        double xOffset;
        double yOffset;
        double screenX;
        double screenY;
		double screenRatio = (double)(window->getSize().x) / 320.0;
        
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
        
        screenX = getScreenX(x, y, window->getSize().x);
        screenY = getScreenY(y, window->getSize().y);
		
		sf::Sprite sprite(paddles[index]);
		//&sprite.setTexture(paddles[index], true); // true to reset the sprite rectangle to the size of the new texture
		sprite.setPosition((sf::Vector2f){screenX-xOffset,screenY-yOffset});
		sprite.setScale((sf::Vector2f){screenRatio,screenRatio});
		window->draw(sprite);
        
        //DrawTexture(paddles[index],screenX-xOffset,screenY-yOffset,WHITE);
    }
};

void movePuck(Puck* puck, float dt){
    // field is 600x800
    double x = puck->x;
    double y = puck->y;
    double x_ = x;
    double y_ = y;
    double vx = puck->vx;
    double vy = puck->vy;
    double diam = puck->diam;
    
    /* COLLISION HANDLING IDEA
     * 1. create line segment from current position to new position at end of frame
     * repeat until no collisions:
     * 2. detect/handle wall collisions
     *   1. move pointA of line segment to point of collision
     *   2. mirror pointB of line segment across pointA, parallel to the given wall
     *   3. return to start of loop
     * 3. detect/handle paddle collisions
     *   1. find distance from point(paddle) to line(path of puck). If this distance is less than 59, there is a collision
     *   2. move pointA of line segment to point of collision
     *   3. assume Tangent Of Collision is a wall, find pointB using components parallel and perpendicular to the TOC
     *   4. negate pointB's perpendicular components to reflect pointB across the TOC
     *   5. return to start of loop
    */
    
    // base puck velocity should be 12 (~1s to travel down the field)
    //double v;
    //double angle;
    //double fric = 0.9;
    //// change velocity
    //if(vx==0){
    //    vy *= fric;
    //    if(vy<12)
    //        vy = 12;
    //}
    //else if(vy==0){
    //    vx *= fric;
    //    if(vx<12)
    //        vx = 12;
    //}
    //else{
    //    v = hypot(vx,vy);
    //    angle = atan(vy/vx);
    //    v *= fric;
    //    if(v<12)
    //        v = 12;
    //    vx = v*cos(angle);
    //    vy = v*sin(angle);
    //}
    //// change position
    //x' += vx;
    //y' += vy;
    //
    //bool collision = true;
    //while(collision){
    //    // wall collisions
    //    if(x<diam/2){ // left wall
    //        
    //    }
    //}
    
    x += vx*dt;
    if(x<diam/2){
        x += 2*((diam/2)-x); // bounce the puck out to avoid losing distance on collision frames
        if(vx<0) // make sure velocity points away from the wall
            vx *= -1;
        if(x>600-(diam/2)){ // if we bounced out to the other side of the field, keep us in bounds
            x = 600-(diam/2);
            vx = -550; // give the puck a reasonable velocity to recover with
        }
    }
    if(x>600-(diam/2)){
        x -= 2*(x-(600-(diam/2)) );
        if(vx>0) // make sure velocity points away from the wall
            vx *= -1;
        if(x<diam/2){ // if we bounced out to the other side of the field, keep us in bounds
            x = diam/2;
            vx = 550; // give the puck a reasonable velocity to recover with
        }
    }
    
    y += vy*dt;
    if(y<diam/2){
        y += 2*((diam/2)-y); // bounce the puck out to avoid losing distance on collision frames
        if(vy<0) // make sure velocity points away from the wall
            vy *= -1;
        if(y>800-(diam/2)){ // if we bounced out to the other side of the field, keep us in bounds
            y = 800-(diam/2);
            vy = -750; // give the puck a reasonable velocity to recover with
        }
    }
    if(y>800-(diam/2)){
        y -= 2*(y-(800-(diam/2)));
        if(vy>0) // make sure velocity points away from the wall
            vy *= -1;
        if(y<diam/2){ // if we bounced out to the other side of the field, keep us in bounds
            y = diam/2;
            vy = 750; // give the puck a reasonable velocity to recover with
        }
    }
    
    puck->x = x;
    puck->y = y;
    puck->vx = vx;
    puck->vy = vy;
}
void movePaddle(Paddle* paddle, float dt){
    int xPos = paddle->xPos;
    int yPos = paddle->yPos;
    double x = paddle->x;
    double y = paddle->y;
    double vx = paddle->vx;
    double vy = paddle->vy;
    double diam = paddle->diam;
    double acc;
    double accConst = 4;
    double fricConst = 5;
    
    // x
	acc = ((xPos*100)-x)/accConst;
	vx += acc;
	vx -= vx*fricConst*dt;
	x += vx*dt;
	// stabilize jittering
	if( ((xPos*100)-x)*((xPos*100)-x)<0.1 && vx*vx*dt*dt<0.1 ){
		x = xPos*100;
		vx = 0;
	}
    paddle->vx = vx;
    paddle->x = x;
    
    // y
	acc = ((yPos*diam)-(diam/2)-y)/accConst;
	vy += acc;
	vy -= vy*fricConst*dt;
	y += vy*dt;
	// return
	if(y>=(yPos*diam)-(diam/2)-1 && yPos==3)
		yPos = 2;
	if(y<=(yPos*diam)-(diam/2)+1 && yPos==10)
		yPos = 11;
	// stabilize jittering
	if( ((yPos*diam)-(diam/2)-y)*((yPos*diam)-(diam/2)-y)<0.1 && vy*vy*dt*dt<0.1 ){
		y = (yPos*diam)-(diam/2);
		vy = 0;
	}
    paddle->vy = vy;
    paddle->y = y;
    paddle->yPos = yPos;
}

int main()
{
	sf::RenderWindow window( sf::VideoMode( { 1280, 720 } ), "Air Hockey Arcade Game" );
	sf::Clock clock;
	sf::Time time;
	float dt;
	
	sf::Texture fieldTex("assets/images/field.png");
	sf::Texture fieldBackTex("assets/images/field_back.png");
	sf::Sprite field(fieldTex);
	sf::Sprite fieldBack(fieldBackTex);
	double screenRatio = (double)(window.getSize().x) / 320.0;
	field.setScale((sf::Vector2f){screenRatio,screenRatio});
	fieldBack.setScale((sf::Vector2f){screenRatio,screenRatio});
	fieldBack.setPosition((sf::Vector2f){0,window.getSize().y-(8*screenRatio)});
	
	Puck puck(300,400,0,0,50);
    Paddle p1paddle(3,11,0,0,67);
    Paddle p2paddle(3,2,0,0,67);
	
	while ( window.isOpen() )
	{
		time = clock.restart();
		dt = time.asSeconds();
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
				window.close();
			// puck movement
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)){
				puck.x = 100;
				puck.y = 775;
				puck.vx = 0;
				puck.vy = -750;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)){
				puck.x = 300;
				puck.y = 775;
				puck.vx = 0;
				puck.vy = -750;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)){
				puck.x = 500;
				puck.y = 775;
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)){
				puck.vx *= 1.25;
				puck.vy *= 1.25;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)){
				puck.vx  = puck.vx/1.25;
				puck.vy = puck.vy/1.25;
			}
			// paddle movement
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
				if(p1paddle.yPos>=11)
					p1paddle.yPos -= 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
				if(p1paddle.yPos<=11)
                p1paddle.yPos += 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
				if(p1paddle.xPos>=2)
					p1paddle.xPos -= 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
				if(p1paddle.xPos<=4)
					p1paddle.xPos += 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
				if(p2paddle.yPos>=2)
					p2paddle.yPos -= 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
				if(p2paddle.yPos<=2)
					p2paddle.yPos += 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
				if(p2paddle.xPos>=2)
					p2paddle.xPos -= 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
				if(p2paddle.xPos<=4)
					p2paddle.xPos += 1;
			}
		}
		
		movePuck(&puck,dt);
        movePaddle(&p1paddle,dt);
        movePaddle(&p2paddle,dt);
		
		window.clear();
		window.draw(field);
		// decide drawing order
		if(puck.y<p2paddle.y){
			puck.draw(&window);
			p2paddle.draw(&window);
			p1paddle.draw(&window);
		}
		else if(puck.y>p1paddle.y){
			p2paddle.draw(&window);
			p1paddle.draw(&window);
			puck.draw(&window);
		}
		else{
			p2paddle.draw(&window);
			puck.draw(&window);
			p1paddle.draw(&window);
		}
		window.draw(fieldBack);
		window.display();
	}
}