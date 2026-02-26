#ifndef FCNS_HPP
#define FCNS_HPP
    #include <iostream>
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>
    #include <optional>
    #include <SFML/Graphics.hpp>

	std::vector<int> getIndexes(int center, int size);
	
	std::vector<double> getGameAnimXYZ(float frame);
	
	double getLoadAnimY(float frame);
	
#endif //FCNS_HPP
