#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>

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
			if(guyTime>=0.075){ // change frames every 0.075s
				guyTime = 0;
				if(++guyFrame == 4){ // stop animation
					guySwing = false;
					guyFacing = (guyFacing+1)%2; // will alternate between 0 and 1
					guyFrame = 0;
				}
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
		guy.setTextureRect(sf::IntRect( sf::Vector2(48*guyFrame,48*guyFacing), sf::Vector2(48,48) )); // change this will change image we draw from tile set
		guy.setPosition(sf::Vector2f( (3+(38*(guyX + (guyMove*guyStep/0.2) )))*screenRatio , window.getSize().y-(58*screenRatio) ));
		
		// draw
		window.clear();
		window.draw(back);
		window.draw(guy);
		window.display();
	}
	return 0;
}