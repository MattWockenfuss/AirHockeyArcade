#ifndef FCNS_HPP
#define FCNS_HPP
    #include <iostream>
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>
    #include <optional>
    #include <SFML/Graphics.hpp>
	
	struct GameOption{
		sf::String name;
		sf::Texture image;
		sf::Texture dot;
		sf::Color foreColor;
		sf::Color backColor;
		
		GameOption(sf::String name, std::string imgPath, std::string dotPath, sf::Color foreColor, sf::Color backColor);
	};

	std::vector<int> getIndexes(int center, int size);
	
	std::vector<double> getGameAnimXYZ(float frame);
	
	double getLoadAnimY(float frame);
	
#endif //FCNS_HPP
