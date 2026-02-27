#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "fcns.hpp"
#include "screens.hpp"

int main() {
	//Begin primary game loop
	//Load any necessary assets to memory

	//Define window object to use throughout system
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	sf::String name = "";
	int gameSelection;
	
	std::vector<GameOption*> gameOptions;
	gameOptions.push_back( new GameOption("Air Hockey","assets/images/gameSelectImage_AH.png","assets/images/ahmDot.png",sf::Color(19,0,223),sf::Color(155,0,0)) );
	gameOptions.push_back( new GameOption("Other Game","assets/images/icon.png","assets/images/iconDot.png",sf::Color(202,208,210),sf::Color(91,63,50)) );
	gameOptions.push_back( new GameOption("Another Game","assets/images/logo.png","assets/images/logoDot.png",sf::Color(255,193,0),sf::Color(15,15,77)) );
	
	//Iterate through all available modes, print to console
	//Uncomment these lines to test your local machine's available fullscreen options.
	/*
	for (std::size_t i = 0; i < modes.size(); i++) {
		sf::VideoMode mode = modes[i];
		std::cout << "Mode #: " << i << std::endl
		<< "    Mode Size: " << mode.width << ", " << mode.height << std::endl
		<< "    Mode bpp : " << mode.bitsPerPixel << std::endl;
	}
	*/

	//Chooses largest resolution and highest bpp, makes window fullscreen
	sf::RenderWindow window(modes[0], "Arcade", sf::State::Fullscreen);
	
	//Pass window to titlescreen, wait
	while(window.isOpen()){
		titleWindow(window);
		gameSelection = gameSelectWindow(window, gameOptions);
		nameSelectWindow(window, &name, gameOptions[gameSelection]);
		loadingWindow(window, name, gameOptions[gameSelection]);
	}
	return 0;
}
