#ifndef SCREENS_HPP
#define SCREENS_HPP
    #include <iostream>
    #include <stdlib.h>
    #include <stdio.h>
    #include <chrono>
    #include <math.h>
    #include <optional>
    #include <SFML/Graphics.hpp>
    
    int titleWindow(sf::RenderWindow &window);
	
    int gameSelectWindow(sf::RenderWindow &window);
	
    int nameSelectWindow(sf::RenderWindow &window, std::string *name);
	
    int loadingWindow(sf::RenderWindow &window, std::string name);
	
	// helper functions
	std::vector<int> getIndexes(int center, int size);
    
#endif //TITLE_HPP
