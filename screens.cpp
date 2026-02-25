#include "screens.hpp"

//This function displays the main title window for players while the machine is waiting.
//It displays the title, some screensaver animation, and a prompt to insert a coin (or payment method) to begin play.
int titleWindow(sf::RenderWindow &window) {
	/*
		Monitor Sizes for 16:9
		1280x720 (720p)
		1920x1080 (1080p)
		2560x1440 (2k)
		3840x2160 (4k)
	*/

	window.clear();
	
	//Start clock to monitor elapsed time. It is MONOTONIC, does not go by system clock.
	sf::Clock clock;
	sf::Time time;
	float dt;
	
	sf::Font title_font, give_money_font;
	if (!title_font.openFromFile("assets/fonts/TECHNOID.TTF")) { std::perror("File not found!"); }
	if (!give_money_font.openFromFile("assets/fonts/square_sans_serif_7.ttf")) { std::perror("File not found!"); }

	//Title text to be displayed at the top of the screen. Does not flash.
	sf::Text title_text(title_font, "Air Hockey Arcade", 75);
	const sf::FloatRect titleRect = title_text.getLocalBounds();
	title_text.setOrigin(titleRect.getCenter());
	title_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 5.0f));

	//Insert coin to be displayed at bottom of screen. Flashes.
	sf::Text play_text(give_money_font, "Insert Coin to Play!", 35);
	const sf::FloatRect textRect = play_text.getLocalBounds();
	play_text.setOrigin(textRect.getCenter());
	play_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 1.2f));

	//Main window loop
	while ( window.isOpen() ) {
		time = clock.getElapsedTime();
		dt = time.asSeconds();
		//Takes the stack and pops each event. We only care about the window closing for now.
		while (std::optional event = window.pollEvent()) {
			// close window
			if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
			// other events
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
				return 1;
			}
		}

		window.clear();
		window.draw(title_text);

		if ((int)dt % 2 == 0) {
			window.draw(play_text);
		}

		window.display();
	}

    return 1;
}

int gameSelectWindow(sf::RenderWindow &window){
	window.clear();
	
	//Start clock to monitor elapsed time. It is MONOTONIC, does not go by system clock.
	sf::Clock clock;
	sf::Time time;
	float dt_Arrow;
	
	// images
	sf::Texture arrows[4];
	arrows[0].loadFromFile("assets/images/gameSelectArrow_L1.png");
	arrows[1].loadFromFile("assets/images/gameSelectArrow_L2.png");
	arrows[2].loadFromFile("assets/images/gameSelectArrow_R1.png");
	arrows[3].loadFromFile("assets/images/gameSelectArrow_R2.png");
	
	sf::Texture demos[1];
	demos[0].loadFromFile("assets/images/gameSelectImage_AH.png");
	
	std::vector<int> indexes = getIndexes(0,1);
	
	sf::Sprite sprite(arrows[0]); // sprites cannot be created without a texture
	double screenRatio = (double)(window.getSize().x) / 320.0;
	
	// font
	sf::Font title_font;
	if (!title_font.openFromFile("assets/fonts/square_sans_serif_7.ttf")) { std::perror("File not found!"); }
	
	sf::Text title_text(title_font, "Air Hockey", 80);
	const sf::FloatRect textRect = title_text.getLocalBounds();
	title_text.setOrigin(textRect.getCenter());
	title_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 7.0f));
	
	//Main window loop
	while ( window.isOpen() ) {
		time = clock.getElapsedTime();
		dt_Arrow = time.asSeconds();
		
		while (std::optional event = window.pollEvent()) {
			// close window
			if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
			// other events
		}

		window.clear();
		window.draw(title_text);

		if(dt_Arrow<0.5){ // alternate frames every 0.5s
			// left arrow
			sprite.setTexture(arrows[0], true); // true to reset the sprite rectangle to the size of the new texture
			sprite.setPosition((sf::Vector2f){48.0*screenRatio,64.0*screenRatio});
			sprite.setScale((sf::Vector2f){screenRatio,screenRatio});
			window.draw(sprite);
			// right arrow
			sprite.setTexture(arrows[2], true); // true to reset the sprite rectangle to the size of the new texture
			sprite.setPosition((sf::Vector2f){240.0*screenRatio,64.0*screenRatio});
			sprite.setScale((sf::Vector2f){screenRatio,screenRatio});
			window.draw(sprite);
		}
		else{
			// left arrow
			sprite.setTexture(arrows[1], true); // true to reset the sprite rectangle to the size of the new texture
			sprite.setPosition((sf::Vector2f){48.0*screenRatio,64.0*screenRatio});
			sprite.setScale((sf::Vector2f){screenRatio,screenRatio});
			window.draw(sprite);
			// right arrow
			sprite.setTexture(arrows[3], true); // true to reset the sprite rectangle to the size of the new texture
			sprite.setPosition((sf::Vector2f){240.0*screenRatio,64.0*screenRatio});
			sprite.setScale((sf::Vector2f){screenRatio,screenRatio});
			window.draw(sprite);
			
			if(dt_Arrow>=1){ // reset counter after 1s so the numbers don't get massive
				clock.restart();
			}
		}

		window.display();
	}
	
	return 1;
}

int nameSelectWindow(sf::RenderWindow &window, std::string *name){
	window.clear();
	
	//Start clock to monitor elapsed time. It is MONOTONIC, does not go by system clock.
	const auto start{std::chrono::steady_clock::now()};
	
	
	return 1;
}

int loadingWindow(sf::RenderWindow &window, std::string name){
	window.clear();
	
	//Start clock to monitor elapsed time. It is MONOTONIC, does not go by system clock.
	const auto start{std::chrono::steady_clock::now()};
	
	
	return 1;
}

std::vector<int> getIndexes(int center, int size){
	// create indexes
	std::vector<int> indexes = {center-1,center,center+1};
	// wrap around if needed
	if(indexes[0]<0)
		indexes[0] = size-1;
	if(indexes[2]>=size)
		indexes[2] = 0;
	
	return indexes;
}
