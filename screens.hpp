#ifndef SCREENS_HPP
#define SCREENS_HPP
    #include <iostream>
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>
    #include <optional>
    #include <SFML/Graphics.hpp>
    
    int titleWindow(sf::RenderWindow &window);
	
    int gameSelectWindow(sf::RenderWindow &window);
	
    int nameSelectWindow(sf::RenderWindow &window, sf::String *name);
	
    int loadingWindow(sf::RenderWindow &window, sf::String name);
    
#endif //SCREENS_HPP
