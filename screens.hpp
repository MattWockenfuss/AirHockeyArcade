#ifndef SCREENS_HPP
#define SCREENS_HPP
    #include <iostream>
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>
    #include <optional>
    #include <SFML/Graphics.hpp>
    
    int titleWindow(sf::RenderWindow &window);
	
    int gameSelectWindow(sf::RenderWindow &window, std::vector<GameOption*> gameOptions);
	
    int nameSelectWindow(sf::RenderWindow &window, sf::String *name, GameOption* game);
	
    int loadingWindow(sf::RenderWindow &window, sf::String name, GameOption* game);
    
#endif //SCREENS_HPP
