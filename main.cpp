#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp> // I believe <vector.h> is included here

struct Fruit{
	sf::Texture tileSet;
	int x;
	double y;
	int w;
	int h;
	double vy;
	int frame;
	float time;
	
	Fruit(sf::Texture tileSet, int x, int w, int h){
		this->tileSet = tileSet;
		this->x = x;
		this->y = -h;
		this->w = w;
		this->h = h;
		this->vy = 0;
		this->frame = 0;
		this->time = 0.0;
	}
	void move(float dt){
		time += dt;
		int n = round(time/0.075f); // account for lag where dt may be larger than the 0.075 time step we want
		for(int i = 0; i<n; i++){
			vy += 0.1;
			y += vy;
		}
		frame = (frame + n)%6;
		time = std::remainder(time,0.075); // bring time back down while including any carry over between time steps
	}
	void draw(sf::RenderWindow* window, double screenRatio){
		sf::Sprite sprite(tileSet);
		sprite.setScale(sf::Vector2f(screenRatio,screenRatio));
		sprite.setTextureRect(sf::IntRect( sf::Vector2(w*frame,0), sf::Vector2(w,h) )); // change this will change image we draw from tile set
		sprite.setPosition(sf::Vector2f( ((19-(w/2))+(38*x))*screenRatio , y*screenRatio ));
		window->draw(sprite);
	}
};

int main()
{
	// screen
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	sf::RenderWindow window(modes[0], "Fruit Fruit Revolution", sf::State::Fullscreen);
	double screenRatio = (double)(window.getSize().x) / 320.0;
	
	// time
	sf::Clock clock;
	sf::Time time;
	float dt;
	
	// textures
	sf::Texture backTex("assets/images/background.png");
	sf::Texture redTex("assets/images/redGuy_tileSet.png");
	sf::Texture greenTex("assets/images/greenGuy_tileSet.png");
	sf::Texture mellonTex("assets/images/mellon_tileSet.png");
	sf::Texture appleTex("assets/images/apple_tileSet.png");
	sf::Texture orangeTex("assets/images/orange_tileSet.png");
	sf::Texture lemonTex("assets/images/lemon_tileSet.png");
	std::vector<sf::Texture> fruitTexs = {mellonTex,appleTex,orangeTex,lemonTex};
	
	sf::Sprite back(backTex);
	back.setScale(sf::Vector2f(screenRatio,screenRatio));
	sf::Sprite guy(redTex);
	guy.setScale(sf::Vector2f(screenRatio,screenRatio));
	
	// animation vars
	int guyX = 0;
	int guyFacing = 0;
	int guyFrame = 0;
	bool guyRed = true;
	bool guyCanChange = false;
	bool guySwing = false;
	int guyMove = 0;
	float guyStep = 0;
	float guyTime = 0.0;
	
	// objects
	std::vector<Fruit*> fruits;
	int fallenFruit;
	float fruitTime = 0;
	int fruitType = 0;
	int x = 0;
	
	while ( window.isOpen() )
	{
		time = clock.restart();
		dt = time.asSeconds();
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
				window.close();
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab) && guyCanChange==false){ // prepare to change shirt color
				guyCanChange = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab) && guyCanChange==true){ // change shirt color
				if(guyRed){
					guy.setTexture(greenTex,false);
					guyRed = false;
				}
				else{
					guy.setTexture(redTex,false);
					guyRed = true;
				}
				guyCanChange = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){ // move left
				if(guyX>0 && !guySwing && guyMove==0){
					guyMove = -1;
					guyFrame = 5;
					guyStep = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){ // move right
				if(guyX<7 && !guySwing && guyMove==0){
					guyMove = 1;
					guyFrame = 4;
					guyStep = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){ // swing sword
				if(!guySwing && guyMove==0){
					guySwing = true;
					guyTime = 0;
					guyFrame = 1;
				}
			}
		}
		
		// animate guy
		if(guySwing){
			guyTime += dt;
			int n = round(guyTime/0.075f); // change frames every 0.075s (also account for lag that may cause a frame to be longer than 0.075s
			guyTime = std::remainder(guyTime,0.075f); // reset guyTime while retaining any overflow
			guyFrame += n;
			if(guyFrame >= 4){ // stop animation
				guySwing = false;
				guyFacing = (guyFacing+1)%2; // will alternate between o and 1
				guyFrame = 0;
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
		guy.setTextureRect(sf::IntRect( sf::Vector2(48*guyFrame,48*guyFacing), sf::Vector2(48,48) )); // change this will change image we draw from tile set
		guy.setPosition(sf::Vector2f( (3+(38*(guyX + (guyMove*guyStep/0.2) )))*screenRatio , window.getSize().y-(58*screenRatio) ));
		// spawn fruit
		if(fruitTime <= 0){
			fruitType = rand()%4;
			if(fruitType==0){
				fruits.push_back( new Fruit(fruitTexs[fruitType],x,20,16) ); // drop mellon
			}
			else{
				fruits.push_back( new Fruit(fruitTexs[fruitType],x,8,8) ); // drop other fruit (they are all the same size)
			}
			
			fruitTime = rand()%2; // choose whether the new fruit will fall to the left or to the right
			if(fruitTime==0){ // move the fruit a ranom number of columns (stay close enough that it is reachable within time)
				fruitTime = rand()&10; // random column movement between 0 and 9
				x += fruitTime;
				if(x>7)
					x = 7;
			}
			else{ // move in the other direction
				fruitTime = rand()&10; // random column movement between 0 and 9
				x -= fruitTime;
				if(x<0)
					x = 0;
			}
			fruitTime = (fruitTime+1)*0.3; // turn random column movement into random time spacing between fruits of 0.3s (same column) to 3.0s (any column plus a pause)
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
		
		// draw
		window.clear();
		// background
		window.draw(back);
		// guy
		window.draw(guy);
		// fruit
		for(int i = 0; i<fruits.size(); i++){
			fruits[i]->draw(&window, screenRatio);
		}
		window.display();
	}
	return 0;
}