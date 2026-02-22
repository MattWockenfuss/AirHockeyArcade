#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "title.hpp"

int main() {
	//Begin primary game loop
	//Load any necessary assets to memory

	//Define window object to use throughout system
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	
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
	titleWindow(window);

	return 0;
}
