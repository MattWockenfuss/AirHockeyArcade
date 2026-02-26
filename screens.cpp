#include "screens.hpp"
#include "fcns.hpp"

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
	// make sure window is not automatically skipped
	bool enterUnPressed = false;
	
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
	sf::Text play_text(give_money_font, "Press Enter To Go To Next Screen!", 35);
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
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !enterUnPressed){
				enterUnPressed = true;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && enterUnPressed){ // go to next screen
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
	// make sure we do not automatically skip window
	bool enterUnPressed = false;
	// get scale of window relative to resolution of assets
	double screenRatio = (double)(window.getSize().x) / 320.0;
	
	//Start clock to monitor elapsed time. It is MONOTONIC, does not go by system clock.
	sf::Clock clock;
	sf::Time time;
	float dt;
	float lap_Arrow;
	
	// images
	sf::Texture arrows[4];
	arrows[0].loadFromFile("assets/images/gameSelectArrow_L1.png");
	arrows[1].loadFromFile("assets/images/gameSelectArrow_L2.png");
	arrows[2].loadFromFile("assets/images/gameSelectArrow_R1.png");
	arrows[3].loadFromFile("assets/images/gameSelectArrow_R2.png");
	
	sf::Texture demos[3];
	demos[0].loadFromFile("assets/images/gameSelectImage_AH.png");
	demos[1].loadFromFile("assets/images/icon.png");
	demos[2].loadFromFile("assets/images/logo.png");
	
	sf::Sprite sprite(arrows[0]); // sprites cannot be created without a texture
	
	// animation vars
	int animate = 0;
	int centerFrame = 0;
	std::vector<int> indexes = getIndexes(centerFrame,3);
	std::vector<float> frames = {0.5,1.5,2.5};
	std::vector<double> xyz;
	
	// font
	sf::Font title_font;
	if (!title_font.openFromFile("assets/fonts/square_sans_serif_7.ttf")) { std::perror("File not found!"); }
	
	sf::Text title_text(title_font, "Air Hockey", 24.0*screenRatio);
	const sf::FloatRect textRect = title_text.getLocalBounds();
	title_text.setOrigin(textRect.getCenter());
	title_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, 14.0*screenRatio));
	
	//Main window loop
	while ( window.isOpen() ) {
		time = clock.restart();
		dt = time.asSeconds();
		lap_Arrow += dt;
		
		while (std::optional event = window.pollEvent()) {
			// close window
			if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
			// other events
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !enterUnPressed){ // prepare to go to next screen
				enterUnPressed = true;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && enterUnPressed){ // go to next screen
				return 1;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && animate==0){ // animate forward
				animate = 1;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && animate==0){ // animate backward
				animate = -1;
			}
		}
		
		if(animate==1){
			for(int i = 0; i<3; i++){
				frames[i] += dt;
				// wrap around
				if(frames[i]>3)
					frames[i] -= 3;
				// pause animation
				if(frames[0]>=1.5 && frames[0]-dt<1.5){
					frames[0] = 0.5;
					frames[1] = 1.5;
					frames[2] = 2.5;
					animate = 0;
					centerFrame -= 1;
					if(centerFrame<0)
						centerFrame = 2;
					indexes = getIndexes(centerFrame,3);
				}
			}
		}
		if(animate==-1){
			for(int i = 0; i<3; i++){
				frames[i] -= dt;
				// wrap around
				if(frames[i]<0)
					frames[i] += 3;
				// pause animation
				if(frames[2]<=1.5 && frames[2]+dt>1.5){
					frames[0] = 0.5;
					frames[1] = 1.5;
					frames[2] = 2.5;
					animate = 0;
					centerFrame += 1;
					if(centerFrame>2)
						centerFrame = 0;
					indexes = getIndexes(centerFrame,3);
				}
			}
		}
		
		window.clear();
		window.draw(title_text);
		
		// draw arrows
		if(lap_Arrow<=0.75){ // alternate frames every 0.75s
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
			
			if(lap_Arrow>=1.5) // prevent total time from getting too big
				lap_Arrow = 0;
		}
		
		// draw images
		for(int i = 0; i<3; i++){
			xyz = getAnimXYZ(frames[i]);
			sprite.setTexture(demos[indexes[i]],true);
			sprite.setPosition((sf::Vector2f){ xyz[0]*screenRatio , xyz[1]*screenRatio });
			sprite.setScale((sf::Vector2f){ xyz[2]/128*screenRatio , xyz[2]/128*screenRatio });
			window.draw(sprite);
		}
		
		window.display();
	}
	
	return 1;
}

int nameSelectWindow(sf::RenderWindow &window, sf::String *name){
	window.clear();
	// make sure we do not automatically skip window
	bool enterUnPressed = false;
	bool letterUnPressed = false;
	// get scale of window relative to resolution of assets
	double screenRatio = (double)(window.getSize().x) / 320.0;
	
	// font
	sf::Font font;
	if (!font.openFromFile("assets/fonts/square_sans_serif_7.ttf")) { std::perror("File not found!"); }
	
	sf::Text title_text(font, "Enter Your Name", 24.0*screenRatio);
	sf::FloatRect rect = title_text.getLocalBounds();
	title_text.setOrigin(rect.getCenter());
	title_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, 30.0*screenRatio));
	
	sf::Text name_text(font, *name, 24.0*screenRatio);
	rect = name_text.getLocalBounds();
	name_text.setOrigin(rect.getCenter());
	name_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, 80.0*screenRatio));
	
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::vector<std::vector<float>> coords = { {28,122},{50,122},{72,122},{94,122},{116,122},{138,122},{160,122},{182,122},{204,122},{226,122},{248,122},{270,122},{292,122} , {28,144},{50,144},{72,144},{94,144},{116,144},{138,144},{160,144},{182,144},{204,144},{226,144},{248,144},{270,144},{292,144} };
	sf::Text letter(font,"Default Text",14*screenRatio); // cannot create empty text
	
	// rectangle
	sf::RectangleShape selectRect( (sf::Vector2f){20*screenRatio,20*screenRatio});
	rect = selectRect.getLocalBounds();
	selectRect.setOrigin(rect.getCenter());
	int selectLoc = 0;
	selectRect.setPosition( (sf::Vector2f){coords[selectLoc][0]*screenRatio , coords[selectLoc][1]*screenRatio} );
	selectRect.setOutlineColor(sf::Color::White);
	selectRect.setFillColor(sf::Color::Transparent);
	selectRect.setOutlineThickness(1*screenRatio);
	
	//Main window loop
	while ( window.isOpen() ) {
		while (std::optional event = window.pollEvent()) {
			// close window
			if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
			// other events
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !enterUnPressed){ // prepare to go to next screen
				enterUnPressed = true;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && enterUnPressed){ // go to next screen
				return 1;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){ // move selection left
				selectLoc -= 1;
				if(selectLoc<0)
					selectLoc = 0;
				if(selectLoc==12)
					selectLoc = 13;
				selectRect.setPosition( (sf::Vector2f){coords[selectLoc][0]*screenRatio , coords[selectLoc][1]*screenRatio} );
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){ // move selection right
				selectLoc += 1;
				if(selectLoc>25)
					selectLoc = 25;
				if(selectLoc==13)
					selectLoc = 12;
				selectRect.setPosition( (sf::Vector2f){coords[selectLoc][0]*screenRatio , coords[selectLoc][1]*screenRatio} );
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){ // move selection up
				selectLoc -= 13;
				if(selectLoc<0)
					selectLoc += 13;
				selectRect.setPosition( (sf::Vector2f){coords[selectLoc][0]*screenRatio , coords[selectLoc][1]*screenRatio} );
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){ // move selection down
				selectLoc += 13;
				if(selectLoc>25)
					selectLoc -= 13;
				selectRect.setPosition( (sf::Vector2f){coords[selectLoc][0]*screenRatio , coords[selectLoc][1]*screenRatio} );
			}
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B) && !letterUnPressed){ // make sure no keys are pressed before regestering a press
				letterUnPressed = true;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && letterUnPressed){ // add letter
				*name = name_text.getString();
				if(name->getSize()<3){
					*name += alphabet[selectLoc];
					name_text.setString(*name);
					// reposition after adding letter
					rect = name_text.getLocalBounds();
					name_text.setOrigin(rect.getCenter());
					name_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, 80.0*screenRatio));
				}
				letterUnPressed = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B) && letterUnPressed){ // remove letter
				*name = name_text.getString();
				*name = name->substring(0, name->getSize()-1);
				name_text.setString(*name);
				// reposition after adding letter
				rect = name_text.getLocalBounds();
				name_text.setOrigin(rect.getCenter());
				name_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, 80.0*screenRatio));
				letterUnPressed = false;
			}
		}
		
		window.clear();
		window.draw(title_text);
		window.draw(name_text);
		for(int i = 0; i<26; i++){
			letter.setString(alphabet[i]);
			rect = letter.getLocalBounds();
			letter.setOrigin(rect.getCenter());
			letter.setPosition( (sf::Vector2f){coords[i][0]*screenRatio , coords[i][1]*screenRatio} );
			window.draw(letter);
		}
		window.draw(selectRect);
		window.display();
	}
	
	return 1;
}

int loadingWindow(sf::RenderWindow &window, sf::String name){
	window.clear();
	// make sure we do not automatically skip window
	bool enterUnPressed = false;
	// get scale of window relative to resolution of assets
	double screenRatio = (double)(window.getSize().x) / 320.0;
	
	//Start clock to monitor elapsed time. It is MONOTONIC, does not go by system clock.
	sf::Clock clock;
	sf::Time time;
	float dt;
	float lap_Arrow;
	
	// font
	sf::Font title_font;
	if (!title_font.openFromFile("assets/fonts/square_sans_serif_7.ttf")) { std::perror("File not found!"); }
	
	sf::Text title_text(title_font, "Air Hockey", 80);
	const sf::FloatRect textRect = title_text.getLocalBounds();
	title_text.setOrigin(textRect.getCenter());
	title_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 7.0f));
	
	//Main window loop
	while ( window.isOpen() ) {
		time = clock.restart();
		dt = time.asSeconds();
		lap_Arrow += dt;
		
		while (std::optional event = window.pollEvent()) {
			// close window
			if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
			// other events
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !enterUnPressed){ // prepare to go to next screen
				enterUnPressed = true;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && enterUnPressed){ // go to next screen
				return 1;
			}
		}
		
		window.clear();
		window.draw(title_text);
		
		window.display();
	}
	
	return 1;
}
